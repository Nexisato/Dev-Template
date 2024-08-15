#include <thread>
#include <iostream>
#include <queue>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>

/**

事件循环：
初始化：事件源、事件队列、事件处理器 
 包括：IO事件、定时器、信号
开启循环：
    - 轮询就绪事件
    - 检测到就绪事件，进行事件分发
    - 事件处理器处理事件
    - 事件处理器执行完毕，继续轮询就绪事件 （上面这一步也可以分配为异步执行，主循环在分发完毕之后继续循环，不参与处理）
 */


using sys_clock = std::chrono::system_clock;


// 未封装具体消息，可扩展
struct event_t {
    sys_clock::time_point time_point;
    std::function<void()> callback;
    // ... string/int ... types
};


class event_loop {
private:
    struct event_cmp {
        bool operator() (const event_t& lhs, const event_t& rhs) {
            return lhs.time_point > rhs.time_point;
        }
    };
    // 小顶堆实现的事件队列，比较器为时间戳
    using evenet_queue_t = std::priority_queue<event_t, std::vector<event_t>, event_cmp>;   

    std::mutex g_mutex;
    std::condition_variable g_cv;
    evenet_queue_t g_event_queue;
    bool shutdown;

    event_t wait_one() {
        while (true) {
            std::unique_lock<std::mutex> locker(g_mutex);
            if (g_event_queue.empty()) {
                g_cv.wait(locker); // 等待事件队列非空，这里把锁释放了, lock 会在 wait 返回时重新加锁
            }
            if (g_event_queue.empty()) {
                continue;   // 这里是为了防止虚假唤醒，是因为 wait 会在条件变量被唤醒时返回，但是可能是虚假唤醒
            }
            auto event = g_event_queue.top();
            if (event.time_point > sys_clock::now()) {
                g_cv.wait_until(locker, event.time_point); // 等待时间到达
            } else {
                g_event_queue.pop();
                return event;
            }
        }
    }

    std::chrono::seconds g_seconds;

public:
    explicit event_loop(int _n): shutdown(false), 
        g_seconds(std::move(std::chrono::seconds(_n))) {
        add_event({sys_clock::now() + g_seconds, [this](){
          std::cout << "Event loop shutdown" << std::endl;
          this->shutdown = true;  
        }});
    }

    void add_event(event_t event) {
        {
            std::unique_lock<std::mutex> locker(g_mutex);
            g_event_queue.push(std::move(event));
        }
        g_cv.notify_one();
    }

    void add_event(std::function<void()> callback) {
        // callback 使用右值引用，避免拷贝，因为 callback 本身是一个函数对象
        add_event({sys_clock::now(), std::move(callback)});
    }

    void add_event(std::function<void()> callback, std::chrono::milliseconds delay) {
        add_event({sys_clock::now() + delay, std::move(callback)});
    }


    // 退出事件循环，不强制退出，等待事件处理完毕
    void quit() {
        add_event({sys_clock::now(), [this](){
            std::cout << "Event loop quit" << std::endl;
            this->shutdown = true;
        }});
    }

    bool empty() {
        std::unique_lock<std::mutex> locker(g_mutex);
        return g_event_queue.empty();
    }

    event_t top() {
        std::unique_lock<std::mutex> locker(g_mutex);
        return g_event_queue.top();
    }

    event_t pop() {
        std::unique_lock<std::mutex> locker(g_mutex);
        auto event = g_event_queue.top();
        g_event_queue.pop();
        return event;
    }


    void start() {
        while (!shutdown) {
            auto event = wait_one();
            event.callback();
        }
    }

};


int main() {
    event_loop* el_ptr = nullptr;

    event_t e1{
        sys_clock::now(),
        []() -> void{ 
            std::cout << "Hello, world!" << std::endl; 
        }
    };


    std::thread root_thread([&el_ptr] {
        el_ptr = new event_loop(10);
        el_ptr->start();

        delete el_ptr;
        el_ptr = nullptr;
        
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << "e1 no delay" << std::endl;
    el_ptr->add_event(e1);

    std::cout << "e2 delay for 2s" << std::endl;
    el_ptr->add_event([](){
        std::cout << "Hello, world! 2" << std::endl;
    }, std::chrono::milliseconds(2000));


    std::this_thread::sleep_for(std::chrono::seconds(3));
    el_ptr->quit();
    root_thread.join();

    return 0;
}
/***
 * @Author: nexisato
 * @Date: 2024-02-07 21:52:05
 * @FilePath: /Dev-Template/Pool-Threads/threadpool.h
 * @Description:
 * @
 * 线程池定义: 任务队列 + 线程池
 */
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

template <typename _Tp>
class TaskQueue {
private:
    std::mutex g_mutex;
    std::queue<_Tp> g_queue;

public:
    // 默认构造函数
    TaskQueue() {}
    // 移动构造函数
    TaskQueue(TaskQueue &&other) {}
    // 析构函数
    ~TaskQueue() {}

    /**
     * @brief 判断队列是否为空
     */
    bool isEmpty() {
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_queue.empty();
    }

    /**
     * @brief 获取队列大小
     */
    size_t getQueueSize() {
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_queue.size();
    }

    /**
     * @brief 队尾添加元素
     */
    void push(_Tp &task) {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_queue.emplace(task);
    }

    /**
     * @brief 队头删除元素
     */
    bool pop(_Tp &task) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_queue.empty()) {
            return false;
        }
        task = std::move(g_queue.front());
        g_queue.pop();
        return true;
    }
};

// 线程池
class ThreadPool {
private:
    bool isDown;
    std::mutex g_mutex;
    // 线程池
    std::vector<std::thread> g_threads;
    // 任务队列
    TaskQueue<std::function<void()>> g_taskqueue;
    // 条件变量
    std::condition_variable g_condition;

private:
    // 工作线程-仿函数
    class ThreadWorker {
    private:
        // 线程ID
        uint32_t worker_id;
        // 线程池
        ThreadPool *worker_pool;

    public:
        // 默认构造函数
        ThreadWorker(uint32_t id, ThreadPool *pool)
            : worker_id(id), worker_pool(pool) {}

        // 重载调用运算符
        void operator()() {
            std::function<void()> task;
            bool dequeued = false;
            while (!worker_pool->isDown) {
                {
                    std::unique_lock<std::mutex> lock(worker_pool->g_mutex);
                    // 若任务队列为空，阻塞当前线程
                    worker_pool->g_condition.wait(lock, [this] {
                        return !worker_pool->g_taskqueue.isEmpty();
                    });
                    dequeued = worker_pool->g_taskqueue.pop(task);
                }
                // 执行任务
                if (dequeued) {
                    task();
                }
            }
        };
    };

public:
    // 默认构造函数，初始化指定线程池中线程数量
    ThreadPool(size_t _nums)
        : isDown(false), g_threads(std::vector<std::thread>(_nums)) {}
    // 禁用拷贝构造函数
    ThreadPool(const ThreadPool &) = delete;
    // 禁用拷贝赋值构造函数
    ThreadPool &operator=(const ThreadPool &) = delete;
    // 禁用移动构造函数
    ThreadPool(ThreadPool &&) = delete;
    // 禁用移动赋值构造函数
    ThreadPool &operator=(ThreadPool &&) = delete;

    /**
     * @brief 初始化线程池
     */
    void init() {
        for (uint32_t i = 0; i < g_threads.size(); ++i) {
            g_threads.at(i) = std::thread(ThreadWorker(i, this));
        }
    }

    /**
     * @brief 关闭线程池
     */
    void shutdown() {
        this->isDown = true;
        g_condition.notify_all();

        // 清理所有没有执行的任务
        for (uint32_t i = 0; i < g_threads.size(); ++i) {
            if (g_threads.at(i).joinable()) {
                g_threads.at(i).join();
            }
        }
    }

    /**
     * @brief 异步提交任务
     */
    template <typename _F, typename... _Args>
    auto commit(_F &&f, _Args &&...args) -> std::future<decltype(f(args...))> {
        // 创建一个 std::packaged_task 包装起来的任务
        std::function<decltype(f(args...))()> func =
            std::bind(std::forward<_F>(f), std::forward<_Args>(args)...);
        auto task_ptr =
            std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        std::function<void()> wrapper_func = [task_ptr]() { (*task_ptr)(); };
        g_taskqueue.push(wrapper_func);

        g_condition.notify_one();
        return task_ptr->get_future();
    }
};
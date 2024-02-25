/*** 
 * @Author: nexisato
 * @Date: 2024-02-24 17:32:21
 * @FilePath: /Dev-Template/Design/singleton-lazy.cpp
 * @Description: 懒汉式单例模式，线程安全，基于互斥锁实现
 * @
 */

#include <iostream>
#include <mutex>
#include <thread>

// lazy initialization
class Singleton {
private:
    Singleton() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    ~Singleton() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    // 禁用 拷贝构造函数 和 拷贝赋值构造函数
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
private:
    static Singleton* m_instance;
    static std::mutex m_mutex;
public:
    static Singleton* &getInstance() {
        // 使用双检锁，提高效率
        if (m_instance == nullptr) {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_instance == nullptr) {
                // new (std::nothrow) 不抛出异常，失败返回nullptr
                m_instance = new (std::nothrow) Singleton();
            }
        }
        return m_instance;
    }
    static void destroyInstance() {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_instance) {
            delete m_instance;
            m_instance = nullptr;
        }
    }
    void Print() {
        std::cout << "Singleton::Instance address: " << this << std::endl;
    }
};
// 初始化静态成员变量
Singleton* Singleton::m_instance = nullptr;
std::mutex Singleton::m_mutex;


int main() {
    const int NUM_THREADS = 10;
    std::thread threads[NUM_THREADS];

    std::cout << "[main] thread start" << std::endl;

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i] = std::thread([i] {
            std::cout << "thread " << i << " start" << std::endl;
            Singleton* instance = Singleton::getInstance();
            instance->Print();
            std::cout << "thread " << i << " end" << std::endl;
        });
        threads[i].join();
    }


    Singleton::destroyInstance();

    std::cout << "[main] thread end" << std::endl;
    return 0;
}




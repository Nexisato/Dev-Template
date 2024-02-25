/*** 
 * @Author: nexisato
 * @Date: 2024-02-24 17:47:49
 * @FilePath: /Dev-Template/Design/singleton-hungry.cpp
 * @Description: 饿汉式单例模式-本身就线程安全
 * @
 */
#include <iostream>
#include <thread>

// hungry initialization
class Singleton {
private:
    Singleton(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    ~Singleton() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton* m_instance;
public:
    static Singleton* &getInstance() {
        return m_instance;
    }
    static void destroyInstance() {
        if (m_instance) {
            delete m_instance;
            m_instance = nullptr;
        }
    }
    void print() {
        std::cout << "Singleton::Instance address: " << this << std::endl;
    }
};
// 初始化即创建
Singleton* Singleton::m_instance = new (std::nothrow) Singleton();

int main() {
    std::cout << "[main] thread start" << std::endl;
    const int NUM_THREADS = 10;
    std::thread threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = std::thread([i]() {
            std::cout << "thread " << i << " start" << std::endl;
            Singleton::getInstance()->print();
            std::cout << "thread " << i << " end" << std::endl;
        });
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
    std::cout << "[main] thread end" << std::endl;



    return 0;
}

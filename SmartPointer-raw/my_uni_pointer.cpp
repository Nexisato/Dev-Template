/*** 
 * @Author: nexisato
 * @Date: 2024-02-25 21:14:36
 * @FilePath: /Dev-Template/SmartPointer-raw/my_uni_pointer.cpp
 * @Description: 实现 unique_ptr
 * @
 */
#include <iostream>
#include <utility>

template <typename T>
class MyUniPtr {
private:
    T* _ptr;
public:
    MyUniPtr() : _ptr(nullptr) {}

    explicit MyUniPtr(T* ptr) : _ptr(ptr) {}
    ~MyUniPtr() {
        if (_ptr) {
            delete _ptr;
        }
    }
    MyUniPtr(const MyUniPtr<T>&) = delete;
    MyUniPtr<T>& operator=(const MyUniPtr<T>&) = delete;

    MyUniPtr(MyUniPtr<T>&& other) : _ptr(other._ptr) {
        other._ptr = nullptr;
    }
    MyUniPtr<T>& operator=(MyUniPtr<T>&& other) {
        if (this != &other) {
            if (_ptr) {
                delete _ptr;
            }
            _ptr = other._ptr;
            other._ptr = nullptr;
        }
        return *this;
    }
    T* operator->() const {
        return _ptr;
    }
    T& operator*() const {
        return *_ptr;
    }
    T* get() const {
        return _ptr;
    }
    void reset(T* ptr = nullptr) {
        if (_ptr) {
            delete _ptr;
        }
        _ptr = ptr;
    }

};
int main() {

    MyUniPtr<int> p1(new int(10));
    std::cout << "p1: " << *p1 << std::endl;
    MyUniPtr<int> p2 = std::move(p1);
    std::cout << "p2: " << *p2 << std::endl;
    std::cout << "p1: " << p1.get() << std::endl;
    std::cout << "p2: " << p2.get() << std::endl;
    p2.reset(new int(20));
    std::cout << "p2: " << *p2 << std::endl;
    return 0;
}

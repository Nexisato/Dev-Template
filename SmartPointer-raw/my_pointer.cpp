/*** 
 * @Author: nexisato
 * @Date: 2024-02-21 10:17:22
 * @FilePath: /Dev-Template/SmartPointer-raw/my_pointer.cpp
 * @Description: 手写智能指针 shared_ptr
 * @
 */
#include <iostream>
#include <utility>

template<typename T>
class MyPtr {
private:
    T*  _ptr;
    int*  _count;
    void release() {
        if (_count && --(*_count) == 0) {
            delete _ptr;
            delete _count;
        }
    }
public:
    MyPtr(): _ptr(nullptr), _count(new int(0)) {}
    explicit MyPtr(T* ptr = nullptr) : _ptr(ptr), _count(ptr ? new int(1) : nullptr) {
    }


    // copy
    MyPtr(const MyPtr<T>& other) : _ptr(other._ptr), _count(other._count) {
        if (other._count) {
            ++(*_count);
        }
    }
    // copy assignment
    MyPtr<T>& operator=(const MyPtr<T>& other) {
        if (this != &other) {
            _ptr = other._ptr;
            _count = other._count;
            if (other._count) {
                ++(*_count);
            }
        }
        return *this;
    }

    // move
    MyPtr(MyPtr<T>&& other): _ptr(other._ptr), _count(other._count) {
        other._ptr = nullptr;
        other._count = nullptr;
    }
    
    // move assignment 
    MyPtr<T>& operator=(MyPtr<T>&& other) {
        if (this != &other) {
            release();
            _ptr = other._ptr;
            _count = other._count;
            other._ptr = nullptr;
            other._count = nullptr;
        }
        return *this;
    }


    ~MyPtr() {
        release();
    }

    T& operator*() const {
        return *_ptr;
    
    }

    T* operator->() const {
        return _ptr;
    }

    int get_reference_count() const {
        return _count ? *_count : 0;
    }


};


int main() {

    MyPtr<int> p1(new int(42));
    std::cout << "p1: " << *p1 << " ref count: " << p1.get_reference_count() << std::endl;

    // copy
    MyPtr<int> p2 = p1;
    std::cout << "p2: " << *p2 << " ref count: " << p2.get_reference_count() << std::endl;

    // move
    MyPtr<int> p3 = std::move(p1);
    std::cout << "p3: " << *p3 << " ref count: " << p3.get_reference_count() << std::endl;

    std::cout << "p1 ref count: " << p1.get_reference_count() << std::endl;


    return 0;
}
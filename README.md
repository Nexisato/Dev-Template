# Dev-Template

## Introduction
Record the code template that may be used frequently.

## Templates
### 1. Qt5-Template
- IDE: CLion (Recommended)
- Compiler: MSVC, g++
- Environment: Windows, Linux, macOS, 

### 2. ThreadPool

- 手动实现一个线程池 (基于 C++11，C++14)

#### References:
- [(*AU) ZhiHu-Skykey-基于C++11实现线程池](https://zhuanlan.zhihu.com/p/367309864)

### 3. Smart Pointer (Raw)
- 仿写 `share_ptr`，支持的功能
    - 引用计数
    - 拷贝构造函数 & 拷贝赋值运算符
    - 移动构造函数 & 移动赋值运算符
    - 解引用运算符, 指针运算符 重载

> 在 C++ 中，类模板的成员函数在具体化时，可以访问同一模板实例化类型的其他对象的私有成员。这是因为模板实例化产生的具体类被视为同一个类的不同对象，而不是完全独立的类型。因此在类模板的拷贝构造函数和赋值操作中访问 other._ptr 和 other._count 时，你实际上是在访问同一个类（具体化后的 MyPtr<T> 类型）的另一个实例的私有成员。这在 C++ 中是允许的，因为这些操作被视为类内部操作，它们有权限访问同一个类的其他实例的私有和保护成员。

- 仿写 `unique_ptr` 
    - 不带引用计数；
    - 禁用 拷贝构造函数 和 拷贝赋值运算符；
    - 支持移动构造函数 和 移动赋值运算符；
    - 支持 `reset` 功能; 


### 4. 设计模式
- 实现几个语言常用的设计模式模板.

- C++ 单例模式. 



### . More...
- Memory Pool
- ncnn-CMake-Template (TODO)
- Deep-Learning-Template (TODO)
    - IDE: VSCode, PyCharm

- Vue-FrontEnd-node-Template(TODO)
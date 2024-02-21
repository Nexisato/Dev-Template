# Dev-Template

## Introduction
Record the code template that may be used frequently.

## Templates
### 1. Qt5-Template
- IDE: CLion (Recommended)
- Compiler: MSVC, g++
- Environment: Windows, Linux, macOS, 

### 2. ThreadPool

#### References:
- [(*AU) ZhiHu-Skykey-基于C++11实现线程池](https://zhuanlan.zhihu.com/p/367309864)

### 3. Smart Pointer (Raw)
仿写 `share_ptr`，支持的功能
- 引用计数
- 拷贝构造函数 & 拷贝赋值运算符
- 移动构造函数 & 移动赋值运算符
- 解引用运算符, 指针运算符 重载

> 在 C++ 中，类模板的成员函数在具体化时，可以访问同一模板实例化类型的其他对象的私有成员。这是因为模板实例化产生的具体类被视为同一个类的不同对象，而不是完全独立的类型。因此在类模板的拷贝构造函数和赋值操作中访问 other._ptr 和 other._count 时，你实际上是在访问同一个类（具体化后的 MyPtr<T> 类型）的另一个实例的私有成员。这在 C++ 中是允许的，因为这些操作被视为类内部操作，它们有权限访问同一个类的其他实例的私有和保护成员。



### 4. Memory Pool

### 3. More...
- ncnn-CMake-Template (TODO)
- Deep-Learning-Template (TODO)
    - IDE: VSCode, PyCharm

- Vue-FrontEnd-node-Template(TODO)
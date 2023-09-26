/***
 * @Author: nexisato
 * @Date: 2024-02-07 23:01:11
 * @FilePath: /Dev-Template/Pool-Threads/test_threadpool.cpp
 * @Description:
 * @
 * 线程池测试函数
 */

#include <iostream>
#include <random>
#include "threadpool.h"


// Will be used to obtain a seed for the random number engine
std::random_device rd;

// Standard mersenne_twister_engine seeded with rd()，获得随机数 mt
std::mt19937 mt(rd());

std::uniform_int_distribution<int> dist(-200, 200);

auto rnd = std::bind(dist, mt);

using chrono_time = std::chrono::_V2::system_clock::time_point;
/**
 * @brief Calculate the Time Duration between 2 points
 *
 * @param t0 : start_time
 * @param t1 : end_time
 * @return double / (unit):milliseconds
 */
double count_time(chrono_time t0, chrono_time t1) {
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    double res = double(duration.count()) *
                 std::chrono::microseconds::period::num /
                 std::chrono::milliseconds::period::den;
    return res;
}

//设置线程睡眠时间
void simulate_hard_computation() {
    std::this_thread::sleep_for(std::chrono::milliseconds(400 + rnd()));
}

// 添加两个数字的简单函数并打印结果，固定时延 200~600 ms
void multiply(const int a, const int b) {
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
}

// 添加并输出结果
void multiply_output(int &out, const int a, const int b) {
    simulate_hard_computation();
    out = a * b;
    std::cout << a << " * " << b << " = " << out << std::endl;
}

// 结果返回
int multiply_return(const int a, const int b) {
    simulate_hard_computation();
    const int res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
    return res;
}

int main() {
    ThreadPool g_pool(8);
    // 初始化线程池
    g_pool.init();

    // 添加两个数字的简单函数并打印结果，固定时延 200~600 ms
    for (int i = 1; i < 5; ++i) {
        for (int j = 1; j < 5; ++j) {
            g_pool.commit(multiply, i, j);
        }
    }

    // 使用return参数提交函数
    auto future1 = g_pool.commit(multiply_return, 5, 6);
    int output_return = future1.get();  //获取返回值
    std::cout << "[Example-Future1] output_return: " << output_return
              << std::endl;

    // 使用引用参数提交函数
    int output_ref;
    auto future2 = g_pool.commit(multiply_output, std::ref(output_ref), 5, 3);
    future2.get();
    std::cout << "[Example-Future2] output_ref: " << output_ref << std::endl;

    // 关闭线程池
    g_pool.shutdown();

    return 0;
}
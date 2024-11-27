#include <spdlog/spdlog.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <string>
#include <signal.h>
#include <atomic>

#include "zmq_addon.hpp"

using sys_clock = std::chrono::system_clock;


static std::atomic<int> g_shutdown(0);
void sighandler(int signum) {
    spdlog::warn("SIGINT");
    g_shutdown.store(1);
}

int main() {
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::rep);
    sock.bind("tcp://*:5555");

    // 注册信号处理函数
    signal(SIGINT, sighandler);

    while (g_shutdown.load() == 0) {
        int rand_num = rand() % 300;
        std::this_thread::sleep_for(std::chrono::milliseconds(rand_num));
        std::string req_data = "req_random_data_" + std::to_string(rand_num);
        zmq::message_t request(&req_data, sizeof(req_data));
        spdlog::info("send: {}", req_data);
        
        sock.recv(&request);

        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        int rand_next = rand() % 256;
        std::string rep_data = "reply_data_" + std::to_string(rand_next);
        zmq::message_t reply(&rep_data, sizeof(rep_data));
        sock.send(reply);

    }
    spdlog::info("Shutdown");


    return 0;

}
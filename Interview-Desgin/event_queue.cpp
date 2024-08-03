#include <iostream>
#include <queue>
#include <vector>
#include <chrono>
#include <optional>
#include <thread>

using namespace std::chrono;

struct Event {
    steady_clock::time_point enqueue_time;
    milliseconds expiration_time;
    int data; // 可以替换为任何你需要的类型

    bool is_expired(const steady_clock::time_point& current_time) const {
        return current_time - enqueue_time >= expiration_time;
    }
};

struct EventComparator {
    bool operator()(const Event& a, const Event& b) const {
        return (a.enqueue_time + a.expiration_time) > (b.enqueue_time + b.expiration_time);
    }
};

class TimeoutEventQueue {
public:
    void push(const Event& event) {
        event_queue.push(event);
    }

    std::vector<Event> pop() {
        std::vector<Event> expired_events;
        auto current_time = steady_clock::now();

        while (!event_queue.empty() && event_queue.top().is_expired(current_time)) {
            expired_events.push_back(event_queue.top());
            event_queue.pop();
        }

        return expired_events;
    }

    std::optional<Event> get_nearest_expiration_event() const {
        if (event_queue.empty()) {
            return std::nullopt;
        }
        return event_queue.top();
    }

private:
    std::priority_queue<Event, std::vector<Event>, EventComparator> event_queue;
};

int main() {
    TimeoutEventQueue queue;

    auto now = steady_clock::now();
    queue.push({now, 100ms, 1});
    queue.push({now, 200ms, 2});
    queue.push({now, 50ms, 3});

    std::this_thread::sleep_for(150ms);

    auto expired_events = queue.pop();
    std::cout << "Expired events: " << expired_events.size() << std::endl;
    for (const auto& event : expired_events) {
        std::cout << "Event data: " << event.data << std::endl;
    }

    auto nearest_event = queue.get_nearest_expiration_event();
    if (nearest_event) {
        std::cout << "Nearest expiration event data: " << nearest_event->data << std::endl;
    }

    return 0;
}
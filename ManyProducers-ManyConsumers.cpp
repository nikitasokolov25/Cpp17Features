#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>

using namespace std::chrono_literals;

struct pcout: public std::stringstream {        //parallel concurrent cout
    static inline std::mutex mut;
    ~pcout() {
        std::lock_guard<std::mutex> lg{ mut };
        std::cout << rdbuf();
        std::cout.flush();
    }
};

static void printPcout(int id) {
    pcout{} << "hello from pcout " << id << "\n";
}

std::queue<std::size_t> q;
std::mutex qmutex;
bool production_stopped = false;
std::condition_variable go_produce;
std::condition_variable go_consume;

static void producer(std::size_t id, std::size_t items, std::size_t stock) {
    for (std::size_t i = 0; i < items; ++i) {
        std::unique_lock<std::mutex> lock(qmutex);
        go_produce.wait(lock, [&] {return q.size() < stock; });
        q.push(id * 100 + i);
        pcout{} << "Producer " << id << "---> item" << std::setw(3) << q.back() << "\n";

        go_consume.notify_all();
        std::this_thread::sleep_for(90ms);
    }
    pcout{} << "Exit from Producer number " << id << "\n";
}

static void consumer(std::size_t id) {
    while (!production_stopped || !q.empty()) {
        std::unique_lock<std::mutex> lock(qmutex);
        if (go_consume.wait_for(lock, 1s, [] {return !q.empty(); })) {
            pcout{} << "                 item " << std::setw(3) << q.front() << "--->Consumer " << id << "\n";
            q.pop();
            go_produce.notify_all();
            std::this_thread::sleep_for(130ms);
        }
    }
    pcout{} << "Exit from Consumer number " << id << "\n";
}

int main() {
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (std::size_t i = 0; i < 3; i++) {
        producers.emplace_back(producer, i, 15, 5);
    }

    for (std::size_t i = 0; i < 5; i++) {
        consumers.emplace_back(consumer, i);
    }

    for (auto& p : producers) p.join(); 
    production_stopped = true;

    for (auto& c : consumers) c.join();
    return 0;
}

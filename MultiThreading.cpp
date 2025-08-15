#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <shared_mutex>
#include <algorithm>
#include <future>
#include <chrono>

std::mutex mut1;
std::mutex mut2;

using namespace std::chrono_literals;

//2 functions making a deadlock because of both waiting
static void deadlock1() {
    std::cout << "f1 is waiting mutex1" << std::endl;
    std::lock_guard<std::mutex> lg1(mut1);
    std::this_thread::sleep_for(100ms);

    std::cout << "f1 is waiting mutex2" << std::endl;
    std::lock_guard<std::mutex> lg2(mut2);
    std::cout << "f1 got both mutexes" << std::endl;
}

static void deadlock2() {
    std::cout << "f2 is waiting mutex1" << std::endl;
    std::lock_guard<std::mutex> lg1(mut2);
    std::this_thread::sleep_for(100ms);

    std::cout << "f2 is waiting mutex2" << std::endl;
    std::lock_guard<std::mutex> lg2(mut1);
    std::cout << "f2 got both mutexes" << std::endl;
}

//2 functions don't make a deadlock with a scoped_lock (similar mutexes locking and unlocking)
void noDeadlock1() {
    std::scoped_lock sp1{ mut1, mut2 };
    std::cout << "f1 got both mutexes" << std::endl;
}

void noDeadlock2() {
    std::scoped_lock sp2{ mut1, mut2 };
    std::cout << "f2 got both mutexes" << std::endl;
}

void deadlockStart() {
    std::thread t1{ deadlock1 };
    std::thread t2{ deadlock2 };
    t1.join();
    t2.join();
}

void noDeadlockStart() {
    std::thread t1{ noDeadlock1 };
    std::thread t2{ noDeadlock2 };
    t1.join();
    t2.join();
}

struct pcout: public std::stringstream {        //parallel concurrent cout
    static inline std::mutex mut;
    ~pcout() {
        std::lock_guard<std::mutex> lg{ mut };
        std::cout << rdbuf();
        std::cout.flush();
    }
};

static void printCout(int id) {
    std::cout << "hello from cout " << id << "\n";
}

static void printPcout(int id) {
    pcout{} << "hello from pcout " << id << "\n";
}

std::once_flag callflag;

static void printC() {
    std::cout << '!';
}
static void printOnce(int id) {
    std::call_once(callflag, printC);//printC will be called only once by the first of the threads
    std::cout << id;
}

static auto countA(const std::string& input) {
    return std::count_if(std::begin(input), std::end(input), [](char c) {return c == 'A'; });
}

static auto countB(const std::string& input) {
    return std::count_if(std::begin(input), std::end(input), [](char c) {return c == 'B'; });
}

static auto countC(const std::string& input) {
    return std::count_if(std::begin(input), std::end(input), [](char c) {return c == 'C'; });
}

int main() {
    std::cout << std::thread::hardware_concurrency() << "\n";
    noDeadlockStart();
    //deadlockStart();

    std::vector<std::thread> tv;
    //bad concurrent cout
    {
        for (int i = 0; i < 10; ++i)
        {
            tv.emplace_back(printCout, i);
        }
        for (auto& t : tv) t.join();
    }
    tv.clear();

    //good concurrent cout
    {
        for (int i = 0; i < 10; ++i)
        {
            tv.emplace_back(printPcout, i);
        }
        for (auto& t : tv) t.join();
    }
    tv.clear();
    
    //print ! once
    {
        for (int i = 0; i < 10; ++i)
        {
            tv.emplace_back(printOnce, i);
        }
        for (auto& t : tv) t.join();
    } 
    
    std::cout << "\n";
    tv.clear();
    std::string input;
    std::cin >> input;
    
    auto countAResult = std::async(std::launch::async, countA, input);//returns later std::future object which be ready after .get()
    auto countBResult = std::async(std::launch::async, countB, input);
    auto countCResult = std::async(std::launch::async, countC, input);

    std::cout << countAResult.get() << " " << countBResult.get() << " " << countCResult.get() << "\n";

    return 0;
}

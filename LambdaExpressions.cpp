#include <iostream>
#include <vector>
#include <functional>
#include <deque>
#include <list>

static auto consumer(auto& container) { //auto parameter in non-lambda function is available in C++ 20 and later
    return [&](auto value) {
        container.push_back(value);
    };
}

static void print(const auto& c) {
    for (auto i : c) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}


int main()
{
    std::vector<int> v;
    int r = 4;
    //& - reference to all variables
    //= - copying all variables
    //&v - reference to vector
    //r - copying variable r
    std::cout << [&v, r]() { v.push_back(r); return v.back(); }() << "\n";

    //create different types of containers
    std::vector<int> vv;
    std::deque<int> q;
    std::list<int> l;

    //make a polimorphism with std::function
    const std::vector<std::function<void(int)>> consumers{ consumer(vv), consumer(q), consumer(l) };
    for (int i{ 0 }; i < 10; ++i) {
        for (auto&& consume : consumers) {
            consume(i);
        }
    }
    print(vv);
    print(q);
    print(l);

    return 0;
}


#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <deque>
#include <queue>
#include <list>
#include <iterator>

//make an iterator for Fibonacci sequence
class Fibit //iterator class
{
    std::size_t number {0};
    std::size_t a {0};
    std::size_t b {1};
public:
    Fibit() = default;
    explicit Fibit(const std::size_t& number_) 
        : number(number_)
    {
    }

    std::size_t operator*() const {
        return b;
    }

    Fibit& operator++() {
        const std::size_t oldB = b;
        b += a;
        a = oldB;
        ++number;
        return *this;
    }

    bool operator!=(const Fibit& f) {
        return (this->number != f.number);
    }
};

class FibRange //range class
{
    std::size_t end_n;
public:
    FibRange(const std::size_t& end_n_)
        : end_n {end_n_}
    {}
    Fibit begin() const { return Fibit(); };
    Fibit end() const { return Fibit(end_n); };
};

//reverse adapter for iterator
void makeAndShowReverseAdapter()
{
    std::list<int> listint {1, 2, 3, 4, 5, 6, 7};

    //if rbegin and rend is available
    std::copy(listint.rbegin(), listint.rend(), std::ostream_iterator<int>{ std::cout, " " });

    //if rbegin and rend is unavailable
    std::copy(std::make_reverse_iterator(std::end(listint)), std::make_reverse_iterator(std::begin(listint)),
        std::ostream_iterator<int> {std::cout, " "});
}

int main()
{
    for (const std::size_t i : FibRange(10))
        std::cout << i << "\n";

    makeAndShowReverseAdapter();
    return 0;
}


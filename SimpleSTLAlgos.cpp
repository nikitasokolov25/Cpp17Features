#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <functional>

template<typename C> // function for print iterator data
static auto opt_print(const C& container) {
    return [end_it(std::end(container))](const auto& item) {
        if (item != end_it) std::cout << *item << "\n";
        else std::cout << "<end>\n";
    };
}

template<typename It>
static void printSubstring(It it, std::size_t chars) {
    std::copy_n(it, chars, std::ostream_iterator<char>{std::cout});
    std::cout << "\n";
}

void print(const std::vector<int>& v) {
    std::copy(std::begin(v), std::end(v), std::ostream_iterator<int> {std::cout, " "});
    std::cout << "\n";
}

void testCopyAndSort() {
    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::cout << std::is_sorted(std::begin(v), std::end(v)) << "\n";

    std::random_device rd;
    std::mt19937 g{ rd() };//randomizer for shuffle
    std::shuffle(std::begin(v), std::end(v), g);

    print(v);

    std::set<int> s{};
    std::copy(std::begin(v), std::end(v), std::inserter(s, std::begin(s)));//adapter for insert from vector to set

    std::copy(std::begin(s), std::end(s), std::ostream_iterator<int> {std::cout, " "});
    std::cout << "\n";

    std::partition(std::begin(v), std::end(v), [](int i) { return i < 6; });
    print(v);
}

void testDeleteElements() {
    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    {
        auto new_end = std::remove(std::begin(v), std::end(v), 2);//for removing extra space after deleting
        v.erase(new_end, std::end(v));
    }
    print(v);

    {
        auto new_end = std::remove_if(std::begin(v), std::end(v), [](int i) { return i % 2 != 0; });
        v.erase(new_end, std::end(v));
    }
    print(v);
}

void testChangeElements() {
    std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::replace(std::begin(v), std::end(v), 4, 123);
    print(v);

    std::transform(std::begin(v), std::end(v), std::ostream_iterator<int> {std::cout, " "}, //change and copy function
        [](int i) {return i * i; });
}

void testSearchElements() {
    std::vector<int> v {10, 2, 5, 4, 3, 6, 8, 7, 1, 9};
    auto print_int(opt_print(v)); // for print v's iterators

    auto find5 = std::find(std::begin(v), std::end(v), 5);//simple find returns an iterator or std::end
    print_int(find5);

    auto given5 = std::find_if(std::begin(v), std::end(v), [](int i) { return i == 5; });//mostly for user types and for unknown element fields
    print_int(given5);
   
    std::sort(std::begin(v), std::end(v));//for binary search
    bool contains7 = std::binary_search(std::begin(v), std::end(v), 7);//returns only "found" or no
    std::cout << contains7 << "\n";

    auto [lower_it, upper_it] = std::equal_range(std::begin(v), std::end(v), 7);//make a binary search and returns the pair of iterators(results of std::lower_bound and std::upper_bound)
    print_int(lower_it);
    print_int(upper_it);
}

void testSearchSubstring() {
    const std::string s = "I like C++ very much!";
    const std::string subs = "ery";

    {
        //old std::search version
        auto match = std::search(std::begin(s), std::end(s), std::begin(subs), std::end(subs));
        printSubstring(match, 3);
    }

    {
        //From C++ 17 std::search has a 3rd parameter - object with possibility to change method of search
        auto match = std::search(std::begin(s), std::end(s), std::default_searcher(std::begin(subs), std::end(subs)));
        printSubstring(match, 3); 
    }

    {
        //Boyer-Moore search algorithm
        auto match = std::search(std::begin(s), std::end(s), std::boyer_moore_searcher(std::begin(subs), std::end(subs)));
        printSubstring(match, 3);
    }

    {
        //Boyer-Moore-Horspool search algorithm
        auto match = std::search(std::begin(s), std::end(s), std::boyer_moore_horspool_searcher(std::begin(subs), std::end(subs)));
        printSubstring(match, 3);
    }

}

int main()
{   
    testCopyAndSort();
    testDeleteElements();
    testChangeElements();
    testSearchElements();
    testSearchSubstring();
    return 0;
}


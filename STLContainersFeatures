#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <deque>
#include <queue>
#include <list>

struct testObject {
    std::string name;
    int cout;
};

struct coord {
    int x;
    int y;
};
bool operator==(const coord& l, const coord& r)// for hashing
{
    return (l.x == r.x && l.y == r.y);
}

template<>
struct std::hash<coord> //self hashing for coord type
{
    using argument_type = coord;
    using result_type = std::size_t;
    result_type operator()(const argument_type &c) const
    {
        return static_cast<result_type>(c.x) + static_cast<result_type>(c.y);
    }
};


//================ Vector useful operations ===================
template<typename T>
void printVector(const std::vector<T>& v)
{
    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

//Fast delete
template<typename T>
void vectorFastDelete(std::vector<T>& v, std::size_t index)
{
    if (index < v.size())
    {
        v.at(index) = std::move(v.back());
        v.pop_back();
    }
}

//Fast insert to sorted vector
template<typename T>
void sortedVectorFastInsert(std::vector<T>& v, const T& elem)
{
    const auto it = std::lower_bound(v.begin(), v.end(), elem);
    v.insert(it, elem);
}


//================ Maps useful operations ===================
template<typename C>
void printMap(const C& c)
{
    for (const auto& [key, value] : c)
    {
        std::cout << key << " " << value << "\n";
    }
    std::cout << "\n";
}

//Fast insert
template<typename C, typename T>
void insertToMapFast(C& c, const T& elem) //insert without call extra constructor(for big structures, not for std::string :)) 
{
    auto [iterator, success] = c.try_emplace(elem, 1);
    if (!success) {
        iterator->second += 1;
    }
}

//InsertWithHint
template<typename C>
void insertToMapWithHint(C& c, std::pair<std::string, std::size_t> elem)//true hint optimizes the insert, false - do nothing
{
    auto insert_it(std::end(c));
    c.insert(insert_it, { elem.first, elem.second });
}

//Fast Change Map Keys
template<typename C>
void fastChangeMapKeys(C& c, int idx1, int idx2)
{
    auto a = c.extract(idx1);//no extra copies and allocates
    auto b = c.extract(idx2);
    std::swap(a.key(), b.key());

    c.insert(std::move(a));
    c.insert(std::move(b));
}

//Self-Hashing for user types
void selfHashingMap()
{
    std::unordered_map<coord, int> m{ {{0,0}, 1}, { {1,1}, 2 }, {{2,2},3} };
    m.insert({ {5,5}, 6 });
    for (const auto& [key, value] : m)
        std::cout << key.x << ", " << key.y << ": " << value << "\n";
    std::cout << "\n";
}

//==========Priority Queue==================
void fillAndPrintPriorityQueue() //using first value in std::pair for a priority of deal
{
    std::initializer_list<std::pair<int, std::string>> deals
    { {0, "watch TV"}, {1, "wash dishes"}, {2, "do homework"}, {0, "play computer games"} };

    std::priority_queue<std::pair<int, std::string>> pq;
    for (const auto& i : deals)
        pq.push(i);

    while (!pq.empty())
    {
        std::cout << pq.top().first << " " << pq.top().second << "\n";
        pq.pop();
    }
}


int main()
{
    std::vector<int> v = { 1, 4, 7, 10, 13, 16, 19 };
    printVector(v);
    vectorFastDelete(v, 4);
    sortedVectorFastInsert(v, 21);
    printVector(v);


    std::map<std::string, std::size_t> testMap = { {"abc", 1}, {"def", 2}, {"ghi", 3}, {"aaa", 1}, {"klm", 1}, {"nop", 1}};
    printMap(testMap);
    insertToMapFast(testMap, "abc");
    insertToMapWithHint(testMap, { "xyz", 1 });

    std::map<int, std::string> testMap2 = { {1, "abc"}, {2, "def"}, {3, "ghi"}, {4, "aaa"}, {5, "klm"}, {6, "nop"}};
    printMap(testMap2);
    fastChangeMapKeys(testMap2, 3, 5);
    printMap(testMap2);

    selfHashingMap();

    fillAndPrintPriorityQueue();
    return 0;
}

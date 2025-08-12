#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <memory>
#include <algorithm>

class Human {
public:
    Human() {};
    Human(std::string name, int age, std::string city): name(name), age(age), city(city) 
    { std::cout << "Human constructor\n"; }
    std::string name = "";
    int age = 0;
    std::string city = "";
    virtual ~Human() { std::cout << "Human destructor\n"; }
};

class ChessPlayer : public Human {
public:
    ChessPlayer() { std::cout << "ChessPlayer constructor\n"; }
    double rating = 0.0;
    int worldRank = 0;
    virtual ~ChessPlayer() { std::cout << "ChessPlayer destructor\n"; }
};

std::string getName(std::unique_ptr<Human> hum) {
    if (!hum) return "";
    return hum->name;
}

double getRating(std::shared_ptr<Human> hum) {
    if (!hum) return 0.0;
    return hum->age;
}

int main() {
    {
        std::unique_ptr<Human> hum1{new Human {"Magnus", 32, "Oslo" } };
        std::unique_ptr<Human> hum2(new ChessPlayer);
    }
    std::cout << getName(std::make_unique<Human>("Nepo", 34, "Moscow")) << "\n";

    {
        std::shared_ptr<Human> hum1{ new Human {"Magnus", 32, "Oslo"}};
        std::shared_ptr<Human> hum2{ new Human {"Nepo", 34, "Moscow"}};
        std::cout << hum1.use_count() << "\n";
        hum2 = hum1;
        std::cout << hum1.use_count() << "\n";
        std::weak_ptr<Human> hum3 = hum1;
        std::cout << hum1.use_count() << "\n";
    }

    {
        std::shared_ptr<Human> humanPtr{ new Human {"Magnus", 32, "Oslo"}};
        std::shared_ptr<std::string> namePtr(humanPtr, &humanPtr->name); //pointer to a single field, but
        //it has access to the control unit of Human in humanPtr
        humanPtr.reset();
        std::cout << *namePtr << "\n"; // it works but humanPtr was reseted
    }
    return 0;
}

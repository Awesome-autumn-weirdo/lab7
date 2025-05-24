// main.cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> myMap = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };

    for (const auto& [key, value] : myMap) {
        std::cout << key << ": " << value << "\n";
    }

    return 0;
}

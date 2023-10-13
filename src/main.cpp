#include <map>
#include <iostream>
#include <allocator.h>

int factorial(int number) {
    if (number < 0) return -1;
    if (number == 0) return 1; 
    
    int fact = 1;
    for (int i = 1; i <= number; ++i)
        fact *= i;
    
    return fact;
}

int main(int, char**) {
    std::map<int, int> stdMap;
    std::map<int, int, std::less<int>, allocator::PoolAllocator<std::pair<const int, int>, 10>> poolMap;

    for (int i = 0; i < 10; ++i)
        stdMap.insert({ i, factorial(i) });

    for (int i = 0; i < 10; ++i)
        poolMap.insert({ i, factorial(i) });

    std::cout << "Default allocator:" << '\n';
    for (auto&& [key, value] : stdMap)
        std::cout << key << ' ' << value << '\n';

    std::cout << "\nPool allocator:" << '\n';
    for (auto&& [key, value] : poolMap)
        std::cout << key << ' ' << value << '\n';

    return 0;
}

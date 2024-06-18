#include <iostream>
#include <vector>
#include <map>
#include "../lib/Adapter.h"

int main() {
    std::vector<long> numbers = {1, 2, 3, 4, 5};
    auto removed = numbers | transform([](long i){return i * 2;});
    for (auto x : removed) {
        std::cout << x << " ";
    }
    std::cout << '\n';
    auto ni = removed | filter([](int i){return i % 2 == 0;});
    for (auto x : ni) {
        std::cout << x << " ";
    }
    std::cout << '\n';
    auto nig = take(ni, 3);
    for (auto x : nig) {
        std::cout << x << " ";
    }
    std::cout << '\n';
    auto nigg = drop(nig, 1);
    for (auto x : nigg) {
        std::cout << x << " ";
    }
    std::cout << '\n';
    auto nigge = reverse(nigg);
    for (auto x : nigge) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::map<int, std::string> mapa = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto key = keys(mapa);
    for (auto x : key) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    auto vals = values(mapa) | filter([](const std::string& i){return i == "one";});
    for (const auto& x : vals) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    return 0;
}


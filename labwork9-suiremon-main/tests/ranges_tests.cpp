#include <cstdint>
#include <lib/Adapter.h>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

class AdapterTestsSuite : public testing::Types<std::vector<int>, std::map<int, std::string>>{};

TEST(AdapterTestsSuite, TransformTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = numbers | transform([](int i){return i * 2;});
    std::vector<int> expected = {2, 4, 6, 8, 10};
    ASSERT_EQ(combined.size(), expected.size());
    for (int i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, FilterTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = numbers | filter([](int i){return i % 2 == 0;});
    std::vector<int> expected = {2, 4};
    ASSERT_EQ(combined.size(), expected.size());
    for (int i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}


TEST(AdapterTestsSuite, TakeTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = take(numbers, 3);
    std::vector<int> expected = {1, 2, 3};
    ASSERT_EQ(combined.size(), expected.size());
    for (int i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, DropTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = drop(numbers, 3);
    std::vector<int> expected = {4, 5};
    ASSERT_EQ(combined.size(), expected.size());
    for (int i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, ReverseTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = reverse(numbers);
    std::vector<int> expected = {5, 4, 3, 2, 1};
    ASSERT_EQ(combined.size(), expected.size());
    for (int i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, KeysTest) {
    std::map<int, std::string> mapa = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto combined = keys(mapa);
    std::vector<int> expected;
    for (auto& i: mapa) {
        expected.push_back(i.first);
    }
    ASSERT_EQ(combined.size(), expected.size());
    for (int i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, ValuesTest) {
    std::map<int, std::string> mapa = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto combined = values(mapa);
    std::vector<std::string> expected;
    for (auto& i: mapa) {
        expected.push_back(i.second);
    }
    ASSERT_EQ(combined.size(), expected.size());
    for (int i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedTransformAndFilterTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = numbers | transform([](int i){return i * 2;}) | filter([](int i){return i % 2 == 0;});
    std::vector<int> expected = {2, 4, 6, 8, 10};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedTakeAndTransformTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = take(numbers, 2) | transform([](int i){return i * 2;});
    std::vector<int> expected = {2, 4};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedTakeAndFilterTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = take(numbers, 3) | filter([](int i){return i % 3 == 0;});
    std::vector<int> expected = {3};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedKeysAndTransformTest) {
    std::map<int, std::string> myMap = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto combined = keys(myMap) | transform([](int i){return i * 5;});
    std::vector<int> expected = {5, 10, 15};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedValuesAndTransformTest) {
    std::map<int, std::string> myMap = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto combined = values(myMap) | transform([](const std::string& i){return i + 'a';});
    std::vector<std::string> expected = {"onea", "twoa", "threea"};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedFilterAndDropTest) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto combined = drop(numbers, 2) | filter([](int i){return i % 2 == 0;}) ;
    std::vector<int> expected = {4};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedPairDropAndFilterTest) {
    std::vector<std::pair<int, int>> numbers = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};
    auto combined = drop(numbers, 2) | filter([](std::pair<int, int> i){return i.second % 2 == 0;}) ;
    std::vector<std::pair<int, int>> expected = {{3, 4}};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

TEST(AdapterTestsSuite, CombinedPairTakeAndFilterTest) {
    std::vector<std::pair<int, int>> numbers = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};
    auto combined = take(numbers, 2) | transform([](std::pair<int, int> i){return std::make_pair(i.first * 15, i.second / 15);}) ;
    std::vector<std::pair<int, int>> expected = {{15, 0}, {30, 0}};
    ASSERT_EQ(combined.size(), expected.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        ASSERT_EQ(combined[i], expected[i]);
    }
}

#include "gtest/gtest.h"
#include <iostream>
#include <cmath>
#include "lib/SearchEngine/SearchEngine.h"
#include "lib/Searcher/Searcher.h"


TEST(SearchEngineTestSuite, StartTest) {
    char *tmp[2] = {"a", R"(C:\Users\suiremon\work\lngst_substr_wo_rep_chars\aaa)"};
    ASSERT_NO_THROW(SearchEngine se(2, tmp););
}

TEST(SearchEngineTestSuite, CreateTest) {
    std::ifstream file("data.txt");
    ASSERT_TRUE(file.is_open());
}

TEST(SearchEngineTestSuite, LengthTest) {
    std::ifstream file(R"(C:\Users\suiremon\labwork11-suiremon\cmake-build-debug\tests\data.txt)");
    file.seekg(-1, std::ios_base::end);
    while (file.peek() != '\n') {
        file.seekg(-1, std::ios_base::cur);
    }
    file.seekg(1, std::ios_base::cur);
    std::string tmp;
    getline(file, tmp);
    size_t fileSize = std::stoll(tmp);
    ASSERT_EQ(fileSize, 2);
}

TEST(SearcherTestSuite, CreateTest) {
    char *tmp[2] = {"a", R"(list AND vector)"};
    Searcher a(2, tmp);
    ASSERT_TRUE(!a.files.empty());
}

TEST(SearcherTestSuite, SizeTest) {
    char *tmp[2] = {"a", R"(list AND vector)"};
    Searcher a(2, tmp);
    ASSERT_EQ(a.files.size(), 2);
}

TEST(SearcherTestSuite, SecondSizeTest) {
    char *tmp[2] = {"a", R"(vector)"};
    Searcher a(2, tmp);
    ASSERT_EQ(a.files["vector"].size(), 2);
}

TEST(SearcherTestSuite, ParseTest) {
    char *tmp[2] = {"a", R"(vector)"};
    Searcher a(2, tmp);
    ASSERT_EQ(a.RPN[0].second, 1);
    ASSERT_EQ(a.RPN[1].second, 1);
}

TEST(SearcherTestSuite, PrintTest) {
    char *tmp[2] = {"a", R"(vector)"};
    Searcher a(2, tmp);
    ASSERT_NO_THROW(a.PrintResult());
}

TEST(ConfigSuite, ErrorTest) {
    char *tmp[2] = {"a", R"(vector AND list)"};
    ASSERT_NO_THROW(Config a(2, tmp););
}

TEST(ConfigSuite, CorrectRPNTest) {
    char *tmp[2] = {"a", R"((vector OR list) AND map)"};
    Config a(2, tmp);
    ASSERT_NO_THROW(auto b = a.getRPN(););
}

TEST(ConfigSuite, IncorrectRPNTest) {
    char *tmp[2] = {"a", R"((vector OR list)) AND map)"};
    ASSERT_DEATH(Config a(2, tmp);, "Incorrect input.");
}
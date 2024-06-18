#include <cstdint>
#include <lib/BST.h>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

class AdapterTestsSuite : public testing::Types<std::vector<int>, std::map<int, std::string>>{};

TEST(AdapterTestsSuite, CreateTest) {
    BST<int> bst;
    ASSERT_TRUE(bst.empty());
}

TEST(AdapterTestsSuite, InsertAndFindTest) {
    BST<int> bst;
    bst.insert(5);
    ASSERT_EQ(bst.size(), 1);
    ASSERT_EQ(bst.find(5)._node->_value, 5);
}

TEST(AdapterTestsSuite, BeginTest) {
    BST<int> bst;
    bst.insert(5);
    ASSERT_EQ(bst.begin()._node->_value, 5);
}

TEST(AdapterTestsSuite, EndTest) {
    BST<int> bst;
    bst.insert(5);
    ASSERT_TRUE(bst.end().is_end);
}

TEST(AdapterTestsSuite, RbeginTest) {
    BST<int> bst;
    bst.insert(5);
    ASSERT_EQ(bst.rbegin()._node->_value, 5);
}

TEST(AdapterTestsSuite, RendTest) {
    BST<int> bst;
    bst.insert(5);
    bst.insert(6);
    ASSERT_EQ(bst.rend()._node, nullptr);
}

TEST(AdapterTestsSuite, EmptyTest) {
    BST<int> bst;
    ASSERT_TRUE(bst.empty());
    bst.insert(5);
    ASSERT_FALSE(bst.empty());
}

TEST(AdapterTestsSuite, SizeTest) {
    BST<int> bst;
    ASSERT_EQ(bst.size(), 0);
    bst.insert(5);
    ASSERT_EQ(bst.size(), 1);
    bst.insert(6);
    ASSERT_EQ(bst.size(), 2);
    bst.insert(7);
    ASSERT_EQ(bst.size(), 3);
}

TEST(AdapterTestsSuite, MaxSizeTest) {
    BST<int> bst;
    ASSERT_EQ(bst.max_size(), std::numeric_limits<size_t>::max() / sizeof(int));
}

TEST(AdapterTestsSuite, ClearTest) {
    BST<int> bst;
    bst.insert(5);
    ASSERT_EQ(bst.size(), 1);
    bst.clear();
    ASSERT_TRUE(bst.empty());
}

TEST(AdapterTestsSuite, EraseTest) {
    BST<int> bst;
    bst.insert(5);
    bst.insert(6);
    bst.erase(5);
    ASSERT_EQ(bst.size(), 1);
}

TEST(AdapterTestsSuite, SwapTest) {
    BST<int> bst1;
    BST<int> bst2;
    bst1.insert(5);
    bst1.insert(6);
    bst1.insert(7);
    bst2.insert(8);
    bst2.insert(9);
    bst1.swap(bst2);
    ASSERT_EQ(bst1.size(), 2);
    ASSERT_EQ(bst2.size(), 3);
}

TEST(AdapterTestsSuite, ExtractTest) {
    BST<int> bst;
    bst.insert(5);
    bst.insert(6);
    auto a = bst.extract(6);
    ASSERT_EQ(bst.size(), 1);
    ASSERT_EQ(a->_value, 6);
}

TEST(AdapterTestsSuite, MergeTest) {
    BST<int> bst1;
    BST<int> bst2;
    bst1.insert(5);
    bst1.insert(6);
    bst1.insert(7);
    bst1.merge(bst2);
    ASSERT_EQ(bst1.size(), 3);
}

TEST(AdapterTestsSuite, CountTest) {
    BST<int> bst1;
    BST<int> bst2;
    bst1.insert(5);
    bst1.insert(6);
    bst1.insert(7);
    ASSERT_EQ(bst1.count(6), 1);
    ASSERT_EQ(bst1.count(8), 0);
}

TEST(AdapterTestsSuite, ContainsTest) {
    BST<int> bst1;
    bst1.insert(5);
    bst1.insert(6);
    bst1.insert(7);
    ASSERT_TRUE(bst1.contains(6));
    ASSERT_FALSE(bst1.count(8));
}

TEST(AdapterTestsSuite, InOrderTest) {
    BST<int> bst;
    bst.insert(5);
    bst.insert(7);
    bst.insert(4);
    auto it = bst.begin();
    ASSERT_EQ(it._node->_value, 4);
    it++;
    ASSERT_EQ(it._node->_value, 5);
    it++;
    ASSERT_EQ(it._node->_value, 7);
}

TEST(AdapterTestsSuite, PrintInOrderTest) {
    BST<int> bst;
    bst.insert(5);
    bst.insert(7);
    bst.insert(4);
    bst.insert(6);
    bst.printInOrderTraversal();
}

TEST(AdapterTestsSuite, PreOrderTest) {
    BST<int,  std::less<>, std::allocator<int>, preOrder> bst;
    bst.insert(5);
    bst.insert(7);
    bst.insert(4);
    auto it = bst.begin();
    ASSERT_EQ(it._node->_value, 5);
    it++;
    ASSERT_EQ(it._node->_value, 4);
    it++;
    ASSERT_EQ(it._node->_value, 7);
}

TEST(AdapterTestsSuite, PostOrderTest) {
    BST<int,  std::less<>, std::allocator<int>, postOrder> bst;
    bst.insert(5);
    bst.insert(7);
    bst.insert(4);
    auto it = bst.begin();
    ASSERT_EQ(it._node->_value, 4);
    it++;
    ASSERT_EQ(it._node->_value, 7);
    it++;
    ASSERT_EQ(it._node->_value, 5);
}
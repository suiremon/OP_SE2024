#include <cstdint>
#include <lib/Array.h>
#include <gtest/gtest.h>

class ConvertingTestsSuite : public testing::TestWithParam<uint32_t> {};

TEST(ConvertingTestsSuite, CreateTest) {
    Array3D arr(2, 2, 8);
    ASSERT_TRUE(arr.m_sizeX == 2);
    ASSERT_TRUE(arr.m_sizeY == 2);
    ASSERT_TRUE(arr.m_sizeZ == 8);
}

TEST(ConvertingTestsSuite, InputTest) {
    Array3D arr(14, 8, 8);
    for (size_t x = 0; x < arr.m_sizeX; ++x) {
        for (size_t y = 0; y < arr.m_sizeY; ++y) {
            for (size_t z = 0; z < arr.m_sizeZ; ++z) {
                arr[x][y][z] = x * y * z;
            }
        }
    }
    for (size_t x = 0; x < arr.m_sizeX; ++x) {
        for (size_t y = 0; y < arr.m_sizeY; ++y) {
            for (size_t z = 0; z < arr.m_sizeZ; ++z) {
                ASSERT_TRUE(arr[x][y][z].GetValue() == x * y * z);
            }
        }
    }
}

TEST(ConvertingTestsSuite, IndexTest) {
    Array3D arr(880, 555, 353);
    arr[0][0][0] = 111111;
    arr[12][34][56] = 78910;
    ASSERT_TRUE(arr[0][0][0].GetValue() == 111111);
    ASSERT_TRUE(arr[12][34][56].GetValue() == 78910);
}

TEST(ConvertingTestsSuite, AddTest) {
    Array3D arr1(15, 15, 15);
    Array3D arr2(15, 15, 15);
    arr1[1][2][3] = 45678;
    arr1[9][10][11] = 121314;
    ASSERT_EQ(arr1 + arr2, arr1);
}

TEST(ConvertingTestsSuite, BigAddTest) {
    Array3D arr1(13, 10, 12);
    Array3D arr2(13, 10, 12);
    for (size_t x = 0; x < arr1.m_sizeX; ++x) {
        for (size_t y = 0; y < arr1.m_sizeY; ++y) {
            for (size_t z = 0; z < arr1.m_sizeZ; ++z) {
                arr1[x][y][z] = x * y * z;
                arr2[x][y][z] = x + y + z;
            }
        }
    }
    Array3D result = arr1 + arr2;
    for (size_t x = 0; x < arr1.m_sizeX; ++x) {
        for (size_t y = 0; y < arr1.m_sizeY; ++y) {
            for (size_t z = 0; z < arr1.m_sizeZ; ++z) {
                ASSERT_EQ(result[x][y][z].GetValue(), x * y * z + x + y + z);
            }
        }
    }
}

TEST(ConvertingTestsSuite, DiffTest) {
    Array3D arr1(15, 15, 15);
    Array3D arr2(15, 15, 15);
    arr1[1][2][3] = 45678;
    arr1[9][10][11] = 121314;
    ASSERT_EQ(arr1 - arr2, arr1);
}

TEST(ConvertingTestsSuite, BigDiffTest) {
    Array3D arr1(13, 10, 12);
    Array3D arr2(13, 10, 12);
    for (size_t x = 0; x < arr1.m_sizeX; ++x) {
        for (size_t y = 0; y < arr1.m_sizeY; ++y) {
            for (size_t z = 0; z < arr1.m_sizeZ; ++z) {
                arr1[x][y][z] = 2 * x + 2 * y + 2 * z;
                arr2[x][y][z] = x + y + z;
            }
        }
    }
    Array3D result = arr1 - arr2;
    for (size_t x = 0; x < arr1.m_sizeX; ++x) {
        for (size_t y = 0; y < arr1.m_sizeY; ++y) {
            for (size_t z = 0; z < arr1.m_sizeZ; ++z) {
                ASSERT_EQ(result[x][y][z].GetValue(), 2 * x + 2 * y + 2 * z - (x + y + z));
            }
        }
    }
}

TEST(ConvertingTestsSuite, MultTest) {
    Array3D arr(23, 343, 654);
    arr[1][2][3] = 456;
    arr[9][10][11] = 789;
    arr = arr * 5;
    //ASSERT_EQ(arr[1][2][3].GetValue(), 456 * 5);
    //ASSERT_EQ(arr[9][10][11].GetValue(), 789 * 5);
}

//TEST(ConvertingTestsSuite, BigMultTest) {
//    Array3D arr(13, 10, 12);
//    Array3D arr2(13, 10, 12);
//    for (size_t x = 0; x < arr.m_sizeX; ++x) {
//        for (size_t y = 0; y < arr.m_sizeY; ++y) {
//            for (size_t z = 0; z < arr.m_sizeZ; ++z) {
//                arr[x][y][z] = x + y + z;
//            }
//        }
//    }
//    arr = arr * 2;
//    for (size_t x = 0; x < arr.m_sizeX; ++x) {
//        for (size_t y = 0; y < arr.m_sizeY; ++y) {
//            for (size_t z = 0; z < arr.m_sizeZ; ++z) {
//                ASSERT_EQ(arr[x][y][z].GetValue(), 2 * (x + y + z));
//            }
//        }
//    }
//}
//

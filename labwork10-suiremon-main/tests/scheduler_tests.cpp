#include "gtest/gtest.h"
#include <iostream>
#include <cmath>
#include "lib/TTaskScheduler.h"


TEST(SchedulerTestSuite, DefaultExecuteAllTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](float a, float c) { return (17 * a * c); }, 1, 2);
    scheduler.executeAll();
    ASSERT_EQ(scheduler.getResult<float>(id), 34);
}

TEST(SchedulerTestSuite, DefaultTaskTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](float a, float c) { return (a * c / 5); }, 47, 5);
    ASSERT_EQ(scheduler.getResult<float>(id), 47);
}

TEST(SchedulerTestSuite, LongChainTest) {
    TTaskScheduler scheduler;
    float a = 2;
    float b = -6;
    float c = 4;
    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id4));
    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id6), 1);
}

TEST(SchedulerTestSuite, VectorTaskOneArgumentTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](int a) { return std::vector<int>(3, a); }, 239);
    ASSERT_EQ(scheduler.getResult<std::vector<int>>(id), std::vector<int>({239, 239, 239}));
}

TEST(SchedulerTestSuite, DefaultFutureResultTest) {
    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](float a, float c) { return (a * c / 5); }, 3, 5);
    auto id2 = scheduler.add([](float a, float b) { return b * b + a; }, 2, scheduler.getFutureResult<float>(id1));
    ASSERT_EQ(scheduler.getResult<float>(id2), 11);
}

TEST(SchedulerTestSuite, DefaultTaskNoArgumentTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([]() { return 30 + 9 + 200; });
    ASSERT_EQ(scheduler.getResult<int>(id), 239);
}

TEST(SchedulerTestSuite, StringTaskNoArgumentTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([]() {std::string a = "amo"; std::string b = "gus"; return a + b;});
    ASSERT_EQ(scheduler.getResult<std::string>(id), "amogus");
}
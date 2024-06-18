#include <lib/TTaskScheduler.h>
#include <iostream>
#include <cmath>
int main() {
    float a = 2;
    float b = -6;
    float c = 4;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id4));
    auto id7 = scheduler.add([](float a){return a*a;}, a);

    scheduler.executeAll();

    std::cout << "x1 = " << scheduler.getResult<float>(id5) << std::endl;
    std::cout << "x2 = " << scheduler.getResult<float>(id6) << std::endl;
    std::cout << "x2 = " << scheduler.getResult<float>(id7) << std::endl;

    return 0;
}

#include <iostream>
#include "lib/Array.h"
using namespace std;


int main() {
    Array3D arr = Array3D::make_array(234, 12, 41253);
    Array3D arr2 = Array3D::make_array(2, 2, 2);
    cout << sizeof(arr) << '\n';
    return 0;
}
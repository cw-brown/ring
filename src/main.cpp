#include <iostream>
#include <complex>
#include <vector>

#include "ring.hpp"



int main(){
    static_assert(std::ranges::input_range<std::initializer_list<float>>);

    std::ring<int> b(6);
    std::ring<int> c = {1,2, 3, 4, 30};
    std::ring<int> d;
    d.assign(c.begin(), c.end());
    std::cout<<d;




    return 0;
}
#include <iostream>
#include <complex>
#include <vector>
#include <cassert>

#include "ring.hpp"



int main(){
    std::ring<float> a = {1,2,3,4};
    std::ring<float> b = {1,2,3,4};
    a.append_range(b);

    std::cout<<a;




    return 0;
}
#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    static_assert(std::ranges::sized_range<std::ring<float>>);
    std::ring<float> a = {1,2,3,4};
    a.pop_back();
    a.pop_back();
    a.push_back(20);
    a.push_back(30.3f);
    std::ring<float> b(std::move(a), std::allocator<int>());



    // std::cout<<b<<std::endl;

    return 0;
}
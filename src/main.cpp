#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    std::ring<float> a{1,2,3,4};
    std::ring<float> b = {8,2,1,2,4,5,1};
    a = {2,1,2};
    std::cout<<a;



    // std::ring<float> b(3, 1);
    // std::cout<<b<<std::endl;

    // std::ring<float> c(a.begin(), a.end());
    // std::cout<<c<<std::endl;

    std::cout<<std::endl<<"Returned without error!"<<std::endl;
    return 0;
}
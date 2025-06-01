#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    [[maybe_unused]] std::vector<float> t = {9,8,7,6,5,4,3};
    std::ring<float> a{1,2,3,4};

    std::cout<<*a.data()<<std::endl;



    // std::ring<float> b(3, 1);
    // std::cout<<b<<std::endl;

    // std::ring<float> c(a.begin(), a.end());
    // std::cout<<c<<std::endl;

    std::cout<<std::endl<<"Returned without error."<<std::endl;
    return 0;
}
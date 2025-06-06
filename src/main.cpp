#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    std::vector<float> a = {9,2,1,5,6,20,1,0};
    std::ring<float> b({0,19,1,0,21,12});

    std::cout<<b<<std::endl;
    // for(size_t i = 0; i < 5; i++){
    //     std::cout<<b.data()[i]<<",";
    // }
    b.assign(a.begin(), a.end());

    std::cout<<b;


    return 0;
}
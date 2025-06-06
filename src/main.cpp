#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    std::vector<float> a = {9,2,1,5,6,20,1,0};
    std::ring<float> b(10);

    for(size_t i = 0; i < 8; i++){
        b.push_back(i);
    }
    std::cout<<b.max_size()<<std::endl;
    b.shrink_to_fit();
    std::cout<<b<<std::endl;
    b.clear();
    std::cout<<b<<std::endl;

    return 0;
}
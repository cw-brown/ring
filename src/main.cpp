#include <iostream>
#include <complex>
#include <vector>

#include "ring.hpp"



int main(){
    std::ring<int> b = {1,2,3,4,5};
    std::cout<<b<<std::endl;
    b.resize(2);
    std::cout<<b<<std::endl;
    std::cout<<b.max_size()<<", "<<b.size();




    return 0;
}
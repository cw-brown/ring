#include <iostream>
#include <complex>
#include <vector>

#include "ring.hpp"



int main(){
    std::ring<int> b(6);
    std::vector<int> c = {1,2, 3, 4, 30};

    b.push_back(20);
    b.push_back(20);
    b.push_back(20);

    std::cout<<b<<std::endl;

    b.append_range(c);
    b.push_back(15);
    b.emplace_back(12);
    
    std::cout<<b<<std::endl;


    return 0;
}
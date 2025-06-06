#include <iostream>
#include <complex>
#include <vector>

#include "ring.hpp"



int main(){
    std::ring<int> b(6);
    b.append_range(std::initializer_list<int>({1,2,3}));
    std::cout<<"Before Resizing: "<<b<<std::endl;
    b.resize(5);
    std::cout<<"After Resizing (up to 5): "<<b<<std::endl;
    b.resize(2);
    std::cout<<"After Resizing (down to 2): "<<b<<std::endl;
    b.resize(6, 12);
    std::cout<<"After Resizing (up to 6 with 12s): "<<b<<std::endl;
    b.push_back(500);
    std::cout<<"Pushed a value: "<<b<<std::endl;




    return 0;
}
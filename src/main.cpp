#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    [[maybe_unused]] std::vector<float> t = {9,8,7,6,5,4,3};
    std::ring<float> a{1,2,3,4,5};

    auto begin = a.begin();
    auto end = a.end();
    for(; begin != end; ++begin){
        std::cout<<*begin<<std::endl;
    }



    std::cout<<std::endl<<"Returned without error."<<std::endl;
    return 0;
}
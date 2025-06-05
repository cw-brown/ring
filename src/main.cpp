#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    std::ring<float> a(47);

    a = {1,5,2,1,6};
    std::vector<float> vec(std::from_range, a);
    for(auto&& v:vec)
        std::cout<<v<<",";
    return 0;
}
#include <iostream>
#include <vector>

#include "ring.hpp"

struct President
{
    std::string name;
    std::string country;
    int year;
 
    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year)
    {
        std::cout << "I am being constructed.\n";
    }

    President(const President& other): name(other.name), country(other.country), year(other.year){};
 
    President(President&& other)
        : name(std::move(other.name)), country(std::move(other.country)), year(other.year)
    {
        std::cout << "I am being moved.\n";
    }
 
    President& operator=(const President& other) = default;
};


int main(){
    std::ring<President> a(3);
    a.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));
    a.emplace_back("Nelson Mandela", "South Africa", 1994);
    for(auto&& v : a){
        std::cout<<v.name<<v.country<<v.year<<std::endl;
    }

    
    


    return 0;
}
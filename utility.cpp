#include "utility.h"

std::string add_number_separator(int fame_number){
    std::string fame = std::to_string(fame_number);
    auto old_size = fame.size();
    fame.resize(fame.size() + (fame.size()-1)/3);
    std::vector<char> stack;
    stack.reserve(5);
    for(int i = 1; i < fame.size() - 1; i++){
        if(old_size != i && (old_size - i) % 3 == 0){
            stack.push_back(fame[i]);
            fame[i] = ',';
        }else if(!stack.empty()){
            if(fame[i] == '\0'){
                fame[i] = stack.back();
                stack.pop_back();
            }else{
                std::swap(fame[i], stack.back());
            }
        }
    }
    return fame;
}
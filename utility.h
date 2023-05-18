#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

class Timer{
    std::chrono::_V2::system_clock::time_point start{std::chrono::high_resolution_clock::now()};
    std::string_view msg;
public:
    Timer(std::string_view msg) : msg{msg} {}
    ~Timer(){
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << msg << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
    }
};

std::string add_number_separator(int fame_number);
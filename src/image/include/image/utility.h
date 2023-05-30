#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <cpr/cpr.h>

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

class BatchHttpRequester {
    cpr::MultiPerform batch_requester;
    std::vector<cpr::Response> responses;
    int next_index{0};
public:
    /// @return A session id used to retrieve response
    int add_request(std::string_view url);
    void send_requests();
    cpr::Response& get_response(int session_id);
    void reset();
};
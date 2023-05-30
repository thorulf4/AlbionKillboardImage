#pragma once
#include <image/albion_item.h>
#include <image/utility.h>

#include <string>
#include <sstream>
#include <vector>
#include <array>

#include <cpr/cpr.h>


struct Item{
    int quality;
    int count;
    int64_t estimated_price{0};
    int found_quality{std::numeric_limits<int>::min()};
};

class PriceCalculator{
    std::map<std::string, Item> items;
    std::vector<std::stringstream> urls;
    std::vector<int> session_ids;

public:
    PriceCalculator();
    void add_item(std::string item_id, int quality, int count);
    void queue_requests(BatchHttpRequester& http_performer);
    int64_t get_price(BatchHttpRequester& http_performer);

private:
    void adjust_price(Item& item, int64_t price, int quality);
    void flush_url();
};
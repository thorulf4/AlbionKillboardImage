#pragma once
#include "albion_item.h"

#include <string>
#include <sstream>
#include <vector>
#include <array>

#include <cpr/cpr.h>


struct Item{
    int quality;
    int count;
};

class PriceCalculator{
    std::map<std::string, Item> items;
    std::vector<std::stringstream> urls;
    std::vector<int> session_ids;
    std::array<bool, 5> qualities;

public:
    PriceCalculator();
    void add_item(std::string item_id, int quality, int count);
    void queue_requests(ImageCache& http_performer);
    int64_t get_price(ImageCache& http_performer);

private:
    void flush_url();
};
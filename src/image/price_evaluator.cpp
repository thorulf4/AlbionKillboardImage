#include <image/price_evaluator.h>
#include <ranges>
#include <algorithm>
#include <memory>
#include <ranges>
#include <iostream>


namespace views = std::ranges::views;
namespace ranges = std::ranges;

PriceCalculator::PriceCalculator(){
    flush_url();
}

void PriceCalculator::flush_url(){
    urls.emplace_back();
    urls.back() << "https://west.albion-online-data.com/api/v2/stats/prices/"; // Consider removing black market from fetch
}

// Check what happens if items appear twice
void PriceCalculator::add_item(std::string item_id, int quality, int count){
    auto& url_builder = urls.back();
    // 4050 = 4kb - padding
    if(url_builder.view().size() + item_id.size() + 1 > 4050){
        flush_url();
    }
    
    url_builder << ',' << item_id;

    items.insert({std::move(item_id), Item{quality, count}});
}

void PriceCalculator::queue_requests(BatchHttpRequester& http_performer){
    ranges::for_each(urls, [](const auto& x){ std::cout << x.view() << '\n';});
    ranges::copy(urls | views::transform([&](const auto& url){
        return http_performer.add_request(url.view());
    }), std::back_inserter(session_ids));
}

void PriceCalculator::adjust_price(Item& item, int64_t price, int quality){
    if(item.found_quality < item.quality){
        if(quality >= item.found_quality && quality <= item.quality || item.quality == 0){
            item.found_quality = quality;
            item.estimated_price = std::max(item.estimated_price, price);
        }
    }else if(item.found_quality > item.quality){
        if(quality <= item.found_quality){
            item.found_quality = quality;
            item.estimated_price = price;
        }
    }else if(quality == item.found_quality){
        item.estimated_price = std::max(item.estimated_price, price);
    }
}

int64_t PriceCalculator::get_price(BatchHttpRequester& http_performer){
    for(int session_id : session_ids){
        const auto& response = http_performer.get_response(session_id);
        auto json_array = nlohmann::json::parse(response.text);
        for(auto item : json_array){
            auto price = item["sell_price_min"].get<int64_t>();
            if(price != 0){
                auto target = items.find(item["item_id"].get<std::string>());
                std::string name = target->first;
                if(target != items.end()){
                    adjust_price(target->second, price, item["quality"].get<int>());
                }
            }
        }
    }

    int64_t price_sum = 0;
    for(auto& item : items){
        price_sum += item.second.estimated_price * item.second.count;
    }

    return price_sum;
}
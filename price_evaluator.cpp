#include "price_evaluator.h"
#include <ranges>
#include <algorithm>
#include <memory>
#include <ranges>


namespace views = std::ranges::views;
namespace ranges = std::ranges;

PriceCalculator::PriceCalculator(){
    flush_url();
}

void PriceCalculator::flush_url(){
    ranges::fill(qualities, false);
    urls.emplace_back();
    urls.back() << "https://west.albion-online-data.com/api/v2/stats/prices/";
}

// Check what happens if items appear twice
void PriceCalculator::add_item(std::string item_id, int quality, int count){
    auto& url_builder = urls.back();
    // 4050 is 4kb - padding for remaining query
    if(url_builder.view().size() + item_id.size() + 1 > 4050){
        url_builder << "?locations=Caerleon&qualities=";
        for(int i = 0; i < qualities.size(); i++){
            if(qualities[i])
                url_builder << ',' << i;
        }

        flush_url();
    }
    
    url_builder << ',' << item_id;
    qualities[quality] = true;

    items.insert({std::move(item_id), Item{quality, count}});
}

std::shared_ptr<cpr::Session> create_session(const std::stringstream& url){
    auto session = std::make_shared<cpr::Session>();
    session->SetUrl(cpr::Url{url.view()});
    return session;
}

void PriceCalculator::queue_requests(ImageCache& http_performer){
    ranges::copy(urls | views::transform([&](const auto& url){
        return http_performer.register_session(create_session(url));
    }), std::back_inserter(session_ids));
}

int64_t PriceCalculator::get_price(ImageCache& http_performer){
    int64_t price_sum = 0;
    for(int session_id : session_ids){
        auto json_array = nlohmann::json{http_performer.get_response(session_id).text};
        for(auto item : json_array){
            auto target = items.find(item["item_id"].get<std::string>());
            if(target != items.end() && item["quality"].get<int>() == target->second.quality){
                price_sum += item["sell_price_min"].get<int>() * target->second.count;
                items.erase(target);
            }
        }
    }
    return price_sum;
}
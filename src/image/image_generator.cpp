#include <iostream>
#include <string_view>
#include <sstream>
#include <ranges>
#include <string>
#include <filesystem>
#include <limits>
#include <chrono>
#include <iomanip>
#include <locale>
#include <variant>
#include <memory>

#include <nlohmann/json.hpp>
#include <image/image_generator.h>
#include <image/albion_item.h>
#include <image/utility.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::views;
using json = nlohmann::json;

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

void put(const cv::Mat& src, cv::Mat& dest, cv::Point position){
    // Consider adding better alpha blending https://en.wikipedia.org/wiki/Alpha_compositing
    for(int y = 0; y < src.rows; y++){
        const cv::Vec4b* src_row = src.ptr<cv::Vec4b>(y);
        cv::Vec4b* dest_row = dest.ptr<cv::Vec4b>(y + position.y) + position.x;
        for(int x = 0; x < src.cols; x++){
            float alpha = src_row->operator[](3) / 255.0;
            *dest_row = *src_row * alpha + (1.0-alpha) * *dest_row;
            src_row++;
            dest_row++;
        }
    }
}

void put(const cv::Mat& src, cv::Mat& dest, int x, int y){
    put(src, dest, {x,y});
}

void ItemPromise::apply(cv::Mat target){
    cv::Mat src = std::visit(overloaded{
        [&](cv::Mat mat){ return mat; },
        [&](Fetch& fetch){ 
            auto mat = fetch.get_response(); 
            images.save_image(fetch.type, fetch.quality, mat);
            return mat;
        }
    }, image);

    auto item_resized = cv::Mat(121, 121, CV_8UC4);
    cv::resize(src, item_resized, cv::Size(121, 121));

    put(item_resized, target, position);
}

const cv::Point bag_position(27, 33);
const cv::Point head_position(180, 46);
const cv::Point cape_position(335, 33);
const cv::Point weapon_position(52, 156);
const cv::Point armor_position(182, 156);
const cv::Point offhand_position(310, 156);
const cv::Point shoes_position(182, 270);
const cv::Point mount_position(182, 382);
const cv::Point consumable_position(335, 284);
const cv::Point potion_position(27, 284);

void ImageGenerator::put_item(std::vector<ItemPromise>& items, json item, const cv::Point& position){
    if(!item.is_null()){
        std::string item_id = item["Type"].get<std::string>();
        int quality = item["Quality"].get<int>();

        auto img = image_cache->get_image(item_id, quality, requester);
        if(price_getter) // Dirty hack reconsider this
            price_getter->add_item(std::move(item_id), quality, item["Count"].get<int>());

        items.push_back(ItemPromise{*image_cache, position, std::move(img)});
    }
}

void ImageGenerator::put_items(std::vector<ItemPromise>& items, json equipment){    
    put_item(items, equipment["Head"], head_position);
    put_item(items, equipment["MainHand"], weapon_position);
    put_item(items, equipment["Mount"], mount_position);
    put_item(items, equipment["OffHand"], offhand_position);
    put_item(items, equipment["Potion"], potion_position);
    put_item(items, equipment["Shoes"], shoes_position);
    put_item(items, equipment["Food"], consumable_position);
    put_item(items, equipment["Cape"], cape_position);
    put_item(items, equipment["Bag"], bag_position);
    put_item(items, equipment["Armor"], armor_position);
}

void put_text(cv::Mat image, const std::string& str, float scale, cv::Point position){
    int baseline;
    auto size = cv::getTextSize(str, cv::FONT_HERSHEY_DUPLEX, scale, 1, &baseline);
    cv::putText(image, str, cv::Point{position.x-size.width/2, position.y}, cv::FONT_HERSHEY_DUPLEX, scale, cv::Scalar{255,255,255,255}, 1, cv::LINE_AA);
}

cv::Mat create_template(){
    cv::Mat base_image = cv::Mat{520, 1000, CV_8UC4, cv::Scalar{255,255,255,0}};

    auto gear = cv::imread("gear.png", cv::IMREAD_UNCHANGED); // 480x520
    put_text(base_image, "Killer", 1, {240, 22});
    put_text(base_image, "Victim", 1, {1000-gear.cols + 240, 22});

    cv::Mat fame_icon = cv::imread("fame.png", CV_8UC4);
    put(fame_icon, base_image, 500-fame_icon.cols/2, 100);

    cv::Mat silver_icon = cv::imread("silver.png", CV_8UC4);
    put(silver_icon, base_image, 500-silver_icon.cols/2, 200);

    put(gear, base_image, 0, 0);
    put(gear, base_image, 1000 - gear.cols, 0);
    return base_image;
}

ImageGenerator::ImageGenerator(std::string_view path_to_cache) : base_image{create_template()}, gear_width{480}{
    image_cache = std::make_unique<ImageCache>(path_to_cache);
}


ImageGenerator::~ImageGenerator() = default;

cv::Mat ImageGenerator::generate(unsigned long long event_id){
    price_getter = nullptr;
    auto event = read_killboard(event_id);

    std::vector<ItemPromise> killer_items;
    put_items(killer_items, event.killer["Equipment"]);

    price_getter = std::make_unique<PriceCalculator>(); // Register PriceCalculator for victim
    std::vector<ItemPromise> victim_items;
    put_items(victim_items, event.victim["Equipment"]);

    price_getter->queue_requests(requester);
    requester.send_requests();

    Timer timer{"Generate image: "};

    cv::Mat image;
    base_image.copyTo(image);

    std::string fame = add_number_separator(event.fame);
    put_text(image, fame, 0.7, {500, 175});

    put_text(image, std::to_string(event.killer["AverageItemPower"].get<int>()) + "IP" , 0.75, {390, 440});
    put_text(image, std::to_string(event.victim["AverageItemPower"].get<int>()) + "IP", 0.75, {610, 440});
    put_text(image, event.killer["Name"].get<std::string>(), 0.7, {240, 45});
    put_text(image, event.victim["Name"].get<std::string>(), 0.7, {1000-gear_width + 240, 45});

    int earnings = price_getter->get_price(requester);
    std::string earnings_str = add_number_separator(earnings);
    put_text(image, earnings_str, 0.7, {500, 275});

    for(auto& item : killer_items)
        item.apply(image(cv::Rect{0,0,480,520}));
    for(auto& item : victim_items)
        item.apply(image(cv::Rect{1000-gear_width,0,480,520}));

    cv::imwrite("tmp_image.png", image);

    //Cleanup
    requester.reset();

    return image;
}
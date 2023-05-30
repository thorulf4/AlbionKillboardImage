#pragma once
#include <memory>
#include <string_view>
#include <vector>
#include <variant>
#include <opencv2/core.hpp>
#include <image/albion_item.h>
#include <image/price_evaluator.h>
#include <image/utility.h>
#include <nlohmann/json.hpp>

class ImageCache;
using ImageCachePtr = std::unique_ptr<ImageCache>;

struct ItemPromise{
    ImageCache& images;
    cv::Point position;
    std::variant<cv::Mat, Fetch> image;

    void apply(cv::Mat target);
};

class ImageGenerator{
    ImageCachePtr image_cache;
    std::unique_ptr<PriceCalculator> price_getter;
    BatchHttpRequester requester;
    cv::Mat base_image;
    int gear_width;
public:
    ImageGenerator(std::string_view path_to_cache);
    ~ImageGenerator();
    cv::Mat generate(unsigned long long event_id);

private:
    void put_items(std::vector<ItemPromise>& items, nlohmann::json equipment);
    void put_item(std::vector<ItemPromise>& items, nlohmann::json item, const cv::Point& position); 
};

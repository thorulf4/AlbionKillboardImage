#pragma once
#include <iostream>
#include <string_view>
#include <string>
#include <sstream>
#include <variant>

#include "utility.h"
#include "json.hpp" // nlohmann json

#include <cpr/cpr.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

struct KillEvent{
    nlohmann::json killer;
    nlohmann::json victim;
    uint32_t fame;
};

KillEvent read_killboard(int64_t url);

class ImageCache;
struct Fetch{
    std::string type;
    int quality;
    int session_id;
    Fetch(std::string type, int quality, int session_id) : 
        type{std::move(type)}, 
        quality{quality},
        session_id{session_id}
    {}

    cv::Mat get_response(ImageCache& image_cache);
};

class ImageCache{
    std::unordered_map<std::pair<std::string, int>, cv::Mat, pair_hash> items;
    cpr::MultiPerform http_performer;
    std::vector<cpr::Response> responses;
    int next_index = 0;

public:
    ImageCache(std::string_view directory);

    std::variant<cv::Mat, Fetch> get_image(const std::string& image_id, int quality);

    void perform_sessions();

    int register_session(std::shared_ptr<cpr::Session> session);

    cpr::Response get_response(int session_id);

    void save_image(const std::string& image_id, int quality, cv::Mat mat);
};
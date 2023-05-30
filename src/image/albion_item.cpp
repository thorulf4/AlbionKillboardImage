#include <image/albion_item.h>


KillEvent read_killboard(int64_t url){
    std::stringstream os;
    os << "https://gameinfo.albiononline.com/api/gameinfo/events/" << url;
    cpr::Response response = cpr::Get(cpr::Url{os.view()});
    auto json = nlohmann::json::parse(response.text);
    return {json["Killer"], json["Victim"], json["TotalVictimKillFame"]};
}

std::stringstream get_image_url(const std::string& image_id, int quality) {
    auto os = std::stringstream{};
    os << "https://render.albiononline.com/v1/item/" << image_id << ".png?count=1&quality=" << quality;
    return os;
}

cv::Mat decode_response(const std::string& image_id, int quality, const cpr::Response& response){
    cv::Mat image = cv::imdecode({response.text.c_str(), static_cast<int>(response.text.size())}, cv::IMREAD_UNCHANGED);

    auto os = std::stringstream{};
    os << "items/" << quality << image_id << ".png";
    cv::imwrite(os.str(), image);
    return image;
}

ImageCache::ImageCache(std::string_view directory){
    for (const auto & entry : fs::directory_iterator(directory)){
        auto name = entry.path().filename().string();
        std::cout << "Preload " << entry.path().string() << '\n';
        int quality = int(name[0]) - 48; // Get first digit
        std::string item_name = name.substr(1, name.find_last_of('.')-1);
        items.insert({{item_name, quality}, cv::imread(entry.path().string(), cv::IMREAD_UNCHANGED)});
    }
}

std::variant<cv::Mat, Fetch> ImageCache::get_image(const std::string& image_id, int quality, BatchHttpRequester& requester){
    auto it = items.find({image_id, quality});
    if(it == items.end()){
        auto url = get_image_url(image_id, quality);
        int id = requester.add_request(url.view());
        return Fetch{image_id, quality, id, requester};
    }
    return it->second;
}

void ImageCache::save_image(const std::string& image_id, int quality, cv::Mat mat){
    items.insert_or_assign({image_id, quality}, mat);
}

cv::Mat Fetch::get_response(){
    return decode_response(type, quality, requester.get_response(session_id));
}
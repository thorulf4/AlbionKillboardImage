#include "albion_item.h"


KillEvent read_killboard(int64_t url){
    std::stringstream os;
    os << "https://gameinfo.albiononline.com/api/gameinfo/events/" << url;
    cpr::Response response = cpr::Get(cpr::Url{os.view()});
    auto json = nlohmann::json::parse(response.text);
    return {json["Killer"], json["Victim"], json["TotalVictimKillFame"]};
}

std::shared_ptr<cpr::Session> fetch_async(const std::string& image_id, int quality) {
    auto os = std::stringstream{};
    os << "https://render.albiononline.com/v1/item/" << image_id << ".png?count=1&quality=" << quality;
    std::cout << "Fetching " << os.view() << '\n';
    auto session = std::make_shared<cpr::Session>();
    session->SetUrl(cpr::Url{os.view()});
    return session;
}

cv::Mat decode_response(const std::string& image_id, int quality, cpr::Response response){
    cv::Mat image = cv::imdecode({response.text.c_str(), response.text.size()}, cv::IMREAD_UNCHANGED);

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

std::variant<cv::Mat, Fetch> ImageCache::get_image(const std::string& image_id, int quality){
    auto it = items.find({image_id, quality});
    if(it == items.end()){
        int id = register_session(fetch_async(image_id, quality));
        return Fetch{image_id, quality, id};
    }
    return it->second;
}

void ImageCache::perform_sessions(){
    // Applies get on all sessions
    Timer timer{"Fetching timer "};
    responses = http_performer.Get();
}

int ImageCache::register_session(std::shared_ptr<cpr::Session> session){
    http_performer.AddSession(session);
    return next_index++;
}

cpr::Response ImageCache::get_response(int session_id){
    return std::move(responses[session_id]);
}

void ImageCache::save_image(const std::string& image_id, int quality, cv::Mat mat){
    items.insert_or_assign({image_id, quality}, mat);
}

cv::Mat Fetch::get_response(ImageCache& image_cache){
    cv::Mat mat = decode_response(type, quality, image_cache.get_response(session_id));
    image_cache.save_image(type, quality, mat);
    return mat;
}
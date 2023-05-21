#include "utility.h"

std::string add_number_separator(int fame_number){
    std::string fame = std::to_string(fame_number); //Test dis
    std::string result{};
    int total_size = fame.size() + (fame.size()-1)/3;
    result.resize(total_size);

    auto it = fame.begin();
    result[0] = *it++;
    for(int i = 1; i < total_size; i++){
        if((total_size - i) % 4 == 0){
            result[i] = ',';
            result[++i] = *it++;
        }else{
            result[i] = *it++;
        }
    }

    return result;
}

int BatchHttpRequester::add_request(std::string_view url){
    auto session = std::make_shared<cpr::Session>();
    session->SetUrl(url);
    batch_requester.AddSession(session);
    return next_index++;
}
void BatchHttpRequester::send_requests(){
    Timer timer{"Fetching timer "};
    responses = batch_requester.Get();
}
cpr::Response& BatchHttpRequester::get_response(int session_id){
    return responses[session_id];
}

void BatchHttpRequester::reset(){
    next_index = 0;
    batch_requester = cpr::MultiPerform{};
}
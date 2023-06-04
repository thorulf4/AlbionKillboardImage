#include <cstdlib>
#include <iostream>
#include <ranges>
#include <concepts>
#include <istream>

#include <image/image_generator.h>
#include <dpp/dpp.h>

namespace views = std::ranges::views;

template<typename T, typename Item>
concept range = std::ranges::range<T> && std::same_as<Item, std::ranges::range_value_t<T>>;

void print_help(){
    std::cout << "A discord bot token must be provided.\n"
              << "  -t, --token {discord token}\n"
              << "  -f, --token-file {path to file}\n";
}

std::string handle_arguments(int argc, const char* argv[]){
    for(int i = 1; i < argc; i++){
        if(i + 2 > argc)
            break;

        if(std::strncmp("-f", argv[i], 2) == 0 || std::strncmp("--token-file", argv[i], 12) == 0){
            std::string token;
            std::ifstream file(argv[i+1]);
            file >> token;
            return token;
        } else if(std::strncmp("-t", argv[i], 2) == 0 || std::strncmp("--token", argv[i], 7) == 0 ){
            return argv[i+1];
        }
    }
    
    print_help();
    std::exit(0);
}
 
int main(int argc, const char* argv[]) {
    std::string bot_token = handle_arguments(argc, argv);

    dpp::cluster bot(bot_token, dpp::i_default_intents | dpp::i_message_content);
    // Save item images in items/ folder
    ImageGenerator img_gen{"items"};

    bot.on_log(dpp::utility ::cout_logger());
 
    bot.on_slashcommand([&](const dpp::slashcommand_t& event) {
         if (event.command.get_command_name() == "kill-image") {
            try{
                std::string killboard_link = std::get<std::string>(event.get_parameter("killboard-link"));
                if(killboard_link.starts_with("https://albiononline.com/killboard/kill/")){
                    event.reply(
                        dpp::interaction_response_type::ir_deferred_channel_message_with_source,
                        dpp::message()
                    );
                    auto event_id_str = killboard_link.substr(40);
                    auto event_id = std::stoull(event_id_str.c_str());
                    img_gen.generate(event_id);
                    auto msg2 = dpp::message{event.command.get_issuing_user().get_mention() + " has linked " + killboard_link};
                    msg2.add_file(event_id_str + "_summary.png", dpp::utility::read_file("tmp_image.png"));
                    event.edit_original_response(msg2);
                }
            }catch(std::exception e){
                std::cerr << e.what() << '\n';
            }
        }
    });
 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {

            auto command = dpp::slashcommand("kill-image", "Provide a link to a killboard to get a summary", bot.me.id);
            command.add_option(dpp::command_option(dpp::co_string, "killboard-link", "Kilboard link e.g: https://albiononline.com/killboard/kill/783367059", true));
            bot.global_command_create(command);
        }
    });
 
    bot.start(dpp::st_wait);
}
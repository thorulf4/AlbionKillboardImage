#include <cstdlib>
#include <iostream>

#include "image_generator.h"
#include <dpp/dpp.h>
 
const std::string BOT_TOKEN = "";
 
int main() {
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
    // Save item images in items/ folder
    ImageGenerator img_gen{"items"};

    bot.on_log(dpp::utility::cout_logger());
 
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
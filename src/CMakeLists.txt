include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(image)
if(${BUILD_DISCORD_BOT})
    add_subdirectory(bot)
endif()
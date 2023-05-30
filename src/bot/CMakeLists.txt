FetchContent_Declare(
    dpp_fc
    GIT_REPOSITORY https://github.com/brainboxdotcc/DPP
    GIT_TAG v10.0.23
    GIT_SHALLOW TRUE # get only the last commit version
    GIT_PROGRESS TRUE # show progress of download
    #FIND_PACKAGE_ARGS NAMES dpp
)
FetchContent_MakeAvailable(dpp_fc)

add_executable(bot bot.cpp)
target_link_libraries(bot PRIVATE dpp image_gen)
cmake_minimum_required(VERSION 3.22.1)
option(BUILD_DISCORD_BOT OFF)


set(CMAKE_CXX_STANDARD 23)
project(image_gen CXX)

include(cmake/opencv.cmake)
include(cmake/cpr.cmake)
include(cmake/dpp.cmake)

# add_compile_options(-fsanitize=address -fsanitize=undefined)
# add_link_options(-fsanitize=address -fsanitize=undefined)

add_subdirectory(src)

if(${BUILD_DISCORD_BOT})
    install(FILES resources/fame.png resources/gear.png resources/silver.png DESTINATION .)
endif()
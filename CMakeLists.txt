cmake_minimum_required(VERSION 3.22.1)


set(CMAKE_CXX_STANDARD 23)
project(image_gen CXX)

include(FetchContent)
FetchContent_Declare(
    opencv
    GIT_REPOSITORY https://github.com/opencv/opencv
    GIT_TAG 4.7.0
    GIT_SHALLOW TRUE # get only the last commit version
    GIT_PROGRESS TRUE # show progress of download
)
FetchContent_MakeAvailable(OpenCv)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.10.2 # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
                         GIT_SHALLOW TRUE # get only the last commit version
                         GIT_PROGRESS TRUE) # show progress of download
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    dpp_fc
    GIT_REPOSITORY https://github.com/brainboxdotcc/DPP
    GIT_TAG v10.0.23
    GIT_SHALLOW TRUE # get only the last commit version
    GIT_PROGRESS TRUE # show progress of download
    #FIND_PACKAGE_ARGS NAMES dpp
)
FetchContent_MakeAvailable(dpp_fc)

add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)
        
add_executable(bot bot.cpp image_generator.cpp albion_item.cpp utility.cpp price_evaluator.cpp)
target_include_directories(bot SYSTEM PUBLIC )
target_link_libraries(bot PRIVATE dpp opencv_core opencv_imgcodecs opencv_imgproc cpr::cpr)
target_include_directories(bot SYSTEM PRIVATE
        ${OPENCV_MODULE_opencv_imgcodecs_LOCATION}/include
        ${OPENCV_MODULE_opencv_imgproc_LOCATION}/include
        ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
        ${OPENCV_MODULE_opencv_core_LOCATION}/include)
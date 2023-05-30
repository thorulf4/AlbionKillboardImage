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

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(bot)
add_subdirectory(image)
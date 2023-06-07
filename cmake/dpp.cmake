include(FetchContent)

message(STATUS "DPP could not be found downloading instead")
FetchContent_Declare(
    dpp_fc
    GIT_REPOSITORY https://github.com/brainboxdotcc/DPP
    GIT_TAG v10.0.23
    GIT_SHALLOW TRUE # get only the last commit version
    GIT_PROGRESS TRUE # show progress of download
    #FIND_PACKAGE_ARGS NAMES dpp
)

FetchContent_GetProperties(dpp_fc)
if(NOT dpp_fc_POPULATED)
    FetchContent_Populate(dpp_fc)
    # Include using EXLUDE_FROM_ALL to avoid building un
    add_subdirectory(${dpp_fc_SOURCE_DIR} ${dpp_fc_BINARY_DIR} EXCLUDE_FROM_ALL)
elseif()
    message(STATUS "DPP has already been included")
endif()

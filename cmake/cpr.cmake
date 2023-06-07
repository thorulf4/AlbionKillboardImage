include(FetchContent)

message(STATUS "CPR could not be found downloading instead")
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
GIT_TAG 1.10.2 # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
GIT_SHALLOW TRUE # get only the last commit version
GIT_PROGRESS TRUE) # show progress of download

FetchContent_GetProperties(cpr)
if(NOT cpr_POPULATED)
    FetchContent_Populate(cpr)
    # Include using EXLUDE_FROM_ALL to avoid building un
    add_subdirectory(${cpr_SOURCE_DIR} ${cpr_BINARY_DIR} EXCLUDE_FROM_ALL)
elseif()
    message(STATUS "CPR has already been included")
endif()

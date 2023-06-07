include(FetchContent)


find_package(OpenCV)
if(NOT OpenCV_FOUND)
    message(STATUS "OpenCV could not be found downloading instead")
    FetchContent_Declare(
        opencv
        GIT_REPOSITORY https://github.com/opencv/opencv
        GIT_TAG 4.7.0
        GIT_SHALLOW TRUE # get only the last commit version
        GIT_PROGRESS TRUE # show progress of download
    )
    FetchContent_GetProperties(opencv)
    if(NOT opencv_POPULATED)
        FetchContent_Populate(opencv)
        # Include using EXLUDE_FROM_ALL to avoid building un
        add_subdirectory(${opencv_SOURCE_DIR} ${opencv_BINARY_DIR} EXCLUDE_FROM_ALL)
    elseif()
        message(STATUS "OpenCV has already been included")
    endif()
endif()

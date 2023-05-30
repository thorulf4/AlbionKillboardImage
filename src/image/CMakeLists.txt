add_library(image_gen image_generator.cpp albion_item.cpp utility.cpp price_evaluator.cpp)
target_link_libraries(image_gen PUBLIC opencv_core opencv_imgcodecs opencv_imgproc cpr::cpr nlohmann_json::nlohmann_json)
target_include_directories(image_gen SYSTEM PUBLIC
        ${OPENCV_MODULE_opencv_imgcodecs_LOCATION}/include
        ${OPENCV_MODULE_opencv_imgproc_LOCATION}/include
        ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
        ${OPENCV_MODULE_opencv_core_LOCATION}/include)


target_include_directories(image_gen PUBLIC "include")
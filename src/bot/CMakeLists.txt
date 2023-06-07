add_executable(bot bot.cpp)
target_link_libraries(bot PUBLIC dpp image_gen)

install(TARGETS bot dpp image_gen opencv_core opencv_imgcodecs opencv_imgproc cpr libcurl RUNTIME)
install(FILES bot.sh DESTINATION .)
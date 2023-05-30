cmake_minimum_required(VERSION 3.22.1)


set(CMAKE_CXX_STANDARD 23)
project(image_gen CXX)

# add_compile_options(-fsanitize=address -fsanitize=undefined)
# add_link_options(-fsanitize=address -fsanitize=undefined)

add_subdirectory(src)
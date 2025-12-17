cmake_minimum_required(VERSION 3.10)
project(vk_engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)

find_package(Vulkan REQUIRED)

add_executable(vk_engine application.cpp)

target_link_libraries(vk_engine
    glfw
    Vulkan::Vulkan
)

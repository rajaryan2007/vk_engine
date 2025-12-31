cmake_minimum_required(VERSION 3.10)
project(vk_engine)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW options (important)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.cc
)

file(GLOB_RECURSE INCLUDE_FILES
    src/include/*.hh
    
)

# Add GLFW from local folder
add_subdirectory(external/glfw)

# Vulkan
find_package(Vulkan REQUIRED)

# Executable
add_executable(vk_engine
   ${SRC_FILES}
   ${INCLUDE_FILES}
)
target_include_directories(vk_engine PRIVATE src/include/)
# Link libraries
target_link_libraries(vk_engine
    glfw
    Vulkan::Vulkan
)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

set(PNG_ARM_NEON on)
find_package(OpenGL REQUIRED)
add_library(utils
        src/file_util.cpp
        src/math_util.cpp
        src/string_util.cpp)
target_link_directories(utils PUBLIC
        /opt/homebrew/Cellar/assimp/5.2.5/lib
        /opt/homebrew/Cellar/boost/1.82.0_1/lib
        /opt/homebrew/Cellar/bullet/3.25/lib
        /opt/homebrew/Cellar/cgal/5.6/lib
        /opt/homebrew/Cellar/fmt/10.1.1/lib
        /opt/homebrew/Cellar/glew/2.2.0_1/lib
        /opt/homebrew/Cellar/glfw/3.3.8/lib
        /opt/homebrew/Cellar/libpng/1.6.40/lib)
target_include_directories(utils
        PUBLIC
        /opt/homebrew/Cellar/assimp/5.2.5/include
        /opt/homebrew/Cellar/boost/1.82.0_1/include
        /opt/homebrew/Cellar/bullet/3.25/include/bullet
        /opt/homebrew/Cellar/cgal/5.6/include
        /opt/homebrew/Cellar/fmt/10.1.1/include
        /opt/homebrew/Cellar/glew/2.2.0_1/include
        /opt/homebrew/Cellar/glfw/3.3.8/include
        /opt/homebrew/Cellar/glm/0.9.9.8/include
        /opt/homebrew/Cellar/libpng/1.6.40/include
        /opt/homebrew/Cellar/nlohmann-json/3.11.2/include
        include)
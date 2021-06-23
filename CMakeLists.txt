cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

set(PNG_ARM_NEON on)
find_package(OpenGL REQUIRED)
add_library(utils
        src/file_util.cpp
        src/math_util.cpp
        src/string_util.cpp)
target_link_libraries(utils
        OpenGL::GL
        GLEW
        GLFW
        CGAL
        fmt
        nlohmann_json_schema_validator
        assimp
        LinearMath
        png)
target_link_directories(utils PRIVATE
        /opt/homebrew/Cellar/glew/2.2.0_1/lib
        /opt/homebrew/Cellar/glfw/3.3.4/lib
        /opt/homebrew/Cellar/assimp/5.0.1/lib
        /opt/homebrew/Cellar/bullet/3.09/lib)
target_include_directories(utils
        PRIVATE
        ../../external/delaunator-cpp/include
        ../../external/libpng
        /opt/homebrew/Cellar/glew/2.2.0_1/include
        /opt/homebrew/Cellar/glm/0.9.9.8/include
        /opt/homebrew/Cellar/glfw/3.3.4/include
        /opt/homebrew/Cellar/assimp/5.0.1/include
        /opt/homebrew/Cellar/bullet/3.09/include/bullet
        PUBLIC include)

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
        LinearMath
        fmt
        nlohmann_json_schema_validator
        assimp
        png)
target_link_directories(utils PRIVATE
        /opt/homebrew/Cellar/assimp/5.2.4/lib
        /opt/homebrew/Cellar/bullet/3.24/lib
        /opt/homebrew/Cellar/fmt/8.1.1_1/lib
        /opt/homebrew/Cellar/glew/2.2.0_1/lib
        /opt/homebrew/Cellar/nlohmann-json/3.10.5/lib
        /opt/homebrew/Cellar/glfw/3.3.8/lib)
target_include_directories(utils
        PRIVATE
        ../../external/libpng
        ../../external/json-schema-validator
        /opt/homebrew/Cellar/fmt/8.1.1_1/include
        /opt/homebrew/Cellar/glew/2.2.0_1/include
        /opt/homebrew/Cellar/glm/0.9.9.8/include
        /opt/homebrew/Cellar/glfw/3.3.8/include
        /opt/homebrew/Cellar/assimp/5.2.4/include
        /opt/homebrew/Cellar/bullet/3.24/include/bullet
        /opt/homebrew/Cellar/nlohmann-json/3.10.5/include
        PUBLIC
        include)

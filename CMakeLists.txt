cmake_minimum_required(VERSION 3.16)

add_library(utils
        file_util.cpp
        file_util.h
        math_util.cpp
        math_util.h
        string_util.cpp
        string_util.h
        util.h)
target_link_libraries(utils fmt nlohmann_json_schema_validator)
target_include_directories(utils PUBLIC ${PROJECT_SOURCE_DIR}/external/glm .)

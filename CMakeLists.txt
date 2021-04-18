cmake_minimum_required(VERSION 3.16)

add_subdirectory(external/fmt)
add_subdirectory(external/json)
add_subdirectory(external/json-schema-validator)
add_library(utils
        src/file_util.cpp
        src/math_util.cpp
        src/string_util.cpp)
target_link_libraries(utils fmt nlohmann_json_schema_validator glm)
target_include_directories(utils PUBLIC include)

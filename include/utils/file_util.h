//
// Created by Philip on 3/2/2020.
//

#ifndef UTILS_FILE_UTIL_H
#define UTILS_FILE_UTIL_H

#include <GL/glew.h>
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>
#include <string>


using json = nlohmann::json;
using namespace nlohmann::json_schema;

namespace utils::file {
    json read_json_file(const std::string &path, const json &schema = nullptr);

    GLuint read_png_file_to_texture(const std::string &path);
} // namespace utils::file

#endif //UTILS_FILE_UTIL_H

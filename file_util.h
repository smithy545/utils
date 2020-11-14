//
// Created by Philip on 3/2/2020.
//

#ifndef CIVILWAR_FILE_UTIL_H
#define CIVILWAR_FILE_UTIL_H

#include <nlohmann/json-schema.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace nlohmann::json_schema;

namespace util::file {
    json read_json_file(const std::string &path, const json &schema = nullptr);

    void write_json_file(const std::string &path, const json &data);
} // namespace util::file

#endif //CIVILWAR_FILE_UTIL_H

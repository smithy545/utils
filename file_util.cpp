//
// Created by Philip on 3/2/2020.
//

#include "file_util.h"

#include <stdexcept>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

using std::runtime_error;

namespace util::file {
    json read_json_file(const std::string &path, const json &schema) {
        std::ifstream file(path);
        if (file.good()) {
            auto data = json::parse(file);
            if (schema != nullptr) {
                json_validator validator;
                try {
                    validator.set_root_schema(schema);
                } catch (runtime_error e) {
                    std::cerr << "Schema error: " << e.what() << std::endl;
                }
                try {
                    validator.validate(data);
                } catch (runtime_error e) {
                    std::cerr << "Validation error: " << e.what() << std::endl;
                }
            }
            return data;
        }
        std::string message = fmt::format("Can't read json file at {0}", path);
        throw runtime_error(message.c_str());
    }

    void write_json_file(const std::string &path, const json &data) {
        std::ofstream file;
        file.open(path, std::ofstream::out);
        file << data;
        file.close();
    }
} // namespace util::file

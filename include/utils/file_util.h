/* Created by Philip on 3/2/2020.
MIT License

Copyright (c) 2021 Philip Arturo Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

void write_json_file(const std::string &path, const json &data);

GLuint read_png_file_to_texture(const std::string &path);

std::string read_file_to_string(const std::string &path);
} // namespace utils::file

#endif //UTILS_FILE_UTIL_H

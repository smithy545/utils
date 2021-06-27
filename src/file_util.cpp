//
// Created by Philip on 3/2/2020.
//

#include <utils/file_util.h>

#include <stdexcept>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>


using std::runtime_error;

namespace utils::file {
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
        std::string message = fmt::format("Error reading json file at {0}", path);
        throw runtime_error(message.c_str());
    }

    GLuint read_png_file_to_texture(const std::string &path) {
        FILE* fp = fopen(path.c_str(), "rb");
        if(!fp) {
            std::string message = fmt::format("Error reading png file at {0}", path);
            throw runtime_error(message.c_str());
        }
        char header[8];
        fread(header, 1, 8, fp);
        if(!png_sig_cmp(reinterpret_cast<png_const_bytep>(header), 0, 8)) {
            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if(!png_ptr) {
                std::string message = fmt::format("Failed to create png read struct {0}", path);
                throw runtime_error(message.c_str());
            }
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if(!info_ptr){
                png_destroy_read_struct(&png_ptr,(png_infopp)NULL, (png_infopp)NULL);
                std::string message = fmt::format("Failed to create png info struct {0}", path);
                throw runtime_error(message.c_str());
            }
            png_infop end_info = png_create_info_struct(png_ptr);
            if(!end_info){
                png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
                std::string message = fmt::format("Failed to create png end info struct {0}", path);
                throw runtime_error(message.c_str());
            }
            if(setjmp(png_jmpbuf(png_ptr))){
                png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);
                fclose(fp);
                std::string message = fmt::format("Lib png error {0}", path);
                throw runtime_error(message.c_str());
            }
            png_set_sig_bytes(png_ptr, 8);
            png_init_io(png_ptr, fp);
            auto png_transforms = PNG_TRANSFORM_IDENTITY;
            png_read_png(png_ptr, info_ptr, png_transforms, NULL);
            // Todo: find a way to pass info_ptr->row_pointers directly to glTexImage2D without moving to data[]
            auto w = info_ptr->width;
            auto h = info_ptr->height;
            auto* data = new unsigned char[h*info_ptr->rowbytes];
            for(int y = 0; y < h; y++) {
                for(int x = 0; x < info_ptr->rowbytes; x += 4) {
                    data[y*info_ptr->rowbytes + x] = info_ptr->row_pointers[y][x];
                    data[y*info_ptr->rowbytes + x + 1] = info_ptr->row_pointers[y][x+1];
                    data[y*info_ptr->rowbytes + x + 2] = info_ptr->row_pointers[y][x+2];
                    data[y*info_ptr->rowbytes + x + 3] = info_ptr->row_pointers[y][x+3];
                }
            }
            GLuint tex_id;
            glGenTextures(1, &tex_id);
            glBindTexture(GL_TEXTURE_2D, tex_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            delete data;
            return tex_id;
        }
        std::string message = fmt::format("File header at {0} does not match png", path);
        throw runtime_error(message.c_str());
    }
} // namespace utils::file

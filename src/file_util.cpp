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
        // Reference: http://www.libpng.org/pub/png/libpng-1.4.0-manual.pdf
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
            GLuint tex_id;
            glGenTextures(1, &tex_id);
            glBindTexture(GL_TEXTURE_2D, tex_id);
            auto size = GL_UNSIGNED_BYTE;
            if(info_ptr->bit_depth == 16)
                size = GL_UNSIGNED_SHORT;
            if(info_ptr->bit_depth == 32)
                size = GL_UNSIGNED_INT;
            auto format = GL_RGB;
            switch(info_ptr->color_type) {
                case PNG_COLOR_TYPE_RGB:
                    format = GL_RGB;
                    break;
                case PNG_COLOR_TYPE_RGBA:
                    format = GL_RGBA;
                    break;
                case PNG_COLOR_TYPE_PALETTE:
                    format = GL_RGB;
                    size = GL_UNSIGNED_BYTE;
                    break;
                case PNG_COLOR_TYPE_GRAY:
                    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                    throw runtime_error("Gray PNG format not supported");
                case PNG_COLOR_TYPE_GRAY_ALPHA:
                    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                    throw runtime_error("Gray Alpha PNG format not supported");
                default:
                    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                    throw runtime_error(fmt::format("Format #{} not supported", info_ptr->color_type));
            }
            auto w = info_ptr->width;
            auto h = info_ptr->height;
            auto stride = info_ptr->pixel_depth/8;
            unsigned char *data;
            if(info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
                data = new unsigned char[3*h*info_ptr->rowbytes];
            else
                data = new unsigned char[h*info_ptr->rowbytes];
            for(int y = 0; y < h; y++) {
                for(int x = 0; x < info_ptr->rowbytes; x += stride) {
                    if(info_ptr->color_type == PNG_COLOR_TYPE_PALETTE) {
                        unsigned char index = info_ptr->row_pointers[y][x];
                        auto pixel = info_ptr->palette[index];
                        data[3 * (y * info_ptr->rowbytes + x)] = pixel.red;
                        data[3 * (y * info_ptr->rowbytes + x) + 1] = pixel.green;
                        data[3 * (y * info_ptr->rowbytes + x) + 2] = pixel.blue;
                    } else {
                        for (int i = 0; i < stride; i++)
                            data[y * info_ptr->rowbytes + x + i] = info_ptr->row_pointers[y][x + i];
                    }
                }
            }

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, size, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            // cleanup
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            delete[] data;

            return tex_id;
        }
        std::string message = fmt::format("File header at {0} does not match png", path);
        throw runtime_error(message.c_str());
    }

    std::string read_file_to_string(const std::string &path) {
        std::ifstream file(path);
        if (file.good()) {
            std::string data{(std::istreambuf_iterator<char>(file)),
                             (std::istreambuf_iterator<char>())};
            return data;
        }
        std::string message = fmt::format("Error reading file at {0}", path);
        throw runtime_error(message.c_str());
    }
} // namespace utils::file

#include <image_converter.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <vendor/stb_image.h>

ImageConverter::ImageConverter(const std::string& filename) {
    int width, height, num_channels;
    this->data = stbi_load(filename.c_str(), &width, &height, &num_channels, 0);
    if (this->data == nullptr) {
        std::cout << "failed to open " << filename << "!" << std::endl;
        return;
    }

    this->width = (uint32_t)width;
    this->height = (uint32_t)height;
    this->num_channels = (uint32_t)num_channels;
    this->data_size = (uint32_t)(width * height * num_channels);
}

ImageConverter::~ImageConverter() {
    stbi_image_free(this->data);
}

pear_image_t ImageConverter::get_pear_image() {
    return (pear_image_t){
        .width = this->width,
        .height = this->height,
        .num_channels = this->num_channels,
        .data = this->data,
        .data_size = this->data_size
    };
}

#include <image_converter.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <vendor/stb_image.h>

ImageConverter::ImageConverter(const std::string& filename) {
    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* data = stbi_load(filename.c_str(), &width, &height, &num_channels, 0);
    if (data == nullptr) {
        std::cout << "failed to open " << filename << "!" << std::endl;
        return;
    }

    this->image = (pear_image_t){
        .width = (uint32_t)width,
        .height = (uint32_t)height,
        .num_channels = (uint32_t)num_channels,
        .data = (uint8_t*)data,
        .data_size = (uint32_t)(width * height * num_channels)
    };
}

ImageConverter::~ImageConverter() {
    stbi_image_free(this->image.data);
}

pear_image_t ImageConverter::get_pear_image() {
    return this->image;
}

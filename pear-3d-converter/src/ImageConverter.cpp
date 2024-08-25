#include <ImageConverter.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <vendor/stb_image.h>

ImageConverter::ImageConverter(const std::string& filename) {
    stbi_set_flip_vertically_on_load(true);

    std::cout << filename << std::endl;

    this->image.data = stbi_load(filename.c_str(), &(this->image.width), &(this->image.height), &(this->image.num_channels), 0);
    this->image.size = this->image.width * this->image.height * this->image.num_channels;
}

ImageConverter::~ImageConverter() {
    stbi_image_free(this->image.data);
}

Pear3D_Image ImageConverter::getPear3D() {
    return this->image;
}

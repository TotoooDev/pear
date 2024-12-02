#pragma once

#include <pear_image.h>
#include <string>

class ImageConverter {
public:
    ImageConverter(const std::string& filename);
    ~ImageConverter();
    pear_image_t get_pear_image();

private:
    uint32_t width;
    uint32_t height;
    uint32_t num_channels;
    uint32_t data_size;
    uint8_t* data;
};

#pragma once

#include <pear_image.h>
#include <string>

class ImageConverter {
public:
    ImageConverter(const std::string& filename);
    ~ImageConverter();
    pear_image_t get_pear_image();

private:
    pear_image_t image;
};

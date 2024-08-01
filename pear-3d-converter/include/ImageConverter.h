#pragma once
#include <image.h>
#include <string>

class ImageConverter {
public:
    ImageConverter(const std::string& filename);
    ~ImageConverter();
    Pear3D_Image getPear3D();

private:
    Pear3D_Image image;
};

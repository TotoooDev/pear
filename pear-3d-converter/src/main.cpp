#include <ModelConverter.h>
#include <pear-3d.h>
#include <image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "no file provided!" << std::endl;
        return 1;
    }

    bool hasOutputParameter = argc > 2;
    std::string input = argv[1];
    std::string output = hasOutputParameter ? argv[2] : input + ".pear3d";

    ModelConverter converter(input);
    pear3d_save(output.c_str(), converter.getPear3D());

    return 0;
}

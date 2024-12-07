#include <image_converter.hpp>
#include <model_converter.hpp>
#include <pear_image.h>
#include <vendor/argparse.hpp>

int main(int argc, char* argv[]) {
    argparse::ArgumentParser parser("converter", "1.0");

    parser.add_argument("-o", "--output")
        .help("specify the output file")
        .default_value("out.model");
    
    parser.add_argument("-i", "--image")
        .help("specify to convert an image only")
        .flag();

    parser.add_argument("-v", "--verbose")
        .help("increase output verbosity")
        .flag();

    parser.add_argument("input")
        .help("specify the file to convert")
        .required();

    try {
        parser.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
        std::cout << parser;
        return 1;
    }

    std::string input = parser.get("input");
    std::string output = parser.get("--output");

    if (parser["--image"] == true) {
        if (parser["--verbose"] == true) {
            std::cout << "converting " << input << "..." << std::endl;
        }

        ImageConverter converter(input);
        pear_image_write(converter.get_pear_image(), output.c_str());
        
        return 0;
    }

    ModelConverter converter(input);
    pear_model_write(converter.get_pear_model(), output.c_str());

    return 0;
}

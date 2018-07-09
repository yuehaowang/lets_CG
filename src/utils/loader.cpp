#include <fstream>
#include <sstream>

#include "loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"


std::string Loader::LoadPlainText(const std::string& file_path)
{
    std::string txt;
    std::ifstream stream(file_path, std::ios::in);

    if (stream.is_open()) {
        std::stringstream sstr;

        sstr << stream.rdbuf();
        txt = sstr.str();

        stream.close();
    } else {
        printf("Failed to open %s\n", file_path.c_str());
    }

    return txt;
}

Image Loader::LoadImage(const std::string& path)
{
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    return Image(data, width, height, channels);
}
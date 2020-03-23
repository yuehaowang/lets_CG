#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "glyk/loader.h"
#include "thirdparty/stb_image.h"


/***************** Image *****************/

Image::Image(unsigned char* d, int w, int h, int ch)
    : data(d)
    , width(w)
    , height(h)
    , channels(ch)
{

}

void Image::Free()
{
    stbi_image_free(data);
}


/***************** Loader *****************/

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

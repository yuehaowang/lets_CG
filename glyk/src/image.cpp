#include <string.h>
#include "glyk/image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"


Image::Image()
: data(NULL)
, width(0)
, height(0)
, channels(0)
{

}

Image::Image(unsigned char* d, int w, int h, int ch)
: width(w)
, height(h)
, channels(ch)
{
    Free();
    data = (unsigned char*)malloc(w * h * ch * sizeof(unsigned char));
    memcpy(data, d, w * h * ch);
}

Image::Image(const Image& img)
{
    Image(img.Data(), img.Width(), img.Height(), img.Channels());
}

Image::Image(const std::string& path)
{
    Load(path);
}

Image::~Image()
{
    Free();
}

void Image::Load(const std::string& path)
{
    Free();
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

bool Image::IsNull() const
{
    return data == NULL;
}

void Image::Free()
{
    if (IsNull()) {
        return;
    }
    stbi_image_free(data);
    data = NULL;
}

unsigned char* Image::Data() const
{
    return data;
}

int Image::Width() const
{
    return width;
}

int Image::Height() const
{
    return height;
}

int Image::Channels() const
{
    return channels;
}

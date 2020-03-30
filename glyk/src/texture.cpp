#include <string.h>
#include "glyk/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"


Texture::Texture()
: data(NULL)
, width(0)
, height(0)
, channels(0)
{

}

Texture::Texture(unsigned char* d, int w, int h, int ch)
: width(w)
, height(h)
, channels(ch)
{
    Free();
    data = (unsigned char*)malloc(w * h * ch * sizeof(unsigned char));
    memcpy(data, d, w * h * ch);
}

Texture::Texture(const Texture& tex)
{
    Texture(tex.Data(), tex.Width(), tex.Height(), tex.Channels());
}

void Texture::Load(const std::string& path)
{
    Free();
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

bool Texture::IsNull() const
{
    return data == NULL;
}

void Texture::Free()
{
    if (IsNull()) {
        return;
    }
    stbi_image_free(data);
    data = NULL;
}

unsigned char* Texture::Data() const
{
    return data;
}

int Texture::Width() const
{
    return width;
}

int Texture::Height() const
{
    return height;
}

int Texture::Channels() const
{
    return channels;
}
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
// #include "thirdparty/stb_image.h"


class Texture
{

protected:

    unsigned char* data;
    int width;
    int height;
    int channels;

public:

    Texture();
    Texture(const Texture& tex);
    Texture(unsigned char* d, int w, int h, int ch);
    bool IsNull() const;
    void Free();
    void Load(const std::string& path);
    unsigned char* Data() const;
    int Width() const;
    int Height() const;
    int Channels() const;

};


#endif
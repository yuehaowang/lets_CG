#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "external/stb_image.h"

struct Image
{
    unsigned char* data;
    int width;
    int height;
    int channels;

    Image(unsigned char* d, int w, int h, int ch)
        : data(d)
        , width(w)
        , height(h)
        , channels(ch)
    {
    
    }

    void Free()
    {
        stbi_image_free(data);
    }
};


#endif

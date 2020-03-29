#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#define TINYOBJLOADER_IMPLEMENTATION
#include "thirdparty/tiny_obj_loader.h"


/***************** Image *****************/

struct Image
{
    unsigned char* data;
    int width;
    int height;
    int channels;

    Image(unsigned char* d, int w, int h, int ch);
    void Free();
};


/***************** Loader *****************/

class Loader
{

public:

    static std::string LoadPlainText(const std::string& file_path);
    static Image LoadImage(const std::string& path);

};


#endif
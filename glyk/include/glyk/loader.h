#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#include "glyk/geometry.h"
#include "thirdparty/tiny_obj_loader.h"


/***************** Image *****************/

struct Image
{
    unsigned char* data;
    int width;
    int height;
    int channels;

    Image();
    Image(unsigned char* d, int w, int h, int ch);
    bool IsNull();
    void Free();
};


/***************** Loader *****************/

class Loader
{

public:

    static std::string LoadPlainText(const std::string& file_path);
    static Image LoadImage(const std::string& path);
    static std::vector<Geometry> LoadModel(const std::string& path);

};


#endif
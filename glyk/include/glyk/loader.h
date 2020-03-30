#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#include "glyk/geometry.h"
#include "glyk/texture.h"
#include "thirdparty/tiny_obj_loader.h"


/***************** Loader *****************/

class Loader
{

public:

    static std::string LoadPlainText(const std::string& file_path);
    static Texture LoadTexture(const std::string& path);
    static std::vector<Geometry> LoadModel(const std::string& path);

};


#endif
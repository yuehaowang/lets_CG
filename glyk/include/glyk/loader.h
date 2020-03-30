#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#include "glyk/geometry.h"
#include "glyk/image.h"
#include "thirdparty/tiny_obj_loader.h"


/***************** Loader *****************/

class Loader
{

public:

    enum PreprocessFlag {
        None = 0x00000000,
        FlipTexcoordX = 0x00000001,
        FlipTexcoordY = 0x00000002
    };

    static std::string LoadPlainText(const std::string& file_path);
    static Image LoadImage(const std::string& path);
    static std::vector<Geometry> LoadModel(const std::string& path, PreprocessFlag flag = None);

};


#endif
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
        FlipTexcoordU = 0x00000001,
        FlipTexcoordV = 0x00000002,
        ComputeTBN = 0x00000004
    };

    static std::string LoadPlainText(const std::string& file_path);
    static Image LoadImage(const std::string& path);
    static std::vector<Geometry> LoadModel(const std::string& path, PreprocessFlag flag = None);

};

Loader::PreprocessFlag operator| (Loader::PreprocessFlag f1, Loader::PreprocessFlag f2);


#endif
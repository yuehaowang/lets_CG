#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>
#include "image.h"


class Loader
{

public:

    static std::string LoadPlainText(const std::string& file_path);
    static Image LoadImage(const std::string& path);

};


#endif
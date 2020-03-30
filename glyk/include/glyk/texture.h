#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "glyk/image.h"
#include "glyk/gl_header_files.h"


class Material;

class Texture
{

protected:

    GLuint tex_id;
    int width;
    int height;
    int channels;
    bool is_null;

public:

    Texture();
    Texture(const std::string& path);
    Texture(const Image& img);
    void Load(const Image& img);
    void Load(const std::string& path);
    int Width() const;
    int Height() const;
    int Channels() const;
    GLuint TexId() const;
    bool IsNull() const;

};


#endif
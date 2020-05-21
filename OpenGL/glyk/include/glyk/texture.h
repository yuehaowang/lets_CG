#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <map>
#include "glyk/image.h"
#include "glyk/gl_header_files.h"


class Material;

class Texture
{

protected:

    GLuint tex_id;
    bool is_null;

    void CreateTex(const Image& img);

public:

    static std::map<std::string, GLuint> texture_list; 

    Texture();
    Texture(const std::string& path);
    Texture(const Image& img);
    void Load(const Image& img);
    void Load(const std::string& path);
    GLuint TexId() const;
    bool IsNull() const;

};


#endif
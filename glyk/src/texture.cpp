#include "glyk/texture.h"


std::map<std::string, GLuint> Texture::texture_list = std::map<std::string, GLuint>(); 

Texture::Texture()
: is_null(true)
{

}

Texture::Texture(const std::string& path)
{
    Load(path);
}

Texture::Texture(const Image& img)
{
    Load(img);
}

void Texture::CreateTex(const Image& img)
{
    printf("-- Creating texture %s...\n", img.Path().c_str());

    glGenTextures(1, &tex_id);

    GLenum format;
    if (img.Channels() == 1) {
        format = GL_RED;
    } else if (img.Channels() == 3) {
        format = GL_RGB;
    } else if (img.Channels() == 4) {
        format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, img.Width(), img.Height(), 0, format, GL_UNSIGNED_BYTE, img.Data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (!img.Path().empty()) {
        texture_list.insert(std::pair<std::string, GLuint>(img.Path(), tex_id));
    }
}

void Texture::Load(const Image& img)
{
    if (img.IsNull()) {
        return;
    }

    std::map<std::string, GLuint>::iterator search = texture_list.find(img.Path());
    if (search != texture_list.end()) {
        tex_id = search->second;
    } else {
        CreateTex(img);
    }

    is_null = false;
}

void Texture::Load(const std::string& path)
{
    std::map<std::string, GLuint>::iterator search = texture_list.find(path);
    if (search != texture_list.end()) {
        tex_id = search->second;
    } else {
        Image img;
        img.Load(path);
        CreateTex(img);
        img.Free();
    }

    is_null = false;
}

GLuint Texture::TexId() const
{
    return tex_id;
}

bool Texture::IsNull() const
{
    return is_null;
}

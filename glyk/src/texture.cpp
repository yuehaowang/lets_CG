#include "glyk/texture.h"


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

void Texture::Load(const Image& img)
{
    if (img.IsNull()) {
        return;
    }

    width = img.Width();
    height = img.Height();
    channels = img.Channels();

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

    is_null = false;
}

void Texture::Load(const std::string& path)
{
    Image img;
    img.Load(path);

    Load(img);

    img.Free();
}

int Texture::Width() const
{
    return width;
}

int Texture::Height() const
{
    return height;
}

int Texture::Channels() const
{
    return channels;
}

GLuint Texture::TexId() const
{
    return tex_id;
}

bool Texture::IsNull() const
{
    return is_null;
}

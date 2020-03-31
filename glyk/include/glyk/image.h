#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>


class Image
{

protected:

    unsigned char* data;
    std::string path;
    int width;
    int height;
    int channels;

public:

    Image();
    Image(const Image& img);
    Image(unsigned char* d, int w, int h, int ch);
    Image(const std::string& path);
    ~Image();
    bool IsNull() const;
    void Free();
    void Load(const std::string& path);
    unsigned char* Data() const;
    int Width() const;
    int Height() const;
    int Channels() const;
    std::string Path() const;

};


#endif
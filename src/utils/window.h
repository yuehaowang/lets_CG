#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>

#include "gl_header_files.h"


class Window
{

protected:

    std::string window_title;
    int window_width;
    int window_height;
    GLFWwindow* window;

public:

    Window(const std::string& title, int width, int height);
    void Run();
    virtual void OnStart();
    virtual void OnUpdate();
    virtual void OnDestroy();
    int WindowWidth() const;
    int WindowHeight() const;
    const std::string& WindowTitle() const;
    ~Window();

};


#endif
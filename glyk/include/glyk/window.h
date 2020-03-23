#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include "glyk/gl_header_files.h"


class Window
{

protected:

    static GLFWwindow* window;
    static std::string window_title;
    static int window_width;
    static int window_height;

    static void OnFrameBufferResized(GLFWwindow* win, int width, int height);

public:

    Window(const std::string& title, int width, int height);
    virtual ~Window();
    void Run();
    virtual void OnStart();
    virtual void OnUpdate();
    virtual void OnDestroy();
    int WindowWidth() const;
    int WindowHeight() const;
    GLFWwindow* WindowHandler() const;
    const std::string& WindowTitle() const;

};


#endif
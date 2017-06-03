#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <string>
#include <GLFW/glfw3.h>

#include "interactivesoundstream.hpp"

class Window
{
public:
    Window();
    ~Window();
    bool good() const
    {
        return !glfwWindowShouldClose(m_window);
    }
    void open(std::string name, int w, int h);
    void close();
    void update();
private:
    void readConfig();
    GLFWwindow* m_window;
    InteractiveSoundStream m_stream;
};

#endif // _WINDOW_HPP_INCLUDED


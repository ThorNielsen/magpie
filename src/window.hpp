#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include <string>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    void open(std::string name, int w, int h);
    void update();
    ~Window();
private:
    GLFWwindow* m_window;
};

#endif // _WINDOW_HPP_INCLUDED


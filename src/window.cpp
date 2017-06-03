#include "window.hpp"

Window::Window()
{
    glfwInit();
    m_window = nullptr;
}

void Window::open(std::string name, int w, int h)
{
    m_window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
}

void Window::update()
{
    glfwSwapBuffers(m_window);
}

Window::~Window()
{
    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}


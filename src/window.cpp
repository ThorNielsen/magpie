#include "window.hpp"

#include <cmath>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include "algorithm.hpp"

#define UNUSED(x) ((void)x)

// Screw good practice and make lots of stuff global to avoid having to code a
// callback -> window system. It is, however, at least contained in an anonymous
// namespace, such that no other part of the code needs to know of this.
namespace
{

std::map<std::string, int> g_keynames =
{
    {"A", GLFW_KEY_A}, {"B", GLFW_KEY_B}, {"C", GLFW_KEY_C}, {"D", GLFW_KEY_D},
    {"E", GLFW_KEY_E}, {"F", GLFW_KEY_F}, {"G", GLFW_KEY_G}, {"H", GLFW_KEY_H},
    {"I", GLFW_KEY_I}, {"J", GLFW_KEY_J}, {"K", GLFW_KEY_K}, {"L", GLFW_KEY_L},
    {"M", GLFW_KEY_M}, {"N", GLFW_KEY_N}, {"O", GLFW_KEY_O}, {"P", GLFW_KEY_P},
    {"Q", GLFW_KEY_Q}, {"R", GLFW_KEY_R}, {"S", GLFW_KEY_S}, {"T", GLFW_KEY_T},
    {"U", GLFW_KEY_U}, {"V", GLFW_KEY_V}, {"W", GLFW_KEY_W}, {"X", GLFW_KEY_X},
    {"Y", GLFW_KEY_Y}, {"Z", GLFW_KEY_Z}, {"0", GLFW_KEY_0}, {"1", GLFW_KEY_1},
    {"2", GLFW_KEY_2}, {"3", GLFW_KEY_3}, {"4", GLFW_KEY_4}, {"5", GLFW_KEY_5},
    {"6", GLFW_KEY_6}, {"7", GLFW_KEY_7}, {"8", GLFW_KEY_8}, {"9", GLFW_KEY_9},
    {",", GLFW_KEY_COMMA}, {".", GLFW_KEY_PERIOD},
};

struct Tone
{
    size_t a, b, c, d;
};

InteractiveSoundStream* g_stream = nullptr;
std::map<int, double> g_frequency;
std::map<int, Tone> g_tones;

double g_maxAmplitude;

void keycb(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(wnd, true);
        return;
    }
    UNUSED(wnd);
    UNUSED(scancode);
    UNUSED(mods);
    if (g_stream == nullptr) return;
    auto binding = g_frequency.find(key);
    if (binding != g_frequency.end())
    {
        U64 pos = g_stream->getCurrentSample() + g_stream->getLag();
        auto tone = g_tones.find(key);
        if (action == GLFW_PRESS && tone == g_tones.end())
        {
            double freq = binding->second;
            Tone t;
            t.a = g_stream->addCoefficients(1., freq * 1., pos);
            t.b = g_stream->addCoefficients(9., freq * 2., pos);
            t.c = g_stream->addCoefficients(15./4., freq * 3., pos);
            t.d = g_stream->addCoefficients(9./5., freq * 4., pos);
            g_stream->setMaxAmplitude(g_maxAmplitude);
            g_tones[key] = t;
        }
        else if (action == GLFW_RELEASE && tone != g_tones.end())
        {
            Tone t = tone->second;
            g_stream->removeCoefficients(t.a);
            g_stream->removeCoefficients(t.b);
            g_stream->removeCoefficients(t.c);
            g_stream->removeCoefficients(t.d);
            g_tones.erase(tone);
        }
    }
}

}

Window::Window()
{
    glfwInit();
    m_window = nullptr;
    readConfig();
    m_stream.addCoefficients(1., 1.);
    m_stream.addCoefficients(9., 2.);
    m_stream.addCoefficients(15./4., 3.);
    m_stream.addCoefficients(9./5., 4.);
}

Window::~Window()
{
    close();
}


void Window::open(std::string name, int w, int h)
{
    m_window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, keycb);
    if (g_stream != nullptr)
    {
        throw std::runtime_error("Cannot open two windows at the same time.");
    }
    g_stream = &m_stream;
    m_stream.play();
}

void Window::update()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::close()
{
    m_stream.stop();
    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
    g_stream = nullptr;
}

void Window::readConfig()
{
    std::ifstream bindings("bindings.txt");
    std::string linereader;
    double maxNotes = 1.;
    double refFreq = 440.;
    while (std::getline(bindings, linereader))
    {
        if (linereader[0] == '#' || !linereader.size()) continue;
        std::stringstream line(trimBeginEndSpace(linereader));
        std::string inputType;
        line >> inputType;

        std::string keyname;
        double freq;
        if (inputType == "frequency" || inputType == "freq" || inputType == "f")
        {
            line >> refFreq;
            continue;
        }
        else if (inputType == "maxnotes" || inputType == "m")
        {
            line >> maxNotes;
            continue;
        }
        else if (inputType == "tone" || inputType == "t")
        {
            line >> keyname >> freq;
            freq = refFreq * std::pow(2., freq/12.);
        }
        else // Raw frequency.
        {
            line >> keyname >> freq;
        }
        g_frequency[g_keynames[keyname]] = freq;
    }
    m_stream.setFrequency(1., 0);
    g_maxAmplitude = maxNotes * 15.55;
}

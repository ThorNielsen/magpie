#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <utility>
#include <string>
#include <stdexcept>

#include <cstdio>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include <cstdlib>
#include <map>

#include <GLFW/glfw3.h>

//#define RUNTIME_CHECKS
#include "interactivesoundstream.hpp"
#include "window.hpp"

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        std::string opt = argv[1];
        if (opt != "-t" && opt != "--test")
        {
            std::cerr << "Unknown argument.\n";
            return 1;
        }
        sf::Music mus;
        mus.openFromFile("test.flac");
        mus.play();
        while (mus.getStatus() == sf::Music::Playing)
        {
            sf::sleep(sf::seconds(0.01f));
        }
        return 0;
    }
    Window wnd;
    //wnd.open("Bob", 640, 480);
    InteractiveSoundStream ss;

    ss.addCoefficients(1., 1.);
    ss.addCoefficients(9., 2.);
    ss.addCoefficients(15./4., 3.);
    ss.addCoefficients(9./5., 4.);

    std::map<char, double> notes
    {
        {'q', 0.}, {'w', 1.}, {'e', 2.}, {'r', 3.}, {'t', 4.}, {'y', 5.},
        {'u', 6.}, {'i', 7.}, {'o', 8.}, {'p', 9.}, {'a', 10.}, {'s', 11.},
        {'d', 12.}, {'f', 13.}, {'g', 14.}, {'h', 15.}, {'j', 16.}, {'k', 17.},
        {'l', 18.}, {'z', 19.}, {'x', 20.}, {'c', 21.}, {'v', 22.}, {'b', 23.},
        {'n', 24.}, {'m', 25.}, {',', 26.}, {'.', 27.}
    };

    ss.play();
    system("/bin/stty -icanon");
    std::noskipws(std::cin);
    char input = 'A';
    while (input != 0x1b)
    {
        input = (char)getchar();
        auto it = notes.find(input);
        double freq;
        if (it != notes.end())
        {
            freq = 440. * std::pow(2., it->second/12.);
        }
        else
        {
            freq = 0.;
        }
        ss.setFrequency(freq, ss.getCurrentSample() + ss.getLag());
    }
    ss.stop();
    return 0;
}

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

#include <SFML/Audio.hpp>

#include <GLFW/glfw3.h>

//#define RUNTIME_CHECKS
#include "soundstream.hpp"
#include "window.hpp"

double getCoefficient(double frequency)
{
    return 6.2831853071795864769252867665590058 * frequency;
}

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
    SoundStream ss;

    /*
    ss.appendCoefficients(1., getCoefficient(440.));
    ss.appendCoefficients(9., getCoefficient(440. * 2.));
    ss.appendCoefficients(15./4., getCoefficient(440. * 3.));
    ss.appendCoefficients(9./5., getCoefficient(440. * 4.));//*/
    ss.appendCoefficients(1., 1.);
    ss.appendCoefficients(9., 2.);
    ss.appendCoefficients(15./4., 3.);
    ss.appendCoefficients(9./5., 4.);
    //ss.setFrequency(440.);
    //ss.appendCoefficients(1.0, )
    /*for (double d = 1.; d < 100.; d += 1.)
    {
        ss.appendCoefficients(5./d, d*100.);
    }*//*
    Coefficients cf =
        {{1., 1.}};//, {0.1, 2.}, {0.33, 3.}, {0.06, 4.}, {0.05, 5.}};

    for (size_t i = 0; i < cf.size(); ++i)
    {
        //cf[i].second *= getCoefficient(440.);
    }

    ss.setCoefficients(cf);//*/

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
        ss.setFrequency(freq);
    }
    ss.stop();
    return 0;
}

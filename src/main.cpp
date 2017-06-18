// This file is part of magpie
// Copyright (C) 2017 Thor G. Nielsen
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>

#include <GLFW/glfw3.h>

#include "interactivesoundstream.hpp"
#include "window.hpp"
#include "function.hpp"

void instrument();
bool playFunctionFromFile(std::string path);
void test();
void menu();

void instrument()
{
    Window wnd;
    wnd.open("Bob", 640, 480);
    wnd.update();
    while (wnd.good())
    {
        wnd.update();
    }
    wnd.close();
}

bool playFunctionFromFile(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
    {
        // Windows unfortunately hides file extension, so some people may not be
        // aware that they actually want to use a .txt file and therefore omit
        // the extension.
        in.open(path + ".txt");
    }
    if (!in.is_open())
    {
        return false;
    }
    playFunction(in);
    return true;
}

void test()
{
    std::cout << "Testing";
    sf::Music mus;
    mus.openFromFile("test.flac");
    mus.play();
    while (mus.getStatus() == sf::Music::Playing)
    {
        std::cout << "." << std::flush;
        sf::sleep(sf::seconds(0.1f));
    }
    std::cout << "\nNot testing any more.\n";
}

void menu()
{
    bool exit = false;
    while (!exit)
    {

        std::cout << "What do you want [Valid answers: function, interactive, "
                     "test, exit]? ";
        std::string choice;
        std::cin >> choice;
        if (choice == "f" || choice == "function")
        {
            std::cin.ignore();
            std::cout << "Enter file name: ";
            std::string filename;
            std::getline(std::cin, filename);
            if (!playFunctionFromFile(filename))
            {
                if (filename.size()) std::cout << "File not found.\n";
                std::cout << "Enter function definition (end with CTRL+D (Linux"
                             ") or CTRL+Z (Windows)):\n";
                playFunction(std::cin);
            }
        }
        else if (choice == "i"
                 || choice == "interactive"
                 || choice == "instrument")
        {
            instrument();
        }
        else if (choice == "t" || choice == "test")
        {
            test();
        }
        else if (choice == "q" || choice == "quit"
                 || choice == "e" || choice == "exit")
        {
            exit = true;
        }
        else
        {
            if (std::cin.eof())
            {
                exit = true;
            }
            else
            {
                std::cout << "Unrecognised command '" << choice << "'\n";
            }
        }
    }
}

int main(int argc, char** argv)
{
    if (argc > 1)
    for (int i = 1; i < argc; ++i)
    {
        std::string opt = argv[i];
        if (opt == "-t" || opt == "--test")
        {
            test();
        }
        else if (opt == "-i" || opt == "--interactive" || opt == "--instrument")
        {
            instrument();
        }
        else if (opt == "-f" || opt == "--function")
        {
            if (i == argc-1)
            {
                std::cerr << "No file name specified.\n";
                return 2;
            }
            else
            {
                ++i;
                if (!playFunctionFromFile(argv[i]))
                {
                    std::cerr << "Couldn't open " << argv[i] << ".\n";
                    return 3;
                }
            }
        }
        else
        {
            std::cerr << "Unknown argument.\n";
            return 1;
        }
    }
    if (argc == 1)
    {
        menu();
    }
    return 0;
}

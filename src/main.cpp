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
                std::cout << "Enter function definition (end with CTRL+Z (Linux"
                             ") or CTRL+D (Windows)):\n";
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

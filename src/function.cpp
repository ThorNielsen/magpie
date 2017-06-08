#include "function.hpp"
#include "soundstream.hpp"
#include "algorithm.hpp"

#include <iostream>
#include <sstream>
#include <string>

void playFunction(std::istream& input)
{
    SoundStream ss;
    std::string line;
    while (!input.eof())
    {
        std::getline(input, line);
        line = trimBeginEndSpace(line);
        if (!line.size() || line[0] == '#') continue;
        std::stringstream reader(line);
        std::string inputType;
        reader >> inputType;
        if (inputType == "frequency" || inputType == "freq" || inputType == "f")
        {
            double freq = 0.;
            reader >> freq;
            ss.setFrequency(freq);
        }
        else if (inputType == "coefficients" ||
                 inputType == "coeff" ||
                 inputType == "c")
        {
            double a = 0., b = 0.;
            reader >> a >> b;
            ss.addCoefficients(a, b);
        }
        else
        {
            std::cerr << "Invalid line: " << line << "\n";
        }
    }
    input.clear();
    ss.play();
    std::cout << "Press enter to stop." << std::endl;
    std::cin.get();
    ss.stop();
}

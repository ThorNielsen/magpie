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

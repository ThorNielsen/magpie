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

#ifndef SOUNDSTREAM_HPP_INCLUDED
#define SOUNDSTREAM_HPP_INCLUDED

#include <cmath>
#include <vector>
#include <SFML/Audio.hpp>

using S16 = int16_t;
using U64 = uint64_t;

using Coefficients = std::vector<std::pair<double, double>>;

class SoundStream : public sf::SoundStream
{
public:
    SoundStream()
    {
        initialize(1, 44100);
    }

    void addCoefficients(double a, double b)
    {
        m_coeff.push_back({a, b});
        updateMaxval();
    }

    void setCoefficients(Coefficients c)
    {
        m_coeff = c;
        updateMaxval();
    }

    void setCoefficients(Coefficients&& c)
    {
        m_coeff = std::move(c);
        updateMaxval();
    }
    void setFrequency(double frequency)
    {
        m_freq = 6.2831853071795864769252867665590058 * frequency;
    }
private:
    void updateMaxval()
    {
        double maxval = 0.;
        for (size_t i = 0; i < m_coeff.size(); ++i)
        {
            maxval += m_coeff[i].first;
        }
        m_invmaxval = 1. / maxval;
    }

    inline S16 getSample(U64 sample)
    {
        double t = static_cast<double>(sample) / 44100.;
        double intensity = 0.;
        for (size_t i = 0; i < m_coeff.size(); ++i)
        {
            intensity += m_coeff[i].first * std::sin(t * m_coeff[i].second * m_freq);
        }
        intensity *= m_invmaxval;
        /*
        if (std::abs(intensity) > 1.)
        {
            throw std::logic_error("Too high intensity (|"
                                    + std::to_string(intensity)
                                    + "| > 1).");
        }
        */
        return static_cast<S16>(intensity * 32767.);
    }

    bool onGetData(Chunk& data) override
    {
        std::vector<S16> samples;
        for (S16 i = 0; i < 44100/25; ++i)
        {
            samples.push_back(getSample(m_samplepos+i));
        }
        data.samples = samples.data();
        data.sampleCount = samples.size();
        m_samplepos += samples.size();
        return true;
    }

    void onSeek(sf::Time seekOffset) override
    {
        m_samplepos = static_cast<U64>(seekOffset.asSeconds())
                                       * getSampleRate();
    }


    Coefficients m_coeff;
    U64 m_samplepos = 0;
    double m_freq = 0.;
    double m_invmaxval = 0.;
};

#endif // _SOUNDSTREAM_HPP_INCLUDED

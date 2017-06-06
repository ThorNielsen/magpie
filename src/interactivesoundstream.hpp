#ifndef INTERACTIVESOUNDSTREAM_HPP_INCLUDED
#define INTERACTIVESOUNDSTREAM_HPP_INCLUDED

#include <cmath>
#include <limits>
#include <vector>
#include <SFML/Audio.hpp>

using S16 = int16_t;
using U64 = uint64_t;
using S16 = int16_t;
using U64 = uint64_t;

constexpr U64 maxSample = std::numeric_limits<U64>::max();

class InteractiveSoundStream : public sf::SoundStream
{
public:
    InteractiveSoundStream()
    {
        initialize(1, 44100);
    }

    size_t addCoefficients(double a, double b, U64 startSample = 0)
    {
        for (size_t i = 0; i < m_coeff.size(); ++i)
        {
            if (m_coeff[i].startSample == maxSample)
            {
                m_coeff[i] = {a, b, startSample, maxSample};
                updateMaxval();
                return i;
            }
        }
        m_coeff.push_back({a, b, startSample, maxSample});
        updateMaxval();
        return m_coeff.size() - 1;
    }

    void removeCoefficients(size_t idx, U64 endSample = 0)
    {
        m_coeff[idx].endSample = endSample;
    }

    void setFrequency(double frequency, U64 sample)
    {
        m_newfreq = 6.2831853071795864769252867665590058 * frequency;
        m_freqchangepos = sample;
    }

    void setMaxAmplitude(double ma)
    {
        if (m_invmaxval > 1. / ma)
        {
            m_invmaxval = 1. / ma;
        }
    }

    U64 getLag() const
    {
        return 44100/35;
    }

    U64 getCurrentSample() const
    {
        U64 mms = getPlayingOffset().asMicroseconds();
        return (mms * 44100) / 1000000;
    }

private:
    struct Coefficients
    {
        double first;
        double second;
        U64 startSample;
        U64 endSample;
    };

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
        if (sample >= m_freqchangepos)
        {
            m_freq = m_newfreq;
        }
        double tf = m_freq * (static_cast<double>(sample) / 44100.);
        double intensity = 0.;
        for (size_t i = 0; i < m_coeff.size(); ++i)
        {
            if (sample >= m_coeff[i].endSample)
            {
                m_coeff[i].startSample = maxSample;
            }
            if (sample < m_coeff[i].startSample) continue;
            intensity += m_coeff[i].first * std::sin(tf * m_coeff[i].second);
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
        for (size_t i = 0; i < getLag(); ++i)
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


    std::vector<Coefficients> m_coeff;
    U64 m_samplepos = 0;
    U64 m_freqchangepos = 0;
    double m_newfreq = 0.;
    double m_freq = 0.;
    double m_invmaxval = 0.;
};

#endif // INTERACTIVESOUNDSTREAM_HPP_INCLUDED


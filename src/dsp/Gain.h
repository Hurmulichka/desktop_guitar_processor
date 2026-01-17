#pragma once

#include "Smoother.h"
#include <cmath>

/**
 * Gain processor with smoothing
 * Converts dB values to linear gain and applies smoothing
 */
class Gain
{
public:
    Gain() = default;
    
    void prepare(double sampleRate)
    {
        smoother.reset(sampleRate, 20.0);
    }
    
    void setGainDecibels(double gainDb)
    {
        double linearGain = std::pow(10.0, gainDb / 20.0);
        smoother.setTargetValue(linearGain);
    }
    
    double processMonoSample(double input)
    {
        return input * smoother.getNextValue();
    }
    
    double getCurrentGain() const
    {
        return smoother.getCurrentValue();
    }
    
private:
    Smoother smoother;
};

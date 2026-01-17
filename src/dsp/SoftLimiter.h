#pragma once

#include <cmath>
#include <algorithm>

/**
 * Soft limiting for output protection
 * Prevents digital clipping using tanh saturation
 */
class SoftLimiter
{
public:
    SoftLimiter() = default;
    
    void prepare(double /*sampleRate*/)
    {
        // No preparation needed for this simple limiter
    }
    
    void setThreshold(double thresholdDb)
    {
        threshold = std::pow(10.0, thresholdDb / 20.0);
    }
    
    double processSample(double input)
    {
        double absInput = std::abs(input);
        
        if (absInput <= threshold)
        {
            return input;
        }
        else
        {
            // Soft clipping using tanh
            double excess = absInput - threshold;
            double softened = threshold + std::tanh(excess * 2.0) * 0.5;
            return (input >= 0.0 ? 1.0 : -1.0) * softened;
        }
    }
    
private:
    double threshold = 0.95; // Default threshold at -0.5 dB
};

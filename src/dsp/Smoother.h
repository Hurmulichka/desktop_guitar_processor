#pragma once

#include <cmath>

/**
 * Simple one-pole smoothing filter for parameter changes
 * Prevents zipper noise and provides smooth parameter transitions
 */
class Smoother
{
public:
    Smoother() = default;
    
    void reset(double sampleRate, double timeMs = 20.0)
    {
        currentValue = targetValue;
        coefficient = std::exp(-1.0 / (sampleRate * timeMs * 0.001));
    }
    
    void setTargetValue(double newTarget)
    {
        targetValue = newTarget;
    }
    
    double getNextValue()
    {
        currentValue = targetValue + coefficient * (currentValue - targetValue);
        return currentValue;
    }
    
    double getCurrentValue() const { return currentValue; }
    bool isSmoothing() const { return std::abs(currentValue - targetValue) > 0.0001; }
    
private:
    double currentValue = 0.0;
    double targetValue = 0.0;
    double coefficient = 0.0;
};

#pragma once

#include <atomic>
#include <cmath>
#include <algorithm>

/**
 * Audio meter for measuring peak and RMS levels
 * Thread-safe communication between audio thread and UI thread
 */
class Meter
{
public:
    Meter() = default;
    
    void prepare(double sampleRate)
    {
        this->sampleRate = sampleRate;
        rmsWindow = static_cast<int>(sampleRate * 0.3); // 300ms window
        rmsSum = 0.0;
        rmsSampleCount = 0;
    }
    
    void reset()
    {
        peakLevel.store(0.0f, std::memory_order_relaxed);
        rmsLevel.store(0.0f, std::memory_order_relaxed);
        rmsSum = 0.0;
        rmsSampleCount = 0;
    }
    
    // Call this from audio thread for each sample
    void processSample(double sample)
    {
        double absSample = std::abs(sample);
        
        // Update peak
        float currentPeak = peakLevel.load(std::memory_order_relaxed);
        if (absSample > currentPeak)
        {
            peakLevel.store(static_cast<float>(absSample), std::memory_order_relaxed);
        }
        
        // Update RMS
        rmsSum += sample * sample;
        rmsSampleCount++;
        
        if (rmsSampleCount >= rmsWindow)
        {
            double rms = std::sqrt(rmsSum / rmsSampleCount);
            rmsLevel.store(static_cast<float>(rms), std::memory_order_relaxed);
            rmsSum = 0.0;
            rmsSampleCount = 0;
        }
    }
    
    // Call from UI thread
    float getPeakLevel() const
    {
        return peakLevel.load(std::memory_order_relaxed);
    }
    
    float getRMSLevel() const
    {
        return rmsLevel.load(std::memory_order_relaxed);
    }
    
    // Call from UI thread to decay peak for display
    void decayPeak(float decayRate = 0.98f)
    {
        float current = peakLevel.load(std::memory_order_relaxed);
        peakLevel.store(current * decayRate, std::memory_order_relaxed);
    }
    
private:
    std::atomic<float> peakLevel{0.0f};
    std::atomic<float> rmsLevel{0.0f};
    
    double sampleRate = 44100.0;
    int rmsWindow = 0;
    double rmsSum = 0.0;
    int rmsSampleCount = 0;
};

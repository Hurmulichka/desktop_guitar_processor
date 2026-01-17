#pragma once

#include <JuceHeader.h>
#include "../dsp/Meter.h"

/**
 * Visual meter component for displaying audio levels
 */
class MeterComponent : public juce::Component,
                       private juce::Timer
{
public:
    MeterComponent(const Meter& meter, const juce::String& label);
    ~MeterComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    void timerCallback() override;
    
    const Meter& meterSource;
    juce::String labelText;
    
    float displayPeak = 0.0f;
    float displayRMS = 0.0f;
    
    static constexpr float DECAY_RATE = 0.96f;
};

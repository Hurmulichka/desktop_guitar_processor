#pragma once

#include <JuceHeader.h>
#include "ParameterBridge.h"
#include "../dsp/Gain.h"
#include "../dsp/Meter.h"
#include "../dsp/SoftLimiter.h"

/**
 * Core audio processing engine
 * Implements the DSP chain: Input -> InputGain -> Bypass -> MasterGain -> Limiter -> Output
 */
class AudioEngine : public juce::AudioIODeviceCallback
{
public:
    AudioEngine(ParameterBridge& paramBridge);
    ~AudioEngine() override;
    
    // AudioIODeviceCallback interface
    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                         int numInputChannels,
                                         float* const* outputChannelData,
                                         int numOutputChannels,
                                         int numSamples,
                                         const juce::AudioIODeviceCallbackContext& context) override;
    
    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void audioDeviceError(const juce::String& errorMessage) override;
    
    // Meter access for UI
    const Meter& getInputMeter() const { return inputMeter; }
    const Meter& getOutputMeter() const { return outputMeter; }
    
private:
    ParameterBridge& parameters;
    
    // DSP components
    Gain inputGain;
    Gain masterGain;
    SoftLimiter outputLimiter;
    
    Meter inputMeter;
    Meter outputMeter;
    
    // Bypass crossfade
    double bypassCrossfade = 0.0; // 0.0 = bypassed, 1.0 = processing
    double bypassSmoothCoeff = 0.0;
    
    // Denormals protection
    void enableDenormalsProtection();
};

#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

/**
 * Audio device service managing device setup and lifecycle
 */
class AudioDeviceService
{
public:
    AudioDeviceService(AudioEngine& engine);
    ~AudioDeviceService();
    
    juce::AudioDeviceManager& getDeviceManager() { return deviceManager; }
    
    bool startAudio();
    void stopAudio();
    bool isAudioRunning() const;
    
    juce::String getLastError() const { return lastError; }
    
    // Get latency information
    int getInputLatency() const;
    int getOutputLatency() const;
    double getTotalLatencyMs() const;
    
private:
    juce::AudioDeviceManager deviceManager;
    AudioEngine& audioEngine;
    juce::String lastError;
    
    void setupDefaultDevice();
};

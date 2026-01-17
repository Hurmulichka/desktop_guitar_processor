#pragma once

#include <JuceHeader.h>
#include "../audio/AudioDeviceService.h"

/**
 * Panel for audio device settings
 */
class AudioSettingsPanel : public juce::Component,
                          private juce::Timer
{
public:
    AudioSettingsPanel(AudioDeviceService& service);
    ~AudioSettingsPanel() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    void timerCallback() override;
    void showAudioSettings();
    
    AudioDeviceService& audioService;
    
    juce::TextButton settingsButton{"Audio Settings..."};
    juce::TextButton startStopButton{"Start Audio"};
    juce::Label statusLabel;
    juce::Label latencyLabel;
    
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSelector;
};

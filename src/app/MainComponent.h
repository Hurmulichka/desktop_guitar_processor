#pragma once

#include <JuceHeader.h>
#include "../audio/AudioDeviceService.h"
#include "../audio/AudioEngine.h"
#include "../audio/ParameterBridge.h"
#include "../ui/MeterComponent.h"
#include "../ui/AudioSettingsPanel.h"

/**
 * Main component of the application
 */
class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    // Audio system (order matters for initialization)
    ParameterBridge parameterBridge;
    AudioEngine audioEngine;
    AudioDeviceService audioService;
    
    // UI Components
    juce::Label titleLabel;
    
    juce::Slider inputGainSlider;
    juce::Label inputGainLabel;
    
    juce::Slider masterGainSlider;
    juce::Label masterGainLabel;
    
    juce::ToggleButton bypassButton;
    juce::TextButton panicButton;
    
    std::unique_ptr<MeterComponent> inputMeter;
    std::unique_ptr<MeterComponent> outputMeter;
    
    std::unique_ptr<AudioSettingsPanel> audioSettingsPanel;
    
    void setupControls();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

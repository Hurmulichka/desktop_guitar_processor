#include "MainComponent.h"

MainComponent::MainComponent()
    : audioEngine(parameterBridge),
      audioService(audioEngine)
{
    setSize(800, 600);
    setupControls();
}

MainComponent::~MainComponent()
{
    audioService.stopAudio();
}

void MainComponent::setupControls()
{
    // Title
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Desktop Guitar Processor - MVP", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    
    // Input Gain
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setRange(-24.0, 24.0, 0.1);
    inputGainSlider.setValue(0.0);
    inputGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    inputGainSlider.setTextValueSuffix(" dB");
    inputGainSlider.onValueChange = [this] {
        parameterBridge.setInputGain(static_cast<float>(inputGainSlider.getValue()));
    };
    
    addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);
    inputGainLabel.attachToComponent(&inputGainSlider, false);
    
    // Master Gain
    addAndMakeVisible(masterGainSlider);
    masterGainSlider.setRange(-24.0, 12.0, 0.1);
    masterGainSlider.setValue(0.0);
    masterGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    masterGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    masterGainSlider.setTextValueSuffix(" dB");
    masterGainSlider.onValueChange = [this] {
        parameterBridge.setMasterGain(static_cast<float>(masterGainSlider.getValue()));
    };
    
    addAndMakeVisible(masterGainLabel);
    masterGainLabel.setText("Master Gain", juce::dontSendNotification);
    masterGainLabel.setJustificationType(juce::Justification::centred);
    masterGainLabel.attachToComponent(&masterGainSlider, false);
    
    // Bypass
    addAndMakeVisible(bypassButton);
    bypassButton.setButtonText("Bypass");
    bypassButton.setToggleState(false, juce::dontSendNotification);
    bypassButton.onClick = [this] {
        parameterBridge.setBypass(bypassButton.getToggleState());
    };
    
    // Panic/Mute
    addAndMakeVisible(panicButton);
    panicButton.setButtonText("PANIC / MUTE");
    panicButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    panicButton.onClick = [this] {
        static bool muted = false;
        muted = !muted;
        parameterBridge.setMute(muted);
        panicButton.setButtonText(muted ? "UNMUTE" : "PANIC / MUTE");
        panicButton.setColour(juce::TextButton::buttonColourId, 
                             muted ? juce::Colours::red : juce::Colours::darkred);
    };
    
    // Meters
    inputMeter = std::make_unique<MeterComponent>(audioEngine.getInputMeter(), "Input");
    addAndMakeVisible(inputMeter.get());
    
    outputMeter = std::make_unique<MeterComponent>(audioEngine.getOutputMeter(), "Output");
    addAndMakeVisible(outputMeter.get());
    
    // Audio Settings Panel
    audioSettingsPanel = std::make_unique<AudioSettingsPanel>(audioService);
    addAndMakeVisible(audioSettingsPanel.get());
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(20);
    
    // Title
    titleLabel.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(10);
    
    // Audio settings panel at top
    audioSettingsPanel->setBounds(bounds.removeFromTop(150));
    bounds.removeFromTop(20);
    
    // Main controls area
    auto controlsArea = bounds;
    
    // Meters on left and right
    auto metersWidth = 60;
    inputMeter->setBounds(controlsArea.removeFromLeft(metersWidth));
    controlsArea.removeFromLeft(20);
    
    auto rightMeterArea = controlsArea.removeFromRight(metersWidth);
    controlsArea.removeFromRight(20);
    outputMeter->setBounds(rightMeterArea);
    
    // Center controls
    auto sliderWidth = 100;
    auto buttonHeight = 40;
    
    // Input gain on left
    auto inputArea = controlsArea.removeFromLeft(sliderWidth);
    inputArea.removeFromTop(30); // Space for label
    inputGainSlider.setBounds(inputArea.removeFromTop(200));
    
    controlsArea.removeFromLeft(40);
    
    // Master gain on right
    auto masterArea = controlsArea.removeFromLeft(sliderWidth);
    masterArea.removeFromTop(30); // Space for label
    masterGainSlider.setBounds(masterArea.removeFromTop(200));
    
    // Buttons in the middle
    auto middleArea = controlsArea;
    middleArea.removeFromTop(50);
    
    bypassButton.setBounds(middleArea.removeFromTop(buttonHeight).reduced(20, 0));
    middleArea.removeFromTop(20);
    
    panicButton.setBounds(middleArea.removeFromTop(buttonHeight).reduced(20, 0));
}

#include "AudioSettingsPanel.h"

AudioSettingsPanel::AudioSettingsPanel(AudioDeviceService& service)
    : audioService(service)
{
    addAndMakeVisible(settingsButton);
    settingsButton.onClick = [this] { showAudioSettings(); };
    
    addAndMakeVisible(startStopButton);
    startStopButton.onClick = [this] {
        if (audioService.isAudioRunning())
        {
            audioService.stopAudio();
            startStopButton.setButtonText("Start Audio");
        }
        else
        {
            if (audioService.startAudio())
            {
                startStopButton.setButtonText("Stop Audio");
            }
        }
    };
    
    addAndMakeVisible(statusLabel);
    statusLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(latencyLabel);
    latencyLabel.setJustificationType(juce::Justification::centred);
    
    // Start audio automatically
    if (audioService.startAudio())
    {
        startStopButton.setButtonText("Stop Audio");
    }
    
    startTimerHz(2); // Update status 2 times per second
}

AudioSettingsPanel::~AudioSettingsPanel()
{
    stopTimer();
    audioSelector.reset();
}

void AudioSettingsPanel::showAudioSettings()
{
    if (audioSelector == nullptr)
    {
        audioSelector = std::make_unique<juce::AudioDeviceSelectorComponent>(
            audioService.getDeviceManager(),
            1, 1,  // min/max input channels
            2, 2,  // min/max output channels
            true,  // show MIDI input options
            false, // show MIDI output options
            false, // treat channels as stereo pairs
            false  // hide advanced options
        );
        
        audioSelector->setSize(500, 400);
    }
    
    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(audioSelector.release());
    options.dialogTitle = "Audio Settings";
    options.dialogBackgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = true;
    options.resizable = false;
    
    auto* window = options.launchAsync();
    if (window != nullptr)
    {
        window->centreWithSize(500, 400);
    }
}

void AudioSettingsPanel::timerCallback()
{
    // Update status
    auto* device = audioService.getDeviceManager().getCurrentAudioDevice();
    if (device != nullptr && device->isOpen())
    {
        juce::String status = device->getName() + " @ " + 
                             juce::String(device->getCurrentSampleRate(), 0) + " Hz, " +
                             juce::String(device->getCurrentBufferSizeSamples()) + " samples";
        statusLabel.setText(status, juce::dontSendNotification);
        
        double latencyMs = audioService.getTotalLatencyMs();
        latencyLabel.setText("Latency: " + juce::String(latencyMs, 1) + " ms", 
                            juce::dontSendNotification);
    }
    else
    {
        statusLabel.setText("No audio device", juce::dontSendNotification);
        latencyLabel.setText("", juce::dontSendNotification);
    }
}

void AudioSettingsPanel::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
}

void AudioSettingsPanel::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    settingsButton.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);
    
    startStopButton.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);
    
    statusLabel.setBounds(bounds.removeFromTop(20));
    bounds.removeFromTop(5);
    
    latencyLabel.setBounds(bounds.removeFromTop(20));
}

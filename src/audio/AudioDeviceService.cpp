#include "AudioDeviceService.h"

AudioDeviceService::AudioDeviceService(AudioEngine& engine)
    : audioEngine(engine)
{
    setupDefaultDevice();
}

AudioDeviceService::~AudioDeviceService()
{
    stopAudio();
}

void AudioDeviceService::setupDefaultDevice()
{
    // Initialize with mono input and stereo output
    auto result = deviceManager.initialiseWithDefaultDevices(1, 2);
    
    if (!result.isEmpty())
    {
        lastError = "Failed to initialize audio: " + result;
        juce::Logger::writeToLog(lastError);
    }
}

bool AudioDeviceService::startAudio()
{
    if (isAudioRunning())
    {
        return true;
    }
    
    deviceManager.addAudioCallback(&audioEngine);
    
    auto* device = deviceManager.getCurrentAudioDevice();
    if (device == nullptr)
    {
        lastError = "No audio device available";
        return false;
    }
    
    lastError.clear();
    return true;
}

void AudioDeviceService::stopAudio()
{
    deviceManager.removeAudioCallback(&audioEngine);
}

bool AudioDeviceService::isAudioRunning() const
{
    auto* device = deviceManager.getCurrentAudioDevice();
    return device != nullptr && device->isOpen();
}

int AudioDeviceService::getInputLatency() const
{
    auto* device = deviceManager.getCurrentAudioDevice();
    return device != nullptr ? device->getInputLatencyInSamples() : 0;
}

int AudioDeviceService::getOutputLatency() const
{
    auto* device = deviceManager.getCurrentAudioDevice();
    return device != nullptr ? device->getOutputLatencyInSamples() : 0;
}

double AudioDeviceService::getTotalLatencyMs() const
{
    auto* device = deviceManager.getCurrentAudioDevice();
    if (device == nullptr)
        return 0.0;
        
    double sampleRate = device->getCurrentSampleRate();
    if (sampleRate <= 0.0)
        return 0.0;
        
    int totalLatencySamples = getInputLatency() + getOutputLatency();
    return (totalLatencySamples / sampleRate) * 1000.0;
}

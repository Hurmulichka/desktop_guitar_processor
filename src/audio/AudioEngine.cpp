#include "AudioEngine.h"
#include <cmath>

#ifdef _MSC_VER
    #include <xmmintrin.h>
#endif

AudioEngine::AudioEngine(ParameterBridge& paramBridge)
    : parameters(paramBridge)
{
}

AudioEngine::~AudioEngine()
{
}

void AudioEngine::enableDenormalsProtection()
{
#ifdef _MSC_VER
    // Enable FTZ (Flush-To-Zero) and DAZ (Denormals-Are-Zero) on Windows
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif
}

void AudioEngine::audioDeviceAboutToStart(juce::AudioIODevice* device)
{
    if (device == nullptr)
        return;
        
    double sampleRate = device->getCurrentSampleRate();
    
    // Prepare DSP components
    inputGain.prepare(sampleRate);
    masterGain.prepare(sampleRate);
    outputLimiter.prepare(sampleRate);
    outputLimiter.setThreshold(-0.5); // -0.5 dB threshold
    
    inputMeter.prepare(sampleRate);
    outputMeter.prepare(sampleRate);
    
    // Bypass crossfade smoothing (20ms)
    bypassSmoothCoeff = std::exp(-1.0 / (sampleRate * 0.02));
    bypassCrossfade = parameters.isBypassed() ? 0.0 : 1.0;
    
    enableDenormalsProtection();
}

void AudioEngine::audioDeviceStopped()
{
    inputMeter.reset();
    outputMeter.reset();
}

void AudioEngine::audioDeviceError(const juce::String& errorMessage)
{
    // Log error to console (not in audio thread)
    juce::Logger::writeToLog("Audio device error: " + errorMessage);
}

void AudioEngine::audioDeviceIOCallbackWithContext(
    const float* const* inputChannelData,
    int numInputChannels,
    float* const* outputChannelData,
    int numOutputChannels,
    int numSamples,
    const juce::AudioIODeviceCallbackContext& /*context*/)
{
    enableDenormalsProtection();
    
    // Read parameters (lock-free)
    inputGain.setGainDecibels(parameters.getInputGain());
    masterGain.setGainDecibels(parameters.getMasterGain());
    bool muted = parameters.isMuted();
    bool bypassed = parameters.isBypassed();
    
    // Smooth bypass crossfade
    double targetCrossfade = bypassed ? 0.0 : 1.0;
    
    // Clear outputs first
    for (int ch = 0; ch < numOutputChannels; ++ch)
    {
        if (outputChannelData[ch] != nullptr)
        {
            std::fill_n(outputChannelData[ch], numSamples, 0.0f);
        }
    }
    
    // Process samples
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Read input (mono)
        double inputSample = 0.0;
        if (numInputChannels > 0 && inputChannelData[0] != nullptr)
        {
            inputSample = static_cast<double>(inputChannelData[0][sample]);
        }
        
        // Measure input
        inputMeter.processSample(inputSample);
        
        double drySample = inputSample;
        double wetSample = inputSample;
        
        if (!muted)
        {
            // Apply input gain
            wetSample = inputGain.processMonoSample(wetSample);
            
            // Apply master gain
            wetSample = masterGain.processMonoSample(wetSample);
            
            // Apply output limiter
            wetSample = outputLimiter.processSample(wetSample);
        }
        else
        {
            wetSample = 0.0;
        }
        
        // Smooth bypass crossfade
        bypassCrossfade = targetCrossfade + bypassSmoothCoeff * (bypassCrossfade - targetCrossfade);
        
        // Mix dry/wet with crossfade
        double outputSample = drySample * (1.0 - bypassCrossfade) + wetSample * bypassCrossfade;
        
        // Measure output
        outputMeter.processSample(outputSample);
        
        // Write to stereo output (duplicate mono to stereo)
        for (int ch = 0; ch < numOutputChannels; ++ch)
        {
            if (outputChannelData[ch] != nullptr)
            {
                outputChannelData[ch][sample] = static_cast<float>(outputSample);
            }
        }
    }
}

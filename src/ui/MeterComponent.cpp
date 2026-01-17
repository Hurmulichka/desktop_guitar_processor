#include "MeterComponent.h"

MeterComponent::MeterComponent(const Meter& meter, const juce::String& label)
    : meterSource(meter), labelText(label)
{
    startTimerHz(30); // Update at 30 Hz
}

MeterComponent::~MeterComponent()
{
    stopTimer();
}

void MeterComponent::timerCallback()
{
    displayPeak = meterSource.getPeakLevel();
    displayRMS = meterSource.getRMSLevel();
    
    // Decay for visual effect
    const_cast<Meter&>(meterSource).decayPeak(DECAY_RATE);
    
    repaint();
}

void MeterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Draw background
    g.setColour(juce::Colours::darkgrey);
    g.fillRoundedRectangle(bounds.toFloat(), 4.0f);
    
    // Draw label
    auto labelArea = bounds.removeFromTop(20);
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText(labelText, labelArea, juce::Justification::centred);
    
    // Draw meter bar
    bounds.reduce(4, 4);
    auto meterArea = bounds.reduced(2);
    
    if (meterArea.getHeight() > 0)
    {
        // Convert to dB for display
        float peakDb = displayPeak > 0.0001f ? 20.0f * std::log10(displayPeak) : -100.0f;
        float rmsDb = displayRMS > 0.0001f ? 20.0f * std::log10(displayRMS) : -100.0f;
        
        // Map dB to height (-60dB to 0dB)
        auto mapDbToHeight = [&](float db) {
            float normalized = juce::jmap(db, -60.0f, 0.0f, 0.0f, 1.0f);
            return juce::jlimit(0.0f, 1.0f, normalized);
        };
        
        float peakHeight = mapDbToHeight(peakDb);
        float rmsHeight = mapDbToHeight(rmsDb);
        
        // Draw RMS level (darker green)
        if (rmsHeight > 0.0f)
        {
            auto rmsRect = meterArea.removeFromBottom(static_cast<int>(meterArea.getHeight() * rmsHeight));
            g.setColour(juce::Colours::darkgreen);
            g.fillRect(rmsRect);
        }
        
        // Draw peak level (bright green, or red if clipping)
        if (peakHeight > 0.0f)
        {
            auto peakRect = meterArea.removeFromBottom(static_cast<int>(meterArea.getHeight() * peakHeight));
            
            if (displayPeak > 0.95f)
                g.setColour(juce::Colours::red);
            else if (displayPeak > 0.8f)
                g.setColour(juce::Colours::orange);
            else
                g.setColour(juce::Colours::green);
                
            g.fillRect(peakRect);
        }
    }
    
    // Draw dB scale markers
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    for (float db : { 0.0f, -6.0f, -12.0f, -24.0f, -48.0f })
    {
        float normalized = juce::jmap(db, -60.0f, 0.0f, 0.0f, 1.0f);
        int y = meterArea.getBottom() - static_cast<int>(meterArea.getHeight() * normalized);
        g.drawHorizontalLine(y, static_cast<float>(meterArea.getX()), static_cast<float>(meterArea.getRight()));
    }
}

void MeterComponent::resized()
{
}

#pragma once

#include <JuceHeader.h>
#include "MainComponent.h"

/**
 * Main application window
 */
class MainWindow : public juce::DocumentWindow
{
public:
    MainWindow(juce::String name);
    ~MainWindow() override;
    
    void closeButtonPressed() override;
    
private:
    std::unique_ptr<MainComponent> mainComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

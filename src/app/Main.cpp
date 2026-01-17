#include <JuceHeader.h>
#include "MainWindow.h"

/**
 * Desktop Guitar Processor Application
 * Stage A (MVP): Basic passthrough with audio device management
 */
class DesktopGuitarProcessorApplication : public juce::JUCEApplication
{
public:
    DesktopGuitarProcessorApplication() {}
    
    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return false; }
    
    void initialise(const juce::String& /*commandLine*/) override
    {
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }
    
    void shutdown() override
    {
        mainWindow.reset();
    }
    
    void systemRequestedQuit() override
    {
        quit();
    }
    
    void anotherInstanceStarted(const juce::String& /*commandLine*/) override
    {
    }
    
private:
    std::unique_ptr<MainWindow> mainWindow;
};

// This macro generates the main() routine that launches the app
START_JUCE_APPLICATION(DesktopGuitarProcessorApplication)

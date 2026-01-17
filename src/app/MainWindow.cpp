#include "MainWindow.h"

MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
                    juce::Desktop::getInstance().getDefaultLookAndFeel()
                        .findColour(juce::ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    mainComponent = std::make_unique<MainComponent>();
    setContentOwned(mainComponent.release(), true);
    
    #if JUCE_IOS || JUCE_ANDROID
        setFullScreen(true);
    #else
        setResizable(true, true);
        centreWithSize(getWidth(), getHeight());
    #endif
    
    setVisible(true);
}

MainWindow::~MainWindow()
{
    mainComponent.reset();
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

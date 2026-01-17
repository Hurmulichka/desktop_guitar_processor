cmake_minimum_required(VERSION 3.15)

project(DesktopGuitarProcessor VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE subdirectory
add_subdirectory(JUCE)

# Create the standalone application
juce_add_gui_app(DesktopGuitarProcessor
    PRODUCT_NAME "Desktop Guitar Processor"
    COMPANY_NAME "GuitarProcessor"
    BUNDLE_ID "com.guitarprocessor.desktop"
    VERSION "0.1.0"
)

# Add source files
target_sources(DesktopGuitarProcessor PRIVATE
    # App
    src/app/Main.cpp
    src/app/MainWindow.h
    src/app/MainWindow.cpp
    src/app/MainComponent.h
    src/app/MainComponent.cpp
    
    # Audio
    src/audio/AudioDeviceService.h
    src/audio/AudioDeviceService.cpp
    src/audio/AudioEngine.h
    src/audio/AudioEngine.cpp
    src/audio/ParameterBridge.h
    src/audio/ParameterBridge.cpp
    
    # DSP
    src/dsp/Gain.h
    src/dsp/Gain.cpp
    src/dsp/Meter.h
    src/dsp/Meter.cpp
    src/dsp/SoftLimiter.h
    src/dsp/SoftLimiter.cpp
    src/dsp/Smoother.h
    
    # UI
    src/ui/MeterComponent.h
    src/ui/MeterComponent.cpp
    src/ui/AudioSettingsPanel.h
    src/ui/AudioSettingsPanel.cpp
)

# Add include directories
target_include_directories(DesktopGuitarProcessor PRIVATE
    src
)

# Link required JUCE modules
target_link_libraries(DesktopGuitarProcessor PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Compiler definitions
target_compile_definitions(DesktopGuitarProcessor PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:DesktopGuitarProcessor,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:DesktopGuitarProcessor,JUCE_VERSION>"
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_USE_DARK_SPLASH_SCREEN=1
)

# Windows-specific settings
if(WIN32)
    target_compile_definitions(DesktopGuitarProcessor PRIVATE
        _WINDOWS=1
        JUCE_WASAPI=1
        JUCE_ASIO=1
    )
endif()

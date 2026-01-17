"# Desktop Guitar Processor

A real-time guitar/audio processor application built with C++ and JUCE framework.

## Project Status

**Current Stage: A (MVP - "звук пошёл")**

This is the first development increment providing basic audio passthrough functionality with device management and real-time safety.

## Features

### Stage A (MVP) - Completed
- ✅ Windows Standalone application
- ✅ Audio device management (ASIO/WASAPI support)
- ✅ Configurable sample rate and buffer size
- ✅ Basic DSP chain: Input Gain → Bypass → Master Gain → Output Limiter
- ✅ Mono input to stereo output passthrough
- ✅ Visual peak and RMS meters
- ✅ Real-time safe audio processing (no allocations/locks/I/O in callback)
- ✅ Parameter smoothing (no zipper noise)
- ✅ Crossfade bypass (no clicks)
- ✅ Panic/Mute button
- ✅ Output soft limiter (prevents clipping)
- ✅ Latency display

## Quick Start

### Building on Windows

#### Prerequisites
- Visual Studio 2019 or later (with C++ Desktop Development)
- CMake 3.15+
- Git

#### Build Instructions

```bash
# Clone with submodules
git clone --recursive https://github.com/Hurmulichka/desktop_guitar_processor.git
cd desktop_guitar_processor

# If already cloned, initialize submodules
git submodule update --init --recursive

# Generate and build
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release

# Run
Release\DesktopGuitarProcessor.exe
```

### Quick Test

1. Launch the application
2. Click "Audio Settings..." to configure your audio interface
3. Select ASIO driver (if available) or WASAPI
4. Set sample rate to 48000 Hz
5. Set buffer size to 128 samples
6. Connect guitar to input, monitors to output
7. Test input/master gain controls and bypass

For detailed testing instructions, see [Quick Start Guide](docs/quick-start.md).

## Project Structure

```
desktop_guitar_processor/
├── src/
│   ├── app/          # Application entry point, main window, main component
│   ├── audio/        # Audio device service, engine, parameter bridge
│   ├── dsp/          # DSP components (gain, meters, limiter, smoother)
│   └── ui/           # UI components (meters, settings panel)
├── docs/             # Documentation
├── JUCE/             # JUCE framework (submodule)
└── CMakeLists.txt    # Build configuration
```

## Technical Details

### Real-time Safety
- No memory allocations in audio callback
- Lock-free parameter communication using atomics
- No mutex, I/O, or logging in audio thread
- Denormals protection enabled

### DSP Chain
```
Input (Mono) → Input Gain → Bypass Crossfade → Master Gain → Soft Limiter → Output (Stereo)
                    ↓                                    ↓
              Input Meter                          Output Meter
```

### Thread Communication
- UI thread ← atomics → Audio thread
- Meters use atomics for lock-free level reporting
- ParameterBridge uses atomics for parameter updates

## Documentation

- [Quick Start Guide](docs/quick-start.md) - Building, testing, and troubleshooting

## Roadmap

- **Stage A (MVP)** ✅ - Basic audio passthrough
- **Stage B** - Amp simulation and cabinet IR
- **Stage C** - Effects chain (delay, reverb, chorus)
- **Stage D** - Preset management
- **Stage E** - UI improvements and optimization

## License

[Add your license here]

## Contributing

[Add contributing guidelines here]" 

# Desktop Guitar Processor - Quick Start Guide

## Stage A (MVP) - Basic Audio Passthrough

This is the first increment implementing basic audio passthrough functionality with device management.

## Features

- **Audio Device Management**: Select ASIO/WASAPI driver, device, sample rate, and buffer size
- **Basic DSP Chain**: Input Gain → Bypass → Master Gain → Output Limiter
- **Mono to Stereo**: Mono input is duplicated to stereo output
- **Visual Metering**: Peak and RMS meters for input and output
- **Real-time Safety**: No allocations, mutexes, or I/O in audio callback
- **Output Protection**: Soft limiter prevents digital clipping
- **Parameter Smoothing**: Prevents zipper noise on gain changes
- **Bypass Crossfade**: Smooth transition between bypass and processing
- **Panic/Mute**: Emergency mute button

## Building the Project

### Windows (MSVC)

#### Prerequisites
- Visual Studio 2019 or later with C++ Desktop Development workload
- CMake 3.15 or later
- Git

#### Build Steps

```bash
# Clone the repository with submodules
git clone --recursive https://github.com/Hurmulichka/desktop_guitar_processor.git
cd desktop_guitar_processor

# If you already cloned, initialize submodules
git submodule update --init --recursive

# Create build directory
mkdir build
cd build

# Generate Visual Studio project
cmake .. -G "Visual Studio 16 2019" -A x64

# Build the project
cmake --build . --config Release

# Run the application
Release\DesktopGuitarProcessor.exe
```

Or open the generated `.sln` file in Visual Studio and build from there.

## Testing the Application

### Basic Functionality Test

1. **Launch the application**
   - The audio device should initialize automatically
   - Default device settings will be used if available

2. **Configure Audio Device**
   - Click "Audio Settings..." to open device configuration
   - Select your audio interface (prefer ASIO for low latency)
   - Choose sample rate: 44100 or 48000 Hz recommended
   - Set buffer size: 128 or 256 samples for good latency
   - Click OK to apply settings

3. **Test Passthrough**
   - Connect a guitar or audio source to input
   - Connect headphones or monitors to output
   - You should hear your input signal

4. **Test Input Gain**
   - Move the "Input Gain" slider
   - Verify smooth gain changes without clicking (zipper noise)
   - Watch the input meter respond

5. **Test Master Gain**
   - Move the "Master Gain" slider
   - Verify smooth gain changes
   - Watch the output meter respond

6. **Test Bypass**
   - Click the "Bypass" toggle button
   - Should smoothly crossfade between dry and processed signal
   - No clicks or pops during bypass

7. **Test Panic/Mute**
   - Click "PANIC / MUTE" button
   - Output should mute immediately
   - Click again to unmute

8. **Test Output Limiter**
   - Increase Input Gain and Master Gain to high values
   - Play loud signal
   - Output meter should not clip (stay below 0 dB)
   - No digital distortion should occur

### Performance Test

Test at 128 samples buffer size at 48 kHz:
- CPU usage should be low (<5% on modern CPUs)
- No audio dropouts or glitches
- Latency display should show reasonable values (5-10ms typical)

### Latency Check

- The latency information is displayed in the Audio Settings panel
- Input + Output latency should be displayed in milliseconds
- For 128 samples at 48 kHz: ~2.7ms buffer + driver latency

## Troubleshooting

### No Audio Device Available
- Check that your audio interface is connected
- Try reopening the application
- Check Windows audio settings

### Audio Dropouts/Glitches
- Increase buffer size (try 256 or 512 samples)
- Close other applications using audio
- Check CPU usage
- Update audio driver

### High Latency
- Use ASIO driver if available (instead of WASAPI)
- Decrease buffer size (but not below 64 samples)
- Check for background processes

### No Input Signal
- Check input connections
- Verify correct input device selected in Audio Settings
- Check input gain is not at minimum
- Verify input is not muted in system settings

## Known Limitations (Stage A)

- No effects processing (amp sim, reverb, delay, etc.)
- No preset management
- Basic UI (no custom skinning)
- Mono input only (stereo input uses first channel)
- No MIDI support yet

## Architecture Notes

### Real-time Safety
The audio callback (`AudioEngine::audioDeviceIOCallbackWithContext`) is real-time safe:
- No memory allocations
- No mutex locks
- No file I/O
- No logging
- Denormals protection enabled

### Thread Communication
Parameters are communicated between UI and audio thread using atomics (lock-free):
- `ParameterBridge` uses `std::atomic` for thread-safe communication
- Meters use atomics for level reporting

### DSP Chain
```
Input (Mono) → Input Meter → Input Gain → Bypass Crossfade → Master Gain → Soft Limiter → Output Meter → Output (Stereo)
```

## Next Steps

This is Stage A (MVP). Future stages will add:
- Amp simulation
- Cabinet IR loading
- Effects (delay, reverb, chorus, etc.)
- Preset management
- Better UI/graphics
- Performance optimizations

# Build Instructions

## Windows Build (Primary Target)

### Prerequisites

1. **Visual Studio 2019 or later**
   - Install the "Desktop development with C++" workload
   - Ensure C++ CMake tools for Windows are installed
   - Windows 10 SDK (included with Visual Studio)

2. **CMake 3.15 or later**
   - Download from https://cmake.org/download/
   - Or install via Visual Studio installer

3. **Git**
   - Download from https://git-scm.com/download/win
   - Or use Git for Windows

### Option 1: Visual Studio IDE

```bash
# Clone with submodules
git clone --recursive https://github.com/Hurmulichka/desktop_guitar_processor.git
cd desktop_guitar_processor

# Generate Visual Studio solution
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64

# This creates DesktopGuitarProcessor.sln
# Open in Visual Studio and build
```

Then in Visual Studio:
1. Open `build/DesktopGuitarProcessor.sln`
2. Set build configuration to `Release` (or `Debug`)
3. Build → Build Solution (Ctrl+Shift+B)
4. Run → Start Without Debugging (Ctrl+F5)

### Option 2: Command Line Build

```bash
# Clone with submodules
git clone --recursive https://github.com/Hurmulichka/desktop_guitar_processor.git
cd desktop_guitar_processor

# Generate and build in one go
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release

# Run the application
Release\DesktopGuitarProcessor.exe
```

### Build Configurations

- **Debug**: Includes debug symbols, assertions enabled, no optimization
  ```bash
  cmake --build . --config Debug
  ```

- **Release**: Optimized, no debug symbols, assertions disabled
  ```bash
  cmake --build . --config Release
  ```

- **RelWithDebInfo**: Optimized with debug symbols
  ```bash
  cmake --build . --config RelWithDebInfo
  ```

### Troubleshooting Windows Build

#### CMake doesn't find Visual Studio
```bash
# List available generators
cmake --help

# Explicitly specify Visual Studio version
cmake .. -G "Visual Studio 17 2022" -A x64  # For VS 2022
cmake .. -G "Visual Studio 16 2019" -A x64  # For VS 2019
cmake .. -G "Visual Studio 15 2017" -A x64  # For VS 2017
```

#### Git submodule not initialized
```bash
git submodule update --init --recursive
```

#### Build fails with missing dependencies
- Ensure Visual Studio has C++ Desktop Development workload installed
- Ensure Windows 10 SDK is installed
- Run Visual Studio Installer and verify/repair installation

#### ASIO SDK (Optional)
For ASIO support, JUCE includes a fallback. For production builds, you may want to download the ASIO SDK:
1. Download from Steinberg website
2. Place in `JUCE/modules/juce_audio_devices/native/asiosdk`
3. Rebuild project

## Linux Build (Experimental/Not Primary Target)

The project is primarily designed for Windows, but can be built on Linux for development purposes.

### Prerequisites (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    libasound2-dev \
    libjack-jackd2-dev \
    libfreetype6-dev \
    libx11-dev \
    libxcomposite-dev \
    libxcursor-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxrender-dev \
    mesa-common-dev \
    libgl1-mesa-dev \
    libcurl4-openssl-dev \
    libwebkit2gtk-4.0-dev \
    libgtk-3-dev
```

### Build Steps

```bash
git clone --recursive https://github.com/Hurmulichka/desktop_guitar_processor.git
cd desktop_guitar_processor
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
./DesktopGuitarProcessor_artefacts/Release/DesktopGuitarProcessor
```

## macOS Build (Experimental/Not Primary Target)

### Prerequisites

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (via Homebrew)
brew install cmake
```

### Build Steps

```bash
git clone --recursive https://github.com/Hurmulichka/desktop_guitar_processor.git
cd desktop_guitar_processor
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(sysctl -n hw.ncpu)
open DesktopGuitarProcessor_artefacts/Release/DesktopGuitarProcessor.app
```

## Continuous Integration

For CI builds, see `.github/workflows/` directory (if present).

## Build Output

After a successful build, you'll find:

```
build/
├── DesktopGuitarProcessor_artefacts/
│   ├── Debug/
│   │   └── DesktopGuitarProcessor.exe (Debug build)
│   └── Release/
│       └── DesktopGuitarProcessor.exe (Release build)
```

Or for Visual Studio builds directly:

```
build/
├── Debug/
│   └── DesktopGuitarProcessor.exe
└── Release/
    └── DesktopGuitarProcessor.exe
```

## Performance Notes

- **Release builds are strongly recommended** for actual audio processing
- Debug builds have significant performance overhead
- For lowest latency, compile with Release or RelWithDebInfo
- CPU usage: Should be <5% on modern CPUs with 128 sample buffer

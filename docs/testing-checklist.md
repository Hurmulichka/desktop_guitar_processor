# Testing Checklist - Stage A (MVP)

This document provides a comprehensive testing checklist for verifying the Stage A implementation.

## Pre-Testing Requirements

- [ ] Application builds successfully without errors
- [ ] Application launches without crashing
- [ ] Audio interface is connected and recognized by Windows

## 1. Basic Functionality Tests

### 1.1 Application Launch
- [ ] Application window opens
- [ ] All UI elements are visible (title, sliders, buttons, meters, settings panel)
- [ ] No error messages on startup
- [ ] Audio automatically initializes (or shows appropriate message if no device)

### 1.2 Audio Device Initialization
- [ ] Status label shows current audio device name
- [ ] Sample rate is displayed correctly
- [ ] Buffer size is displayed correctly
- [ ] Latency value is displayed (should be > 0 ms)
- [ ] Start/Stop button shows "Stop Audio" if running

## 2. Audio Settings Tests

### 2.1 Audio Settings Dialog
- [ ] Click "Audio Settings..." button opens settings dialog
- [ ] Dialog shows available audio device types (ASIO, WASAPI, etc.)
- [ ] Available input/output devices are listed
- [ ] Sample rate options are available (44100, 48000, 96000, etc.)
- [ ] Buffer size options are available (64, 128, 256, 512, 1024, 2048)
- [ ] Settings can be changed and applied
- [ ] Dialog closes properly

### 2.2 Device Selection
- [ ] ASIO devices are shown (if ASIO drivers installed)
- [ ] WASAPI devices are shown
- [ ] Changing device updates status label
- [ ] No crash when changing devices
- [ ] Audio restarts properly after device change

### 2.3 Sample Rate & Buffer Size
- [ ] Change sample rate to 44100 Hz → status updates
- [ ] Change sample rate to 48000 Hz → status updates
- [ ] Change buffer size to 64 samples → status updates, latency changes
- [ ] Change buffer size to 128 samples → status updates, latency changes
- [ ] Change buffer size to 256 samples → status updates, latency changes
- [ ] Change buffer size to 512 samples → status updates, latency changes
- [ ] Latency increases with larger buffer sizes
- [ ] No audio glitches when changing settings

## 3. Audio Passthrough Tests

### 3.1 Basic Passthrough
**Setup**: Connect guitar/audio source to input, monitors/headphones to output

- [ ] Audio signal passes through (hear input at output)
- [ ] Mono input is duplicated to both stereo channels
- [ ] Passthrough works at 44100 Hz
- [ ] Passthrough works at 48000 Hz
- [ ] Passthrough works at 96000 Hz (if supported)

### 3.2 Passthrough Stability
**Test at 48kHz, 128 samples**

- [ ] No dropouts during 1 minute of continuous playback
- [ ] No clicks or pops during normal operation
- [ ] No digital artifacts or noise
- [ ] CPU usage remains low (<5% on modern CPUs)
- [ ] No glitches when moving mouse or typing
- [ ] Stable during sustained notes (no wandering pitch)

## 4. Parameter Control Tests

### 4.1 Input Gain
- [ ] Slider moves smoothly
- [ ] Default position is 0 dB
- [ ] Range is -24 dB to +24 dB
- [ ] Display shows current value with "dB" suffix
- [ ] Audio level changes when moving slider
- [ ] **NO zipper noise when adjusting gain**
- [ ] **Smooth transitions even with fast movements**
- [ ] Input meter responds to gain changes
- [ ] Gain changes apply in real-time

### 4.2 Master Gain
- [ ] Slider moves smoothly
- [ ] Default position is 0 dB
- [ ] Range is -24 dB to +12 dB
- [ ] Display shows current value with "dB" suffix
- [ ] Audio level changes when moving slider
- [ ] **NO zipper noise when adjusting gain**
- [ ] **Smooth transitions even with fast movements**
- [ ] Output meter responds to gain changes
- [ ] Gain changes apply in real-time

### 4.3 Bypass
- [ ] Toggle button works (on/off states visible)
- [ ] When bypassed: dry signal passes through
- [ ] When active: processed signal passes through
- [ ] **NO clicks or pops when toggling bypass**
- [ ] **Smooth crossfade between states**
- [ ] Can toggle during playback without artifacts
- [ ] Works correctly with sustained notes

### 4.4 Panic/Mute Button
- [ ] Button labeled "PANIC / MUTE"
- [ ] Button is red/dark red color
- [ ] Clicking button immediately mutes output
- [ ] Button text changes to "UNMUTE"
- [ ] Button color changes when muted
- [ ] Clicking again unmutes
- [ ] **Muting is immediate (no delay)**
- [ ] **No clicks or pops when muting/unmuting**
- [ ] Works as emergency stop

## 5. Visual Metering Tests

### 5.1 Input Meter
- [ ] Meter is visible on left side
- [ ] Labeled "Input"
- [ ] Responds to input signal
- [ ] Shows peak level (bright green)
- [ ] Shows RMS level (darker green)
- [ ] Peak level higher than RMS level
- [ ] Meter decays smoothly when signal stops
- [ ] Updates at reasonable rate (30 Hz, smooth animation)
- [ ] Scale markers visible (-48, -24, -12, -6, 0 dB)
- [ ] Meter turns orange/red when approaching clip
- [ ] Meter doesn't exceed 0 dB mark

### 5.2 Output Meter
- [ ] Meter is visible on right side
- [ ] Labeled "Output"
- [ ] Responds to output signal
- [ ] Shows peak level (bright green)
- [ ] Shows RMS level (darker green)
- [ ] Peak level higher than RMS level
- [ ] Meter decays smoothly when signal stops
- [ ] Updates at reasonable rate (30 Hz, smooth animation)
- [ ] Scale markers visible (-48, -24, -12, -6, 0 dB)
- [ ] Meter turns orange/red when approaching clip
- [ ] Meter doesn't exceed 0 dB mark (due to limiter)

## 6. Output Protection Tests

### 6.1 Soft Limiter
**Setup**: Set both Input Gain and Master Gain to maximum (+24 dB and +12 dB)

- [ ] Play loud guitar signal (power chords, high gain)
- [ ] Output meter approaches but doesn't exceed 0 dB
- [ ] No digital clipping/distortion heard
- [ ] Sound is limited but not harshly clipped
- [ ] Limiter engages smoothly (soft knee)
- [ ] No pumping or artifacts
- [ ] Lower gain returns to normal operation

### 6.2 Extreme Signal Test
- [ ] Set Input Gain to +24 dB
- [ ] Set Master Gain to +12 dB
- [ ] Play maximum volume input
- [ ] **Output never clips (no digital distortion)**
- [ ] Soft clipping sounds musical (not harsh)
- [ ] System remains stable

## 7. Real-time Safety Verification

### 7.1 Performance Under Load
- [ ] Open Task Manager / Resource Monitor
- [ ] Monitor CPU usage during playback
- [ ] CPU usage stays low (<5% typical)
- [ ] No CPU spikes during parameter changes
- [ ] No memory leaks (memory usage stable)
- [ ] Application responsive during audio processing

### 7.2 Stress Testing
- [ ] Move all sliders continuously for 30 seconds → no dropouts
- [ ] Toggle bypass rapidly → no crashes
- [ ] Change buffer size during playback → handles gracefully
- [ ] Open/close settings dialog repeatedly → no issues
- [ ] Minimize/restore window during playback → audio continues

## 8. Error Handling Tests

### 8.1 No Audio Device
- [ ] Disconnect audio interface
- [ ] Restart application
- [ ] Shows appropriate error message
- [ ] Application doesn't crash
- [ ] Reconnect interface → can recover

### 8.2 Device Disconnection During Operation
- [ ] Start audio playback
- [ ] Disconnect audio interface mid-playback
- [ ] Application handles gracefully (no crash)
- [ ] Shows error or status update
- [ ] Reconnect → can restart audio

### 8.3 Unsupported Configuration
- [ ] Try to set unrealistic buffer size (if possible)
- [ ] Try to set unsupported sample rate
- [ ] Application handles or prevents invalid settings
- [ ] No crash with invalid configurations

## 9. Cross-Platform Checks (Windows Specific)

### 9.1 Windows 10
- [ ] Application runs on Windows 10
- [ ] WASAPI works
- [ ] ASIO works (if installed)
- [ ] All features functional

### 9.2 Windows 11
- [ ] Application runs on Windows 11
- [ ] WASAPI works
- [ ] ASIO works (if installed)
- [ ] All features functional

## 10. Latency Verification

### 10.1 Latency Display
- [ ] Latency shown in milliseconds
- [ ] Value is reasonable (typically 5-15 ms)
- [ ] Latency decreases with smaller buffer sizes
- [ ] Latency increases with larger buffer sizes

### 10.2 Actual Latency Feel
- [ ] At 128 samples: latency is imperceptible for playing
- [ ] At 256 samples: latency is acceptable
- [ ] At 512 samples: latency is noticeable but usable
- [ ] ASIO has lower latency than WASAPI (if available)

## Acceptance Criteria (Must Pass All)

✅ **Critical Requirements**:
- [ ] Application builds and launches successfully
- [ ] Passthrough is stable at 128 samples, 48 kHz for 5 minutes
- [ ] Input Gain changes produce NO zipper noise
- [ ] Master Gain changes produce NO zipper noise
- [ ] Bypass toggle produces NO clicks (smooth crossfade)
- [ ] Panic/Mute works instantly without artifacts
- [ ] Output limiter prevents digital clipping
- [ ] No allocations/mutex/IO/logs in audio callback (code review confirms)
- [ ] Meters display levels accurately
- [ ] Audio settings can be changed without crashes

## Notes

- Test with real guitar signal (varying dynamics)
- Test with sustained notes (to catch instability)
- Test with percussive sounds (to catch clicks/pops)
- Monitor CPU and memory usage
- Test for at least 5 minutes per configuration
- Document any issues found

## Test Environment

- Windows Version: __________
- CPU: __________
- RAM: __________
- Audio Interface: __________
- ASIO/WASAPI: __________
- Sample Rate: __________
- Buffer Size: __________
- Build Config: Debug / Release

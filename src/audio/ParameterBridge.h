#pragma once

#include <atomic>

/**
 * Thread-safe parameter bridge between UI and audio threads
 * Uses atomics for lock-free communication
 */
class ParameterBridge
{
public:
    ParameterBridge() = default;
    
    // UI thread sets parameters
    void setInputGain(float gainDb) { inputGain.store(gainDb, std::memory_order_relaxed); }
    void setMasterGain(float gainDb) { masterGain.store(gainDb, std::memory_order_relaxed); }
    void setBypass(bool shouldBypass) { bypass.store(shouldBypass, std::memory_order_relaxed); }
    void setMute(bool shouldMute) { mute.store(shouldMute, std::memory_order_relaxed); }
    
    // Audio thread reads parameters
    float getInputGain() const { return inputGain.load(std::memory_order_relaxed); }
    float getMasterGain() const { return masterGain.load(std::memory_order_relaxed); }
    bool isBypassed() const { return bypass.load(std::memory_order_relaxed); }
    bool isMuted() const { return mute.load(std::memory_order_relaxed); }
    
private:
    std::atomic<float> inputGain{0.0f};
    std::atomic<float> masterGain{0.0f};
    std::atomic<bool> bypass{false};
    std::atomic<bool> mute{false};
};

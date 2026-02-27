/*
  ==============================================================================

    AudioVisualiser.h
    Created: 21 Aug 2025 10:41:25pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

//==============================================================================
/*
*/
class AudioVisualiser  : public juce::Component
{
public:
    AudioVisualiser();
    ~AudioVisualiser() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Call this from the audio thread with the mixed output
    void pushAudioBuffer(const AudioBuffer<float>& buffer) noexcept;

private:
    AudioVisualiserComponent visual{ 1 }; // stereo
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioVisualiser)
};

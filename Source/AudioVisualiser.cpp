/*
  ==============================================================================

    AudioVisualiser.cpp
    Created: 21 Aug 2025 10:41:25pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AudioVisualiser.h"

//==============================================================================
AudioVisualiser::AudioVisualiser()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    // Configure the built-in visualiser
    visual.setBufferSize(1024);        // size of the scrolling window
    visual.setSamplesPerBlock(256);    // how often we push
    visual.setColours(juce::Colours::black, juce::Colours::lightgreen);


    addAndMakeVisible(visual);
}

AudioVisualiser::~AudioVisualiser()
{
}

void AudioVisualiser::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("AudioVisualiser", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void AudioVisualiser::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    visual.setBounds(getLocalBounds());

}

void AudioVisualiser::pushAudioBuffer(const juce::AudioBuffer<float>& buffer) noexcept
{
    visual.pushBuffer(buffer); //call from audio thread
}

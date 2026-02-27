/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 27 Jul 2025 9:25:46pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(   AudioFormatManager& formatManagerToUse,
                                    AudioThumbnailCache& cacheToUse,
                                    Colour waveformColourToUse)
                                    :   audioThumb (1000,formatManagerToUse, cacheToUse),
                                        fileLoaded (false),
                                        position(0),
                                        waveformColour(waveformColourToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (waveformColour);
    if (fileLoaded)
    {
        audioThumb.drawChannel( g, 
                                getLocalBounds(), 
                                0, 
                                audioThumb.getTotalLength(),
                                0,
                                1.0f);
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth()/50, getHeight());
    }
    else
    {
        g.setFont(juce::FontOptions(20.0f));
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
    std::cout << "WaveformDisplay::loadURL" << std::endl;
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "WaveformDisplay::loadURL: Loaded" << std::endl;
    }
    else
    {
        std::cout << "WaveformDisplay::loadURL: Not Loaded" << std::endl;
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    std::cout << "WaveformDisplay::changeListenerCallback: Change received" << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::clear()
{
    audioThumb.clear();   // juce::AudioThumbnail
    fileLoaded = false;
    repaint();
}





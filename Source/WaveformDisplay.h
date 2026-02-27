/*
  ==============================================================================

    WaveformDisplay.h
    Created: 27 Jul 2025 9:25:46pm
    Author:  Admin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;


//==============================================================================
/*
*/
class WaveformDisplay  :    public Component,
                            public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager & formatManagerToUse,
                    AudioThumbnailCache & cacheToUse,
                    Colour waveformColourToUse);

    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead */
    void setPositionRelative(double pos);

    void clear();


private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    //adding a member for waveform color
    Colour waveformColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};

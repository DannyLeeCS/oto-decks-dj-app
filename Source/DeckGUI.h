/*
  ==============================================================================

    DeckGUI.h
    Created: 21 Jul 2025 8:54:51pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse,
            Colour waveformColourToUse);

    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button* button) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    //timer method
    void timerCallback() override;

    //method for loading tracks and ejecting track from deck
    void loadTrack(const juce::File& file);
    void ejectTrack();



private:
    //declare components in deck GUI
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton ejectButton{ "EJECT" };
    TextButton loadExternalButton{ "LOAD EXTERNAL TRACK" };
    ToggleButton loopSwitch;

    //declare different sliders
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    Slider scratchingSlider;

    //labels
    Label volLabel;
    Label speedLabel;


    //initial vinyl angle
    float vinylAngle = 0.0f;

    //flag to detect if the vinyl is being scratched
    bool isScratching = false;

    //declaring image components
    ImageComponent vinylDisk;

    //declaring member variables of deck GUI
    DJAudioPlayer* player;
    FileChooser fChooser{ "Select a file..." };
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};

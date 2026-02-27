/*
  ==============================================================================

    SoundBoard.h
    Created: 22 Aug 2025 9:44:50pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class SoundBoard  : public Component,
                    public Button::Listener,
                    public Slider::Listener
                    
{
public:
    
    SoundBoard();
    ~SoundBoard() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button* button) override;

    // --- NEW: engine hooks this so buttons can trigger sounds ---
    void setPadTrigger(std::function<void(int)> fn) { padTrigger = std::move(fn); }

    void sliderValueChanged(Slider* slider) override;

    //settler for callback function 
    void setOnVolumeChanged(std::function<void(float)> fn) { onVolumeChanged = std::move(fn); }


private:
    //instruments row 1
    TextButton aButton{ "crash" };
    TextButton bButton{ "drum kick" };
    TextButton cButton{ "piano" };
    TextButton dButton{ "guitar" };
    TextButton eButton{ "violin" };
    TextButton fButton{ "stick snap" };

    //instruments row 2
    TextButton gButton{ "snare" };
    TextButton hButton{ "synth kick" };
    TextButton iButton{ "e-drum" };
    TextButton jButton{ "guitarras" };
    TextButton kButton{ "e-guitar" };
    TextButton lButton{ "glitch" };

    //sound effects row
    TextButton mButton{ "chill" };
    TextButton nButton{ "laser" };
    TextButton oButton{ "gun shot" };
    TextButton pButton{ "kiss" };
    TextButton qButton{ "crowd cheer" };
    TextButton rButton{ "clap" };

    //labels
    Label instrumentLabel;
    Label soundEffectLabel;
    Label masterLabel;

    //new
    DJAudioPlayer* player = nullptr;

    Slider soundboardvolSlider;
    Label soundboardvolLabel;

    //trigger pad by index
    std::function<void(int)> padTrigger;

    //function to trigger when soundboad slider value chaneges
    std::function<void(float)> onVolumeChanged;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundBoard)
};

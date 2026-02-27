/*
  ==============================================================================

    SoundBoard.cpp
    Created: 22 Aug 2025 9:44:50pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SoundBoard.h"

//==============================================================================
SoundBoard::SoundBoard()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //label settings
    instrumentLabel.setText("Instruments", juce::dontSendNotification);
    instrumentLabel.setJustificationType(juce::Justification::centredBottom);
    instrumentLabel.setFont(juce::FontOptions(16.0f, juce::Font::plain));
    instrumentLabel.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);

    soundEffectLabel.setText("Sound Effects", juce::dontSendNotification);
    soundEffectLabel.setJustificationType(juce::Justification::centredBottom);
    soundEffectLabel.setFont(juce::FontOptions(16.0f, juce::Font::plain));
    soundEffectLabel.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);

    masterLabel.setText("Soundboard volume", juce::dontSendNotification);
    masterLabel.setJustificationType(juce::Justification::centredBottom);
    masterLabel.setFont(juce::FontOptions(16.0f, juce::Font::plain));
    masterLabel.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);

    addAndMakeVisible(soundboardvolSlider);
    addAndMakeVisible(soundboardvolLabel);

    addAndMakeVisible(instrumentLabel);
    addAndMakeVisible(soundEffectLabel);
    addAndMakeVisible(masterLabel);

    addAndMakeVisible(aButton);
    addAndMakeVisible(bButton);
    addAndMakeVisible(cButton);
    addAndMakeVisible(dButton);
    addAndMakeVisible(eButton);
    addAndMakeVisible(fButton);

    addAndMakeVisible(gButton);
    addAndMakeVisible(hButton);
    addAndMakeVisible(iButton);
    addAndMakeVisible(jButton);
    addAndMakeVisible(kButton);
    addAndMakeVisible(lButton);

    addAndMakeVisible(mButton);
    addAndMakeVisible(nButton);
    addAndMakeVisible(oButton);
    addAndMakeVisible(pButton);
    addAndMakeVisible(qButton);
    addAndMakeVisible(rButton);

    //1st instrument row
    aButton.addListener(this);
    bButton.addListener(this);
    cButton.addListener(this);
    dButton.addListener(this);
    eButton.addListener(this);
    fButton.addListener(this);

    //2nd instrument row
    gButton.addListener(this);
    hButton.addListener(this);
    iButton.addListener(this);
    jButton.addListener(this);
    kButton.addListener(this);
    lButton.addListener(this);

    //sound effect row
    mButton.addListener(this);
    nButton.addListener(this);
    oButton.addListener(this);
    pButton.addListener(this);
    qButton.addListener(this);
    rButton.addListener(this);

    //soundboard volume slider
    soundboardvolSlider.setRange(0.0, 1.0, 0.01);
    soundboardvolSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    soundboardvolSlider.setValue(0.5);
    soundboardvolSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    soundboardvolSlider.addListener(this);


    //labels properties
    soundboardvolLabel.setText("Volume", juce::dontSendNotification);
    soundboardvolLabel.setJustificationType(juce::Justification::centredBottom);
    soundboardvolLabel.setFont(juce::FontOptions(12.0f, juce::Font::plain));
    soundboardvolLabel.setInterceptsMouseClicks(false, false);

}

SoundBoard::~SoundBoard()
{

}

void SoundBoard::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::darkgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("SoundBoard", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void SoundBoard::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    int width = getWidth() / 12;
    int height = getHeight() / 12;

    instrumentLabel.setBounds(0, 0, getWidth(), height);
    soundEffectLabel.setBounds(0, height*5, getWidth(), height);
    masterLabel.setBounds(0, height * 8, getWidth(), height);

    aButton.setBounds(0, height, width*2, height*2);
    bButton.setBounds(width*2, height, width * 2, height * 2);
    cButton.setBounds(width*4, height, width * 2, height * 2);
    dButton.setBounds(width*6, height, width * 2, height * 2);
    eButton.setBounds(width*8, height, width * 2, height * 2);
    fButton.setBounds(width*10, height, width * 2, height * 2);

    gButton.setBounds(0, height*3, width * 2, height * 2);
    hButton.setBounds(width * 2, height * 3, width * 2, height * 2);
    iButton.setBounds(width * 4, height * 3, width * 2, height * 2);
    jButton.setBounds(width * 6, height * 3, width * 2, height * 2);
    kButton.setBounds(width * 8, height * 3, width * 2, height * 2);
    lButton.setBounds(width * 10, height * 3, width * 2, height * 2);

    mButton.setBounds(0, height * 6, width * 2, height * 2);
    nButton.setBounds(width * 2, height * 6, width * 2, height * 2);
    oButton.setBounds(width * 4, height * 6, width * 2, height * 2);
    pButton.setBounds(width * 6, height * 6, width * 2, height * 2);
    qButton.setBounds(width * 8, height * 6, width * 2, height * 2);
    rButton.setBounds(width * 10, height * 6, width * 2, height * 2);

    soundboardvolSlider.setBounds(getWidth()/2- width, height * 9.5, width * 2, height * 3.5);
    soundboardvolLabel.setBounds(getWidth() / 2 +20, height* 10, 100, 30);
}

void SoundBoard::buttonClicked(Button* button)
{
    if (!padTrigger)
    {
        // if engine hasn’t set it yet
        return;
    }


    // map buttons to padTrigger
    if (button == &aButton) { padTrigger(0);  return; }
    if (button == &bButton) { padTrigger(1);  return; }
    if (button == &cButton) { padTrigger(2);  return; }
    if (button == &dButton) { padTrigger(3);  return; }
    if (button == &eButton) { padTrigger(4);  return; }
    if (button == &fButton) { padTrigger(5);  return; }
    if (button == &gButton) { padTrigger(6);  return; }
    if (button == &hButton) { padTrigger(7);  return; }
    if (button == &iButton) { padTrigger(8);  return; }
    if (button == &jButton) { padTrigger(9);  return; }
    if (button == &kButton) { padTrigger(10);  return; }
    if (button == &lButton) { padTrigger(11);  return; }
    if (button == &mButton) { padTrigger(12);  return; }
    if (button == &nButton) { padTrigger(13);  return; }
    if (button == &oButton) { padTrigger(14);  return; }
    if (button == &pButton) { padTrigger(15);  return; }
    if (button == &qButton) { padTrigger(16);  return; }
    if (button == &rButton) { padTrigger(17);  return; }

}
//slider for adjusting volume
void SoundBoard::sliderValueChanged(Slider* slider)
{
    if (slider == &soundboardvolSlider)
    {

        const float g = (float)slider->getValue();
        if (onVolumeChanged) onVolumeChanged(g);      

        int volVal = (int)std::round(slider->getValue() * 100.0);
        soundboardvolLabel.setText("Volume: " + juce::String(volVal) + "%", juce::dontSendNotification);

    }
}

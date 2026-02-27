/*
  ==============================================================================

    DeckGUI.cpp
    Created: 21 Jul 2025 8:54:51pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(   DJAudioPlayer* _player,
                    AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse,
                    Colour waveformColourToUse) 
    
                    :   player(_player),      
                        waveformDisplay(formatManagerToUse, cacheToUse, waveformColourToUse)
{
    //labels properties
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.setJustificationType(juce::Justification::centredBottom);
    volLabel.setFont(juce::FontOptions(12.0f, juce::Font::plain));
    volLabel.setInterceptsMouseClicks(false, false);

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centredBottom);
    speedLabel.setFont(juce::FontOptions(12.0f, juce::Font::plain));
    speedLabel.setInterceptsMouseClicks(false, false);

    //toggle button
    loopSwitch.setButtonText("Loop");

    //make components visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadExternalButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(ejectButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(scratchingSlider);
    addAndMakeVisible(vinylDisk);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(loopSwitch);

    //add listeners to components
    playButton.addListener(this);
    stopButton.addListener(this);
    loadExternalButton.addListener(this);
    ejectButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    scratchingSlider.addListener(this);
    loopSwitch.addListener(this);

    //set slider's range
    volSlider.setRange(0.0, 1.0, 0.01);
    speedSlider.setRange(0.25, 4.0, 0.25);
    posSlider.setRange(0.0, 1.0);
    scratchingSlider.setRange(0.0005, 0.005);

    //set sliders' style
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    scratchingSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);

    //stylers's properties
    scratchingSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    scratchingSlider.setAlpha(0.001f);
    // make the slider invisible by adjusting its alpha value but still receives mouse
    posSlider.setAlpha(0.001f);
    
    // spring back when drag ends
    scratchingSlider.onDragEnd = [this]() 
    {
        scratchingSlider.setValue(0.00275, juce::dontSendNotification);
    };

    //setting sliders initial values
    scratchingSlider.setValue(0.00275);
    volSlider.setValue(0.5);
    speedSlider.setValue(1.0);

    //Slider textbox options
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    scratchingSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    startTimer(500);

    //initially disable play and stop button since no track is loaded
    playButton.setEnabled(false);
    stopButton.setEnabled(false);
    scratchingSlider.setEnabled(false);

    //set image component properties
    vinylDisk.setImage(ImageFileFormat::loadFrom(   BinaryData::vinyl_png,
                                                    BinaryData::vinyl_pngSize));

    vinylDisk.setImagePlacement(RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize);
    // make the disk ignore mouse so events pass to slider
    vinylDisk.setInterceptsMouseClicks(false, false);
    // draw the disk above the slider
    vinylDisk.toFront(false); 
    vinylDisk.setVisible(false);
    //make the waveform ignore mouse so events pass to position slider underneath
    waveformDisplay.setInterceptsMouseClicks(false, false);
    waveformDisplay.toFront(false);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{

    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(juce::Colours::black);   // clear the background

    g.setColour (juce::Colours::grey);
    // draw an outline around the component
    g.drawRect (getLocalBounds(), 1);   

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    //placeholder text
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);  
}

void DeckGUI::resized()
{
    //setting components bounds inside the DeckGUI
    double rowH = getHeight() / 12;
    scratchingSlider.setBounds(0, 0, getWidth(), rowH*8);
    waveformDisplay.setBounds(0, rowH*8, getWidth(), rowH);
    posSlider.setBounds(0, rowH*8, getWidth(), rowH);

    loopSwitch.setBounds(25, rowH * 9+17, getWidth() / 4, rowH);
    volSlider.setBounds(getWidth() / 3, rowH * 9, getWidth()/3, rowH*2);
    volLabel.setBounds(getWidth() / 3, rowH * 9, getWidth() / 3, rowH * 2);

    speedSlider.setBounds(getWidth()*2 / 3, rowH*9.5, getWidth()/4, rowH);
    speedLabel.setBounds(getWidth() * 2 / 3, rowH * 10, getWidth() / 4, rowH);

    loadExternalButton.setBounds(0, rowH * 11, getWidth() / 4, getHeight()- rowH * 11);
    ejectButton.setBounds(getWidth() / 4, rowH * 11, getWidth() / 4, getHeight() - rowH * 11);
    playButton.setBounds(getWidth()/2, rowH * 11, getWidth()/4, getHeight() - rowH * 11);
    stopButton.setBounds(getWidth()*3 / 4, rowH * 11, getWidth()/4, getHeight() - rowH * 11);

    vinylDisk.setBounds(scratchingSlider.getBounds());

}
//button logics
void DeckGUI::buttonClicked(Button* button) {
    if (button == &playButton)
    {
        std::cout << "Play button was clicked" << std::endl;
        player->start();

    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked" << std::endl;
        player->stop();
    }
    if (button == &ejectButton)
    {
        ejectTrack();
        return;
    }
    if (button == &loadExternalButton)
    {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles
            | FileBrowserComponent::openMode;
            //| FileBrowserComponent::canSelectMultipleItems;

        //enable loading of external files straight to deck
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            player->loadURL(URL{ chosenFile });
            waveformDisplay.loadURL(URL{ chosenFile });

            //enable play & stop button since track is loaded
            playButton.setEnabled(true);
            stopButton.setEnabled(true);
            scratchingSlider.setEnabled(true);
            vinylDisk.setVisible(true);

        });
    }
    if (button == &loopSwitch)
    {
        if (loopSwitch.getToggleState())
        {
          const bool on = loopSwitch.getToggleState();
          player->setLooping(on);
          loopSwitch.setButtonText("Loop is on");
        }
        else
        {
          const bool off = loopSwitch.getToggleState();
          player->setLooping(off);
          loopSwitch.setButtonText("Loop if off");
        }
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    //detecting scratch
    if (slider == &scratchingSlider)
    {
        player->setScratchPosition(slider->getValue());
        //user is scratching
        isScratching = true;  
    }
    else
    {
        // the use of other sliders resets scratching state
        isScratching = false;
    }

    //listeners for different sliders
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
        int volVal = (int)std::round(slider->getValue() * 100.0);
        volLabel.setText("Volume: " + juce::String(volVal) + "%", juce::dontSendNotification);
    
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
        float speedVal = slider->getValue();
        speedLabel.setText("Speed: " + juce::String(speedVal) + "x", juce::dontSendNotification);
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &scratchingSlider)
    {
        player->setScratchPosition(slider->getValue());
    }
}

//selecting files window
bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag " << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped " << std::endl;
    if (files.size()==1)
    {
        player->loadURL(URL{ File{files[0]} });
    }
}

//Timer as track plays
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    posSlider.setValue(player->getPositionRelative());

    // spin the vinyl while playing
    if (vinylDisk.isVisible() && player->isPlaying() && !isScratching)
    {
        //sets rotation frequency
        constexpr float revPerSec = 33.333f / 60.0f;
        constexpr float dt = 1.0f / 60.0f; // because startTimerHz(60)
        vinylAngle += juce::MathConstants<float>::twoPi * revPerSec * dt;

        // disk rotates around its centre
        vinylDisk.setTransform(juce::AffineTransform::rotation(
            vinylAngle,
            vinylDisk.getWidth() * 0.5f,
            vinylDisk.getHeight() * 0.5f
        ));
    }
}

//method for loading tracks
void DeckGUI::loadTrack(const juce::File& file)
{
    auto url = juce::URL{ file };
    player->loadURL(url);
    waveformDisplay.loadURL(url);

    //enable play & stop button since track is loaded
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
    scratchingSlider.setEnabled(true);
    vinylDisk.setVisible(true);
}

//function for ejecting the whole playlist from playlist table
void DeckGUI::ejectTrack()
{
    //Stop & unload audio from the player
    if (player) {
        player->stop();     
        player->unload();   
    }

    //Clear the waveform
    waveformDisplay.clear();  // implemented in waveformDisplay.cpp

    //reset UI state
    // disable play until a new track is loaded
    playButton.setEnabled(false);
    stopButton.setEnabled(false);
    scratchingSlider.setEnabled(false);

    // reset sliders to their default values
    volSlider.setValue(0.5, juce::dontSendNotification);
    speedSlider.setValue(1.0, juce::dontSendNotification);
    scratchingSlider.setValue(0.00275, juce::dontSendNotification);

    //disk disappears if no track is loaded
    vinylDisk.setVisible(false);
}
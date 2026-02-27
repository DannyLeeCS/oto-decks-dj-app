#pragma once

#include <JuceHeader.h>
using namespace juce;
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "AudioVisualiser.h"
#include "SoundBoard.h"


//==============================================================================
/*
    This component lives inside window, and this is where all
    controls and content are put.
*/
class MainComponent :   public AudioAppComponent,
                        public Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

    //will be called by soundboard to play a pad
    void triggerPad(int index);

    //method for loading from JUCE binary
    bool loadPadFromBinary(int index, const void* data, size_t size);

    //slider listener
    void sliderValueChanged(Slider* slider) override;


private:
    //==============================================================================
    // Private member variables go here...

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{ 100 };

    //Instantiate deck 1 with its player1 audio player inside
    DJAudioPlayer player1{ formatManager };
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache, Colours::lightblue };

    //Instantiate deck 2 with its player1 audio player inside
    DJAudioPlayer player2{ formatManager };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache, Colours::pink};

    //Instantiate audio source
    MixerAudioSource mixerSource;

    //Instantiate the audio visualiser
    AudioVisualiser visualiser;

    //Instantiate the soundboard
    SoundBoard soundboard;

    //Instantiate playlist component with pointers to 2 decks
    //since this allows the playlist to send tracks directly to the two decks
    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2 };

    //An array for 18 pad players for each button in the soundboard
    //one AudioTransportSource + reader per pad
    std::array<juce::AudioTransportSource, 18> padTransports;
    std::array<std::unique_ptr<juce::AudioFormatReaderSource>, 18> padReaders;

    //crossfader components
    Slider crossfader;
    Label crossfaderDeck1Label;
    Label crossfaderDeck2Label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

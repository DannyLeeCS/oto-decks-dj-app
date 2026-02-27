/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 15 Jul 2025 10:28:49pm
    Author:  Admin

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
using namespace juce;

class DJAudioPlayer : public AudioSource 
{
public:
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain (double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    void setScratchPosition(double change);

    void start();
    void stop();

    //looping feature
    void setLooping(bool shouldLoop);
    bool getLooping() const { return looping; }

    //Get the relative position of the playhead
    double getPositionRelative();
    void unload();

    //Constant determine if the audio is being played or not
    bool isPlaying() const;


private:
    AudioFormatManager& formatManager;
    std::unique_ptr <AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    //remember's looping state 
    bool looping = false;

};
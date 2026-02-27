/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 15 Jul 2025 10:28:49pm
    Author:  Admin

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                                : formatManager (_formatManager)
{

}

DJAudioPlayer:: ~DJAudioPlayer()
{

}

void DJAudioPlayer:: prepareToPlay( int samplesPerBlockExpected, 
                                    double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer:: getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer:: releaseResources()
{
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) //check if the file is an empty file
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(
            new AudioFormatReaderSource(
                reader,
                true));

        //adding a loop flag
        newSource->setLooping(looping);                // <-- important

        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

//method for increasing the volume
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0) 
    {
        std::cout << "DJAudioPlayer::setGain: Gain should be between 0 and 1" << std::endl;
    }
    else
    {
       transportSource.setGain(gain);
    }
}

//method for increasing/decreasing speed
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 5.0)
    {
        std::cout << "DJAudioPlayer::setSpeed: Ratio should be between 0 and 5" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

//method for changing position in the track
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

//function to determine the track position when pos Slider value changes
void DJAudioPlayer::setPositionRelative(double pos) 
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative: pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

//method for slightly changing track's position when scratching
void DJAudioPlayer::setScratchPosition(double change)
{
    if (change < 0.0005 || change > 0.005)
    {
        std::cout << "DJAudioPlayer::setScratchPosition pos is out of bound" << std::endl;
    }
    if(change >= 0.0005 && change <0.00275)
    {
        double pos = transportSource.getCurrentPosition();
        transportSource.setPosition(pos - change); // backward change

    }
    if (change >= 0.00275 && change <= 0.005)
    {
        double pos = transportSource.getCurrentPosition();
        transportSource.setPosition(pos + change); // forward change
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{

    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();

};

void DJAudioPlayer::unload()
{
    transportSource.stop();
    // detach from transport
    transportSource.setSource(nullptr);         
    readerSource.reset();

    //reset position
    transportSource.setPosition(0.0);
}

//determines if the audio is being played
bool DJAudioPlayer::isPlaying() const
{
    return transportSource.isPlaying();
}

//looping function
void DJAudioPlayer::setLooping(bool shouldLoop)
{
    looping = shouldLoop;
    if (readerSource)
        readerSource->setLooping(looping);
}

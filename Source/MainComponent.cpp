#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        //Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    //labels properties
    crossfaderDeck1Label.setText("Deck 1", juce::dontSendNotification);
    crossfaderDeck1Label.setJustificationType(juce::Justification::centredBottom);
    crossfaderDeck1Label.setFont(juce::FontOptions(11.0f, juce::Font::plain));
    crossfaderDeck1Label.setInterceptsMouseClicks(false, false);
    crossfaderDeck1Label.setColour(juce::Label::textColourId, juce::Colours::white);

    crossfaderDeck2Label.setText("Deck 2", juce::dontSendNotification);
    crossfaderDeck2Label.setJustificationType(juce::Justification::centredBottom);
    crossfaderDeck2Label.setFont(juce::FontOptions(11.0f, juce::Font::plain));
    crossfaderDeck2Label.setInterceptsMouseClicks(false, false);
    crossfaderDeck2Label.setColour(juce::Label::textColourId, juce::Colours::white);

    //make components visible
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(visualiser);
    addAndMakeVisible(soundboard);
    addAndMakeVisible(crossfader);
    addAndMakeVisible(crossfaderDeck1Label);
    addAndMakeVisible(crossfaderDeck2Label);

    //crossfader listener
    crossfader.addListener(this);

    //crossfader style
    crossfader.setRange(0.0, 1.0, 0.01);
    crossfader.setValue(0.5);                // start at center
    crossfader.setSliderStyle(juce::Slider::LinearVertical);

    //make the format manager recognise basic audio formats
    formatManager.registerBasicFormats();

    //map specific files to specific pads
    loadPadFromBinary(0, BinaryData::crash_mp3, BinaryData::crash_mp3Size);
    loadPadFromBinary(1, BinaryData::drumkick_wav, BinaryData::drumkick_wavSize);
    loadPadFromBinary(2, BinaryData::piano_wav, BinaryData::piano_wavSize);
    loadPadFromBinary(3, BinaryData::guitar_wav, BinaryData::guitar_wavSize);
    loadPadFromBinary(4, BinaryData::violin_wav, BinaryData::violin_wavSize);
    loadPadFromBinary(5, BinaryData::sticksnap_wav, BinaryData::sticksnap_wavSize);
    loadPadFromBinary(6, BinaryData::esnare_wav, BinaryData::esnare_wavSize);
    loadPadFromBinary(7, BinaryData::synthkick_wav, BinaryData::synthkick_wavSize);
    loadPadFromBinary(8, BinaryData::drumloop_wav, BinaryData::drumloop_wavSize);
    loadPadFromBinary(9, BinaryData::guitarras_wav, BinaryData::guitarras_wavSize);
    loadPadFromBinary(10, BinaryData::eguitar_flac, BinaryData::eguitar_flacSize);
    loadPadFromBinary(11, BinaryData::glitch_wav, BinaryData::glitch_wavSize);
    loadPadFromBinary(12, BinaryData::whawhamus_wav, BinaryData::whawhamus_wavSize);
    loadPadFromBinary(13, BinaryData::laser_wav, BinaryData::laser_wavSize);
    loadPadFromBinary(14, BinaryData::gunshot_mp3, BinaryData::gunshot_mp3Size);
    loadPadFromBinary(15, BinaryData::kiss_mp3, BinaryData::kiss_mp3Size);
    loadPadFromBinary(16, BinaryData::crowdcheer_wav, BinaryData::crowdcheer_wavSize);
    loadPadFromBinary(17, BinaryData::clap_wav, BinaryData::clap_wavSize);

    //register pad players
    soundboard.setPadTrigger([this](int i) { this->triggerPad(i); });

    //calls method to adjust soundboard pad volume
    soundboard.setOnVolumeChanged([this](float g) 
    {
        for (auto& t : padTransports) t.setGain(g);
    }
    );

}

MainComponent::~MainComponent()
{
    //Shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

    //a loop to play soundboard pads
    for (auto& t : padTransports)
    {
        mixerSource.addInputSource(&t, false);
    }
        
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);

    //send bufferToFill to visualiser
    if (bufferToFill.buffer != nullptr && bufferToFill.numSamples > 0)
        visualiser.pushAudioBuffer(*bufferToFill.buffer);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();

    //cleanup: stop pads; detach sources when load new audio or unload
    for (auto& t : padTransports)
    {
        t.stop();
    }
        
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // Fill the background with a solid colour
    g.fillAll (Colours:: black);

}

void MainComponent::resized()
{
    //the layout of the whole app
    int height = getHeight()/12;
    visualiser.setBounds(0, 0, getWidth(), height);
    deckGUI1.setBounds(0, height, getWidth()/2-20, height*7);
    deckGUI2.setBounds(getWidth() / 2+20, height, getWidth() / 2-20, height*7);

    //set bounds and stretch them until the end of the window
    playlistComponent.setBounds(0, height*8, getWidth()/2, getHeight() - height * 8);
    soundboard.setBounds(getWidth() / 2, height * 8, getWidth() / 2, getHeight() - height * 8);

    //crossfader
    crossfader.setBounds(getWidth() / 2-10, height * 2, 20, height * 5);
    crossfaderDeck1Label.setBounds(getWidth() / 2-20, height, 40, height);
    crossfaderDeck2Label.setBounds(getWidth() / 2 - 20, height*6+30, 40, height);
}

// buttons in the soundboard will call this to trigger sound
void MainComponent::triggerPad(int index)
{
    if (index < 0 || index >= (int)padTransports.size())
    {
        return;
    }
    auto& t = padTransports[(size_t)index];

    //if no source loaded for the button yet, restart the soundboard to original state
    t.stop();
    t.setPosition(0.0);
    t.start();
}
//handles binary loading (for soundboard)
bool MainComponent::loadPadFromBinary(int index, const void* data, size_t size)
{
    if (index < 0 || index >= (int)padTransports.size() || data == nullptr || size == 0)
        return false;

    // Memory stream over the embedded bytes 
    //wraps BinaryData
    auto mem = std::make_unique<juce::MemoryInputStream>(data, size, false);
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(std::move(mem)));
    //if unsupported or failed
    if (!reader)
    {
        return false;
    }

    const double fileRate = reader->sampleRate;

    auto newReaderSource = std::make_unique<juce::AudioFormatReaderSource>(reader.release(), true);

    auto& transport = padTransports[(size_t)index];
    transport.stop();
    transport.setSource(nullptr); // detach any previous source
    transport.setSource(newReaderSource.get(),0,nullptr,fileRate);

    //keep AudioFormatReaderSource and its AudioFormatReader alive
    padReaders[(size_t)index] = std::move(newReaderSource);
    return true;
}


void MainComponent::sliderValueChanged(Slider* slider)
{
    if (slider == &crossfader)
    {
        //adjust player gain based on crossfader value
        const double x = slider->getValue();
        //crossfader will make players gain value oppositely
        player1.setGain(1.0-x);
        player2.setGain(x);

        //convert value to % and show in crossfader's label
        const int deck2Pct = (int)std::round(x * 100.0);
        const int deck1Pct = 100 - deck2Pct;
        crossfaderDeck1Label.setText("Deck 1:\n" + juce::String(deck1Pct) + "%", juce::dontSendNotification);
        crossfaderDeck2Label.setText("Deck 2:\n" + juce::String(deck2Pct) + "%", juce::dontSendNotification);
    }
}
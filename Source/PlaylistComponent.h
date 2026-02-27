/*
  ==============================================================================

    PlaylistComponent.h
    Created: 7 Aug 2025 8:29:43pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
#include <vector>

//new
class DeckGUI;

//==============================================================================
/*
*/
class PlaylistComponent :   public Component,
                            public TableListBoxModel,
                            public Button::Listener

{
public:
    //new
    PlaylistComponent(DeckGUI* d1, DeckGUI* d2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber, int width,
                            int height, bool rowIsSelected) override;

    void paintCell(Graphics&, int rowNumber, int columnId, int width,
                   int height, bool rowIsSelected) override;

    Component* refreshComponentForCell( int rowNumber, int columnId,
                                        bool isRowSelected,
                                        Component* existingComponentToUpdate);
    void buttonClicked(Button* button) override;


private:
    //table
    TableListBox tableComponent;

    //vector of files to store
    std::vector <File> trackFiles;

    TextButton loadPlaylistFolder{ "LOAD PLAYLIST" };
    TextButton removePlaylistFolder{ "REMOVE PLAYLIST" };

    //enable choosing folder
    FileChooser chooser{ "Select folder containing tracks...", File{}, "*.mp3;*.wav", true};

    //declaring pointers to 2 GUI decks
    DeckGUI* deck1 = nullptr;
    DeckGUI* deck2 = nullptr;

    //functions to save and load playlist ensuring it persists between sessions
    void savePlaylist() const;
    void loadPlaylist();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

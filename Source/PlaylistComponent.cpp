/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 7 Aug 2025 8:29:43pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DeckGUI.h" 

//==============================================================================
//
PlaylistComponent::PlaylistComponent(DeckGUI* d1, DeckGUI* d2)
                        : deck1(d1), deck2(d2)
{
    // Child components, and initialise any settings that components need.
    tableComponent.getHeader().addColumn("Track Title", 1, 100);
    //tableComponent.getHeader().addColumn("Length", 5, 100);
    tableComponent.getHeader().addColumn("Send to 1", 2, 100);
    tableComponent.getHeader().addColumn("Send to 2", 3, 100);
    tableComponent.getHeader().addColumn("Action", 4, 100);

    tableComponent.setModel(this);
    loadPlaylistFolder.addListener(this);
    removePlaylistFolder.addListener(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadPlaylistFolder);
    addAndMakeVisible(removePlaylistFolder);

    //load stored playlist (if any)
    loadPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    //Set the bounds of any child components
    removePlaylistFolder.setBounds(0, 0, getWidth()/2, getHeight()/5);
    loadPlaylistFolder.setBounds(getWidth()/2, 0, getWidth()/2, getHeight() / 5);
    tableComponent.setBounds(0, getHeight()/5, getWidth(), getHeight());

    tableComponent.getHeader().setColumnWidth(1, getWidth() / 6 * 3);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 6);
    tableComponent.getHeader().setColumnWidth(3, getWidth() / 6);
    tableComponent.getHeader().setColumnWidth(4, getWidth() / 6);
}

int PlaylistComponent::getNumRows()
{
    return trackFiles.size();
}

void PlaylistComponent::paintRowBackground( Graphics& g, int rowNumber, int width,
                                            int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else
    {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(  Graphics& g, int rowNumber, int columnId, int width,
                                    int height, bool rowIsSelected)
{
    g.drawText( trackFiles[rowNumber].getFileName(), 2, 0, width - 4, height,
                Justification::centredLeft, true);

}

Component* PlaylistComponent::refreshComponentForCell(  int rowNumber, 
                                                        int columnId,
                                                        bool isRowSelected,
                                                        Component* existingComponentToUpdate)
{
    //send to deck 1 button
    if (columnId == 2) 
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Deck 1" };
            //set button ID
            btn->setComponentID("d1:" + juce::String(rowNumber));
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }

    }
    //send to deck 2 button
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Deck 2" };
            //set button ID
            btn->setComponentID("d2:" + juce::String(rowNumber));
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }

    }
    //remove individual track from list logic
    if (columnId == 4) // Remove column for removing file from table
    {
        if (existingComponentToUpdate == nullptr)
        {
            auto* btn = new juce::TextButton{ "Remove" };
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }

        // Always update the ID because table cells get reused
        if (auto* btn = dynamic_cast<juce::TextButton*>(existingComponentToUpdate))
            btn->setComponentID("rm:" + juce::String(rowNumber));

        return existingComponentToUpdate;
    }


    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    //declare namespace for shorter code
    using juce::String; 

    //Handle "send to deck" table buttons
    if (auto* tb = dynamic_cast<TextButton*>(button))
    {
        //get the id of button clicked in the playlist table
        const String id = tb->getComponentID();
        if (id.startsWith("d1:") || id.startsWith("d2:"))
        {
            const int row = id.fromFirstOccurrenceOf(":", false, false).getIntValue();

            if (juce::isPositiveAndBelow(row, (int)trackFiles.size()))
            {
                auto& file = trackFiles[(size_t)row];
                if (id.startsWith("d1:") && deck1) deck1->loadTrack(file);
                if (id.startsWith("d2:") && deck2) deck2->loadTrack(file);
            }
            return;
        }
    }

    //load playlist button logic
    if (button == &loadPlaylistFolder)
    {
        std::cout << "loadPlaylistFolder button was clicked" << std::endl;

        chooser.launchAsync(
            FileBrowserComponent::canSelectDirectories,
            [this](const FileChooser& fc)
            {
                File folder = fc.getResult();
                if (!folder.exists())
                {
                    std::cout << "Selected folder doesn't exist." << std::endl;
                    return;
                }

                std::cout << "Folder selected: " << folder.getFullPathName() << std::endl;

                Array<File> audioFiles = folder.findChildFiles(File::findFiles, false, "*.mp3;*.wav");

                if (audioFiles.isEmpty())
                {
                    std::cout << "No audio files found in selected folder." << std::endl;
                    return;
                }

                trackFiles.clear();
                for (auto& file : audioFiles)
                {
                    std::cout << "Found file: " << file.getFullPathName() << std::endl;
                    trackFiles.push_back(file);  // Store files temporarily
                }

                tableComponent.updateContent();
                tableComponent.repaint();
                savePlaylist();
            });
    }
    //remove playlist button logic
    if (button == &removePlaylistFolder)
    {
        std::cout << "removePlaylistFolder button was clicked" << std::endl;
        trackFiles.clear();
        tableComponent.updateContent();
        tableComponent.repaint();
        savePlaylist();
    }

    //remove track in row logic
    if (auto* tb = dynamic_cast<juce::TextButton*>(button))
    {
        const auto id = tb->getComponentID();      
        if (id.startsWith("rm:"))
        {
            const int row = id.fromFirstOccurrenceOf(":", false, false).getIntValue();

            if (juce::isPositiveAndBelow(row, (int)trackFiles.size()))
            {
                //Remove the file at that row
                trackFiles.erase(trackFiles.begin() + row);

                //Refresh the table
                tableComponent.updateContent();  // updates row count
                tableComponent.repaint();
                savePlaylist();
            }
            return;
        }
    }
}

//function to save playlist in trackFiles to a text file
void PlaylistComponent::savePlaylist() const
{
    auto file = juce::File::getSpecialLocation(
        juce::File::userApplicationDataDirectory)
        .getChildFile("OtoDecks")
        .getChildFile("playlist.txt");

    file.getParentDirectory().createDirectory();

    juce::StringArray lines;
    for (auto& f : trackFiles)
        lines.add(f.getFullPathName());
    //creates a text file to store file paths
    file.replaceWithText(lines.joinIntoString("\n"));
}

//function to load playlist
void PlaylistComponent::loadPlaylist()
{
    auto file = juce::File::getSpecialLocation(
        juce::File::userApplicationDataDirectory)
        .getChildFile("OtoDecks")
        .getChildFile("playlist.txt");

    if (!file.existsAsFile())
        return;

    auto text = file.loadFileAsString();
    juce::StringArray lines;
    lines.addLines(text);

    trackFiles.clear();
    for (auto& path : lines)
    {
        juce::File f(path);
        if (f.existsAsFile())
            trackFiles.push_back(f);
    }

    tableComponent.updateContent();
    tableComponent.repaint();
}
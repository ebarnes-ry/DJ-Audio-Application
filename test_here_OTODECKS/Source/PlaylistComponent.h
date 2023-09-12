/*
  ==============================================================================

    PlaylistComponent.h
    Created: 4 Sep 2023 10:56:02pm
    Author:  Emmab

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener,
                           public juce::FileDragAndDropTarget,
                           public juce::AudioSource
{
public:
    PlaylistComponent(AudioFormatManager& formatManagerToUse);
    ~PlaylistComponent() override;

    /**input graphics*/
    void paint (juce::Graphics&) override;
    void resized() override;

    //==========================================================================
    /**doing audio source things*/
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
    void releaseResources();


    //========================================================================
    /**overriding the tableListBoxModel stuff that draws background behind rows*/
    int getNumRows() override;
    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;
    /**overriding TablListBoxModel stuff, draws one cells used to input details in cells*/
    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;
    /**overriding TablListBoxModel stuff, creates and updates custom components like buttons*/
    Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate) override;

    //========================================================================

    /**called when button clicked, button loads files*/
    void buttonClicked(Button* button) override;

    //========================================================================

    /**Override FileDragAndDropTarget*/
    /**Callback to check if target can accept stuff dropped*/
    bool isInterestedInFileDrag(const StringArray& files) override;
    /**callback to check user dropped files HERE*/
    void filesDropped(const StringArray& files, int x, int y) override;

    //========================================================================

    /**override TextEditor::Listener*/
    /**called whenever user changes text in obj*/
    void textEditorTextChanged(TextEditor&) override;

    //========================================================================
    /**data struct of songs to be added left and right, used by DeckGUI*/
    std::vector<std::string> playlist1;
    std::vector<std::string> playlist2;

    //========================================================================
    //whenever cue is pressed with an empty playlist, user prompted to load a file to respective playlist manually
    void PlaylistComponent::promptLoadButton(int deck);
    
    /*accessible so that the deck's load prompt can add in a file to the playlist when it's empty and someone presses play*/
    //void addToDeckList(std::string filepath, int deck);

private:
    //playlist
    TableListBox tableComponent;

    //vectors for file data
    std::vector<std::string> inputFiles;
    std::vector<std::string> trackTitles;
    std::vector<std::string> potentialFiles;
    std::vector<std::string> potentialTitles;
    std::vector<int> trackDurations;
    std::vector<int> potentialDurations;

    //processing the data
    void addToDeckList(std::string filepath, int deck);
    void getTrackLength(URL audioURL);

    //search bar and label thing
    TextEditor searchBar;
    Label searchSet;

    //manual load button
    TextButton loadButton{"LOAD"};

    //format manager stuff for just interpreting files, nothing else...
    AudioFormatManager& formatManagerToUse;
    std::unique_ptr<AudioFormatReaderSource> readerSourceToUse;
    AudioTransportSource transportSourceToUse;

    FileChooser fChooser{ "Select file..." };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

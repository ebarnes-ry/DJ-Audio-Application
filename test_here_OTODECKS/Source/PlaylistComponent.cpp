/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 4 Sep 2023 10:56:02pm
    Author:  Emmab

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(AudioFormatManager& _formatManagerToUse):formatManagerToUse(_formatManagerToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(tableComponent);

    tableComponent.getHeader().addColumn("TRACK TITLE", 1, 250);
    tableComponent.getHeader().addColumn("DURATION", 2, 100);
    tableComponent.getHeader().addColumn("ADD to Deck 1", 3, 100);
    tableComponent.getHeader().addColumn("ADD to Deck 2", 4, 100);


    tableComponent.setModel(this);

    addAndMakeVisible(searchSet);
    searchSet.setText("Search", juce::dontSendNotification);

    addAndMakeVisible(searchBar);
    searchBar.addListener(this);

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
}

int PlaylistComponent::getNumRows() {
    return potentialTitles.size();
}

//=====================================================================
void PlaylistComponent::paintRowBackground(Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected) {
    g.fillAll(rowIsSelected ? Colours::orange : Colours::darkgrey);
}
void PlaylistComponent::paintCell(Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{
    if (rowNumber >= 0 && rowNumber < potentialTitles.size()) {
        if (columnId == 1) {
            g.drawText(potentialTitles[rowNumber],
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
        if (columnId == 2) {
            g.drawText(std::to_string(potentialDurations[rowNumber]) + "s",
                1, rowNumber,
                width - 4, height,
                Justification::centredLeft,
                true);
        }

    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate) 
{
    //create buttons to add the row's data to the first deck and second deck
    if (columnId == 3 || columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ (columnId == 3) ? "<--" : "-->" };
            String id{ std::to_string(rowNumber) + ((columnId == 4) ? "RIGHT" : "LEFT") };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->setColour(TextButton::buttonColourId, Colours::darkslateblue);
        }
    }
    
    return existingComponentToUpdate;
}

//=====================================================================

void PlaylistComponent::buttonClicked(Button* button) {
    String id = button->getComponentID().toStdString();//get button id

    //differentiate buttons to process differently
    if (id.contains("RIGHT")) {//if it's the right or second button
        addToDeckList(potentialFiles[id.replace("RIGHT", "").getIntValue()], 1);
        //DBG("right side just added: " + (potentialFiles[id.replace("RIGHT", "").getIntValue()]));
    }
    else if(id.contains("LEFT")) {//if it's the left or first button
        addToDeckList(potentialFiles[id.getIntValue()], 0);
        //DBG("left side just added: " + (potentialFiles[id.getIntValue()]));
    }
    
    if (button == &loadButton) {//load button gets pressed
        if (id.contains("RIGHT")) {//right side, deck 1, playlist 2
            promptLoadButton(1);
        }
        else {//left side, deck 0, playlist 1
            promptLoadButton(0);
        }
    }
    
}


//=====================================================================

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files) {
    return true; //we want it to!
}
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y) {
    //whenever files are dropped here
    for (const String& filename : files) {
        File file(filename);
        std::string filepath = filename.toStdString();
        //DBG("filepath:  " + filepath);

        if (file.existsAsFile()) {
            //std::string fileExtension = file.getFileExtension().toStdString();
            std::string fileName = file.getFileNameWithoutExtension().toStdString();
            //DBG("file     " + fileName);

            inputFiles.push_back(filepath);
            trackTitles.push_back(fileName);

            // Since you're adding the file path, you can directly call getTrackLength
            getTrackLength(URL{ file });
        }
    }
    //initialize "potential files/titles" to the current list for searching
    potentialFiles = inputFiles;
    potentialTitles = trackTitles;
    //also do duration here
    //potentialDurations = trackDurations;

    //update lib table
    tableComponent.updateContent();
}

//====================================================================
void PlaylistComponent::textEditorTextChanged(TextEditor& textEditor) {
    // Clear vectors used in table when search changes
    potentialFiles.clear();
    potentialTitles.clear();
    potentialDurations.clear();

    // Begin at 0 of the original and pick out which ones work with the search params
    juce::String searchText = searchBar.getText().trim().toLowerCase();
    for (int i = 0; i < inputFiles.size(); ++i) {
        if (i < trackTitles.size() && i < trackDurations.size()) {
            if (juce::String(trackTitles[i]).toLowerCase().contains(searchText)) {
                potentialFiles.push_back(inputFiles[i]);
                potentialTitles.push_back(trackTitles[i]);
                potentialDurations.push_back(trackDurations[i]);
            }
        }
    }

    tableComponent.updateContent();
}
//=====================================================================

void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {}
void PlaylistComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {}
void PlaylistComponent::releaseResources(){}

//====================================================================

void PlaylistComponent::promptLoadButton(int deck) {
    // Open a file dialog to let the user choose a file
    auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
    if (deck == 0) {
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser) {
            DBG("prompting the load");
            std::string filepath = File{ chooser.getResult() }.getFullPathName().toStdString();
            // Add the selected file to the playlist
            DBG("adding track to playlist 1");
            addToDeckList(filepath, 0);
            });
    }
    if(deck == 1) {
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser) {
            std::string filepath = File{ chooser.getResult() }.getFullPathName().toStdString();
            // Add the selected file to the playlist
            addToDeckList(filepath, 1);
         });
    }

    tableComponent.updateContent();
}

void PlaylistComponent::addToDeckList(std::string filepath, int deck) {
    if (deck == 0) {//1
        playlist1.push_back(filepath);
        //DBG("playlist 1 just added " + filepath);

        //send this new file to this deck's queue object

    }
    if (deck == 1) {//2
        playlist2.push_back(filepath);
        //DBG("playlist 2 just added " + filepath);
    }

    //update lib table
    tableComponent.updateContent();
}


void PlaylistComponent::getTrackLength(URL audioURL) {
    double trackLength = 0.0;
    
    auto* inputReader = formatManagerToUse.createReaderFor(audioURL.createInputStream(false));
    if (inputReader != nullptr) {//good file
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(inputReader, true));
        transportSourceToUse.setSource(newSource.get(), 0, nullptr, inputReader->sampleRate);
        readerSourceToUse.reset(newSource.release());
        trackLength = transportSourceToUse.getLengthInSeconds();
        trackDurations.push_back(trackLength);
    }

    //return trackLength;
    potentialDurations = trackDurations;
}

//====================================================================

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0, getHeight() / 8, getWidth(), getHeight() / 8 * 6);

    searchSet.setBounds(0, 0, getWidth() / 6, getHeight() / 8);
    searchBar.setBounds(getWidth() / 6, 0, getWidth() / 6 * 4, getHeight() / 8);
    loadButton.setBounds(getWidth() - getWidth()/6, 0, getWidth() / 6, getHeight() / 8);
}

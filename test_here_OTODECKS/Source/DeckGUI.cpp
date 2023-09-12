/*
  ==============================================================================

    DeckGUI.cpp
    Created: 31 Aug 2023 9:38:44pm
    Author:  Emmab

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    PlaylistComponent* _playlistComponent,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse,
    int deckToUse):
    player(_player),
    playlistComponent(_playlistComponent),
    waveformDisplay(formatManagerToUse, cacheToUse),
    deck(deckToUse)
{
    //display stuff
    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(cueButton);

    addAndMakeVisible(positionSlider);
    addAndMakeVisible(speedSlider);

    //look and feel PROBLEM HERE
    //speedSlider.setLookAndFeel(&customElementLook);
    //positionSlider.setLookAndFeel(&customElementLook);


    //add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    cueButton.addListener(this);

    positionSlider.addListener(this);
    speedSlider.addListener(this);

    //more mods
    positionSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    positionSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //specify speed spec stuff
    speedSlider.setRange(0.5, 2, 0);//min half speed, max is 2x speed
    speedSlider.setMouseDragSensitivity(80);


    startTimer(200);//timercallback is called every 200 ms so updates 5fps

    addMouseListener(this, true); // Register for mouse events

    //initialize emptyCues
    emptyCue1 = true;
    emptyCue2 = true;
}

DeckGUI::~DeckGUI()
{
    //reset look and feel of things that use it
    //speedSlider.setLookAndFeel(nullptr);
    //positionSlider.setLookAndFeel(nullptr);

    stopTimer();
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files) {
    std::cout << "DECKGUI isInterestedInFileDrag" << std::endl;
    return true;
}
void DeckGUI::filesDropped(const StringArray& files, int x, int y) {
    
    for (String filename : files) {
        std::cout << "files dropped" << std::endl;
        URL fileURL = URL{ File{filename} };
        player->loadURL(fileURL);
        return;
    }
    
}

void DeckGUI::buttonClicked(Button* button) {
    if (button == &playButton) {
        //player->setPosition(0);
        player->play();
    }
    if (button == &stopButton) {
        player->stop();
    }

    if (button == &cueButton) {
        //DBG("cue was pressed");
        if (deck == 0) {
            if (playlistComponent->playlist1.empty()) {
                emptyCue1 = true;
                playlistComponent->promptLoadButton(0);
            }
            else {
                // Ensure there are tracks
                if (!playlistComponent->playlist1.empty()) {
                    // Get the URL
                    URL fileURL = URL{ File{playlistComponent->playlist1[0]} };

                    // Load it to the player
                    player->loadURL(fileURL);

                    // Display the waveform
                    waveformDisplay.loadURL(fileURL);

                    // Remove the first URL to avoid replay
                    playlistComponent->playlist1.erase(playlistComponent->playlist1.begin());
                }
            }

            if (emptyCue1) {//once first songs are loaded, this can just keep playing
                emptyCue1 = false;
            }
            else {
                player->play();
            }
        }

        if (deck == 1) {
            if (playlistComponent->playlist2.empty()) {
                emptyCue2 = true;
                playlistComponent->promptLoadButton(1);
            }
            else {
                // Ensure there are tracks
                if (!playlistComponent->playlist2.empty()) {
                    // Get the URL
                    URL fileURL = URL{ File{playlistComponent->playlist2[0]} };

                    // Load it to the player
                    player->loadURL(fileURL);

                    // Display the waveform
                    waveformDisplay.loadURL(fileURL);

                    // Remove the first URL to avoid replay
                    playlistComponent->playlist2.erase(playlistComponent->playlist2.begin());
                }
            }
            if (emptyCue2) {//once first songs are loaded, this can just keep playing
                emptyCue2 = false;
            }
            else {
                player->play();
            }

        }
    }
}

    /*
    if (button == &loadButton) {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser) {
            player->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{ chooser.getResult() });
         });
    }
    */

/**if mouse is pressed on waveform, then position in song gets changed*/
void DeckGUI::mouseDown(const MouseEvent& event) {
    if (event.eventComponent == &waveformDisplay) {
        //DBG("Mouse clicked on DeckGUI");
        //player->stop(); //is it even working??
        // Calculate the relative position within the waveform display where the user clicked
        double clickX = event.position.x;
        double waveformWidth = waveformDisplay.getWidth();
        double relativePosition = clickX / waveformWidth;

        // Set the playback position relative to the waveform display
        player->setPositionRelative(relativePosition);
        positionSlider.setValue(relativePosition);
    }
}


void DeckGUI::sliderValueChanged(Slider* slider) {
    if (slider == &positionSlider) {
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());      
        //DBG("speed val changed     " + std::to_string(slider->getValue()));
    }
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}



void DeckGUI::paint (juce::Graphics& g)
{
    double margin = 4;
    double columnWidth = getWidth() / 6;
    double rowHeight = getWidth() / 12;

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // Draw a thick border
    g.setColour(juce::Colours::black); // Set border color to black
    g.fillRect(0, 0, getWidth(), 4);
    g.fillRect(0, 0, 4, getHeight()); // Left border
    g.fillRect(getWidth()-4, 0, 4, getHeight()); // Right border
    g.fillRect(0, getHeight() - 4, getWidth(), 4); // Bottom border

    //box around waveform
    g.setColour(juce::Colours::black);
    g.fillRect(0, 0, getWidth(), rowHeight*5+margin);
}

void DeckGUI::resized()
{

    double columnWidth = getWidth() / 6;
    double rowHeight = getWidth() / 12;

    waveformDisplay.setBounds(columnWidth*0.25, rowHeight*0.5, columnWidth * 6 - columnWidth * 0.5, rowHeight * 4);
    //positionSlider.setBounds(columnWidth, rowHeight * 9, columnWidth * 2, rowHeight * 4);
    
    
    if (deck == 0) {
        positionSlider.setBounds(columnWidth * 0.15, rowHeight * 5.5, columnWidth * 5, rowHeight * 8);
        speedSlider.setBounds(columnWidth * 5, rowHeight * 8, columnWidth, rowHeight * 8);
        playButton.setBounds(columnWidth * 0.5, rowHeight * 15, columnWidth, rowHeight * 2);
        stopButton.setBounds(columnWidth * 2, rowHeight * 15, columnWidth, rowHeight * 2);
        cueButton.setBounds(columnWidth * 3.5, rowHeight * 15, columnWidth, rowHeight * 2);
    }
    if(deck==1){
        positionSlider.setBounds(columnWidth, rowHeight * 5.5, columnWidth * 5, rowHeight * 8);
        speedSlider.setBounds(0, rowHeight * 8, columnWidth, rowHeight * 8);
        playButton.setBounds(columnWidth*4.5, rowHeight * 15, columnWidth, rowHeight * 2);
        stopButton.setBounds(columnWidth * 3, rowHeight * 15, columnWidth, rowHeight * 2);
        cueButton.setBounds(columnWidth * 1.5, rowHeight * 15, columnWidth, rowHeight * 2);
    }
    
    
    //speedSlider.setBounds(columnWidth * 2, rowHeight * 8, columnWidth, rowHeight * 8);
    //playButton.setBounds(columnWidth * 0.5, rowHeight * 14, columnWidth, rowHeight * 2);
}

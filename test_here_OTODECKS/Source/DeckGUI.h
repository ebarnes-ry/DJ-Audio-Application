/*
  ==============================================================================

    DeckGUI.h
    Created: 31 Aug 2023 9:38:44pm
    Author:  Emmab

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//#include "CustomPosSlider.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer
{
public:
    DeckGUI(DJAudioPlayer* _player,
        PlaylistComponent* playlistComponent,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse,
        int deckToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(Button* button)override;
    void sliderValueChanged(Slider* slider)  override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;

    void mouseDown(const MouseEvent& event) override;

    //FileChooser fChooser{ "Select file..." };

private:
    TextButton playButton{ "PLAY" };
    TextButton stopButton{"STOP"};
    TextButton cueButton{ "CUE" };

    Slider positionSlider;
    Slider speedSlider;
    

    //FileChooser fChooser{ "Select file..." };

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    PlaylistComponent* playlistComponent;

    bool emptyCue1;
    bool emptyCue2;
    //TODO:
    //void setEmptyCue(bool emptyCue);

    //CustomPosSlider customElementLook;

    int deck;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};

/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "mixerComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;


private:
    //==============================================================================
    // Your private member variables go here...
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{ 100 };

    int deckL = 0;
    int deckR = 1;


    DJAudioPlayer player1{ formatManager };
    DeckGUI deck1{ &player1 , &playlistComponent,formatManager, thumbCache, deckL};
    
    DJAudioPlayer player2{ formatManager };
    DeckGUI deck2{ &player2 , &playlistComponent, formatManager, thumbCache, deckR};

    mixerComponent mixer;

    MixerAudioSource mixerSource;
    PlaylistComponent playlistComponent{ formatManager };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

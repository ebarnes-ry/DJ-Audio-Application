/*
  ==============================================================================

    mixerComponent.h
    Created: 9 Sep 2023 8:48:49pm
    Author:  Emmab

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class mixerComponent  : public juce::Component,
                        public juce::Slider::Listener
{
public:
    mixerComponent();
    ~mixerComponent() override;

    void setDeck1(DJAudioPlayer* deckPlayerSource1);
    void setDeck2(DJAudioPlayer* deckPlayerSource2);

    void sliderValueChanged(Slider* slider) override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    Slider crossfadeSlider;
    Slider volumeSlider1;
    Slider volumeSlider2;

    Slider dampSlider;
    Slider wetSlider;
    Slider drySlider;

    ToggleButton enableReverbToggle;

    Label crossfadeLabel;
    Label volumeLabel1;
    Label volumeLabel2;
    
    void crossFade(double sliderVal);

    DJAudioPlayer* deckPlayer1;
    DJAudioPlayer* deckPlayer2;

    CustomLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mixerComponent)
};

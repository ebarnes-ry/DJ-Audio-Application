/*
  ==============================================================================

    mixerComponent.cpp
    Created: 9 Sep 2023 8:48:49pm
    Author:  Emmab

  ==============================================================================
*/

#include <JuceHeader.h>
#include "mixerComponent.h"


//==============================================================================
mixerComponent::mixerComponent()
{

    //customLookAndFeel = std::make_unique<CustomLookAndFeel>();

    //slider and knob setup
    addAndMakeVisible(crossfadeSlider);
    addAndMakeVisible(volumeSlider1);
    addAndMakeVisible(volumeSlider2);
    

    //listener setup
    crossfadeSlider.addListener(this);
    volumeSlider1.addListener(this);
    volumeSlider2.addListener(this);

    //range setup
    
    volumeSlider1.setRange(0.0, 1.0);
    volumeSlider2.setRange(0.0, 1.0);
    crossfadeSlider.setRange(0.0, 1.0);

    crossfadeSlider.setValue(0.5); //initialize to .5
    volumeSlider1.setValue(0.5);
    volumeSlider2.setValue(0.5);

    //more style setup
    volumeSlider1.setSliderStyle(Slider::SliderStyle::LinearVertical);
    volumeSlider2.setSliderStyle(Slider::SliderStyle::LinearVertical);
    crossfadeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);

    //remove the stuff
    crossfadeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    volumeSlider1.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    volumeSlider2.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    //set look and feel
    crossfadeSlider.setLookAndFeel(&customLookAndFeel);
    volumeSlider1.setLookAndFeel(&customLookAndFeel);
    volumeSlider2.setLookAndFeel(&customLookAndFeel);
    

    //set labels
    crossfadeLabel.setText("FADE", juce::dontSendNotification);
    crossfadeLabel.attachToComponent(&crossfadeSlider, false);
    crossfadeLabel.setJustificationType(juce::Justification::centred);
    volumeLabel1.setText("VOL1", juce::dontSendNotification);
    volumeLabel1.attachToComponent(&volumeSlider1, false);
    volumeLabel1.setJustificationType(juce::Justification::centred);
    volumeLabel2.setText("VOL2", juce::dontSendNotification);
    volumeLabel2.attachToComponent(&volumeSlider2, false);
    volumeLabel2.setJustificationType(juce::Justification::centred);

    //deck pointer setup
    deckPlayer1 = nullptr;
    deckPlayer2 = nullptr;
}

mixerComponent::~mixerComponent()
{
    // Make sure to set the LookAndFeel to nullptr for components using it
    volumeSlider1.setLookAndFeel(nullptr);
    volumeSlider2.setLookAndFeel(nullptr);
    crossfadeSlider.setLookAndFeel(nullptr);
}

void mixerComponent::setDeck1(DJAudioPlayer* deckPlayerSource1) {
    deckPlayer1 = deckPlayerSource1;
}

void mixerComponent::setDeck2(DJAudioPlayer* deckPlayerSource2) {
    deckPlayer2 = deckPlayerSource2;
}

void mixerComponent::sliderValueChanged(Slider* slider) {
    if (slider == &volumeSlider1) {
        deckPlayer1->setGain(slider->getValue());
    }
    if (slider == &volumeSlider2) {
        deckPlayer2->setGain(slider->getValue());
    }
    if (slider == &crossfadeSlider) {
        //calls crossfade logic
        crossFade(slider->getValue());
    }
}

void mixerComponent::crossFade(double sliderVal) {
    double gain1 = 1.0 - sliderVal;
    double gain2 = sliderVal;

    deckPlayer1->setGain(gain1);
    deckPlayer2->setGain(gain2);

    volumeSlider1.setValue(gain1);
    volumeSlider2.setValue(gain2);
}

void mixerComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void mixerComponent::resized()
{
    double columnWidth = getWidth() / 6;
    double rowHeight = getHeight() / 12;
    crossfadeSlider.setBounds(columnWidth, rowHeight, columnWidth*4, rowHeight);
    volumeSlider1.setBounds(columnWidth - columnWidth/2, rowHeight * 3, columnWidth, rowHeight * 8);
    volumeSlider2.setBounds(columnWidth * 5, rowHeight * 3, columnWidth, rowHeight * 8);
}

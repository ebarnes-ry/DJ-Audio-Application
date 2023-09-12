/*
  ==============================================================================

    WaveformDisplay.h
    Created: 2 Sep 2023 4:47:16pm
    Author:  Emmab

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
    public juce::ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse, 
        AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void loadURL(URL audioURL);

    void changeListenerCallback(ChangeBroadcaster* source) override;

    /**sets the relative pos of the playhead*/
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumbnail;
    bool fileLoaded;
    double position;

    //DJAudioPlayer& player;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};

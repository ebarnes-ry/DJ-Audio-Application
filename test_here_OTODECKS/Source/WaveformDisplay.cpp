/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 2 Sep 2023 4:47:16pm
    Author:  Emmab

  ==============================================================================
*/

#include <../JuceLibraryCode/JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse):
    audioThumbnail(1000, formatManagerToUse, cacheToUse), 
    fileLoaded(false),
    position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumbnail.addChangeListener(this);
    addMouseListener(this, true);
}


WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::loadURL(URL audioURL) {
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new URLInputSource(audioURL)); //**new == dynamically loaded object
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source) {
    std::cout << "changed" << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position) {
        position = pos;
        repaint();
    }
}


void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    if (fileLoaded) {
        if (audioThumbnail.getTotalLength() > 0) {
            float playPos = position;//make sure it's a float
            double totalLength = audioThumbnail.getTotalLength();
            double currentTime = playPos * totalLength;

            // lkeft side
            juce::Rectangle<int> leftBounds(0, 0, playPos * getWidth(), getHeight());

            // right side
            juce::Rectangle<int> rightBounds(playPos * getWidth(), 0, getWidth() - playPos * getWidth(), getHeight());

            // left side as it moves
            g.setColour(Colours::cornflowerblue);
            audioThumbnail.drawChannel(g, leftBounds, 0, playPos * audioThumbnail.getTotalLength(), 0, 1.0f);

            // right side as it moves
            g.setColour(Colours::beige);
            audioThumbnail.drawChannel(g, rightBounds, playPos * audioThumbnail.getTotalLength(), audioThumbnail.getTotalLength(), 0, 1.0f);

            // Draw the playhead 
            g.setColour(Colours::darkred);
            g.drawLine(playPos * getWidth(), 0, playPos * getWidth(), getHeight(), 1.2f);

            // Display timestamp above the playhead
            String timestamp = Time(currentTime).formatted("%M:%S");
            g.setColour(Colours::black);
            g.drawText(timestamp, playPos * getWidth() - 50, 10, 100, 20, Justification::centred);
        }
    }
    else {
        g.setFont(20.0f);
        g.drawText("Not loaded yet", getLocalBounds(), Justification::centred, true);
    }
}


void WaveformDisplay::resized()
{

}

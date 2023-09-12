/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    mixer.setDeck1(&player1);
    mixer.setDeck2(&player2);

    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(mixer);


    formatManager.registerBasicFormats();

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    playlistComponent.prepareToPlay(samplesPerBlockExpected, sampleRate);

    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()

    playlistComponent.releaseResources();


    mixerSource.removeAllInputs();
    mixerSource.releaseResources();

    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    deck1.setBounds(0, 0, getWidth()/3, getHeight()/3*2);
    deck2.setBounds(getWidth()/3*2, 0, getWidth()/3, getHeight()/3*2);
    mixer.setBounds(getWidth()/3, 0, getWidth()/3, getHeight()/3*2);
    playlistComponent.setBounds(0, getHeight()/3*2, getWidth(), getHeight()/3);
}


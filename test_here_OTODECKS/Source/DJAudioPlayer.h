/*
  ==============================================================================

	DJAudioPlayer.h
	Created: 30 Aug 2023 10:29:24pm
	Author:  Emmab

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
public:
	DJAudioPlayer(AudioFormatManager& formatManager);
	~DJAudioPlayer();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================

	/**loads files*/
	void loadURL(URL file);
	/**plays audio*/
	void play();
	/**stops audio*/
	void stop();
	/**sets position of time window*/
	void setPosition(double posInSecs);
	/**sets gain*/
	void setGain(double gain);
	/**sets speed*/
	void setSpeed(double ratio);
	/**sets position relative*/
	void setPositionRelative(double pos);

	double getPositionRelative();


	/*==============================
	//REVERB TEST STUFF
	// Functions for setting reverb parameters
	void setReverbRoomSize(float roomSize);
	void setReverbDamping(float damping);
	void setReverbWetLevel(float wetLevel);
	void setReverbDryLevel(float dryLevel);

	void setReverbEnabled(bool enableReverb);
	*/
	//=====================================

private:
	AudioFormatManager& formatManager;
	AudioTransportSource transportSource;
	std::unique_ptr<AudioFormatReaderSource> readerSource;
	ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

	//=========================================
	// Reverb related members
	//Reverb reverb;
	//Reverb::Parameters reverbParameters;

	//bool reverbEnabled;

};
/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 30 Aug 2023 10:29:24pm
    Author:  Emmab

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager):formatManager(_formatManager) {
    //formatManager.registerBasicFormats();
    
    /*REVERB TEST STUFF============== =
    // Initialize the reverb object with default parameters
    reverbParameters.roomSize = 0.5f;
    reverbParameters.damping = 0.5f;
    reverbParameters.wetLevel = 0.5f;
    reverbParameters.dryLevel = 1.0f;

    reverb.setParameters(reverbParameters);
    //====================================*/
}

DJAudioPlayer::~DJAudioPlayer() {

}
//================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    resampleSource.getNextAudioBlock(bufferToFill);

    /*REVERB STUFF REVERB STUFF
    //================================
    if (reverbEnabled) {
        // Apply reverb to the audio if reverb is enabled
        const int numChannels = bufferToFill.buffer->getNumChannels();
        for (int channel = 0; channel < numChannels; ++channel) {
            float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

            // Apply reverb to the channel data
            reverb.processStereo(channelData, channelData, bufferToFill.numSamples);
        }
    }
    //===================================*/
} 

//REVERB REVERB
//=====================
//void DJAudioPlayer::setReverbEnabled(bool enableReverb) {
    //reverbEnabled = enableReverb;
//}
//=========================

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}
//=======================================================

void DJAudioPlayer::loadURL(URL audioURL) {
    //readerSource.reset(); // Release previous resources
    //auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    auto* reader = this->formatManager.createReaderFor(audioURL.createInputStream(URL::InputStreamOptions{ URL::ParameterHandling::inAddress }));
    if (reader != nullptr) {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
    else {
        std::cout << "Failed to create reader for the audio file." << std::endl;
    }
}

void DJAudioPlayer::play() {
    transportSource.start();
}

void DJAudioPlayer::stop() {
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos>1.0) {
        std::cout << "issue with range" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain>1.0) {
    }
    else {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio>100.0) {
        transportSource.stop();
    }
    else {
        DBG("Speed is being set in player");
        resampleSource.setResamplingRatio(ratio);
    }
}



double DJAudioPlayer::getPositionRelative() {
    if (transportSource.getLengthInSeconds() == 0) {
        return 0;
    }
    else {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
}


/*=============================
//reverb stuff reverb stuff reverb stuff
// Functions to set reverb parameters
void DJAudioPlayer::setReverbRoomSize(float roomSize) {
    reverbParameters.roomSize = roomSize;
    reverb.setParameters(reverbParameters);
}

void DJAudioPlayer::setReverbDamping(float damping) {
    reverbParameters.damping = damping;
    reverb.setParameters(reverbParameters);
}

void DJAudioPlayer::setReverbWetLevel(float wetLevel) {
    reverbParameters.wetLevel = wetLevel;
    reverb.setParameters(reverbParameters);
}

void DJAudioPlayer::setReverbDryLevel(float dryLevel) {
    reverbParameters.dryLevel = dryLevel;
    reverb.setParameters(reverbParameters);
}
//===========================================*/
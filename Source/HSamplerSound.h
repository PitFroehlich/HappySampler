/*
  ==============================================================================

    HSamplerSound.h
    Created: 26 Jul 2022 10:59:53am
    Author:  pitfr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class HSamplerSound : public juce::SamplerSound {

private:
    //friend class for custom sub class
    friend class HSamplerVoice;

    //private Variables from base class
    juce::String name;
    std::unique_ptr<juce::AudioBuffer<float>> data;
    double sourceSampleRate;
    juce::BigInteger midiNotes;
    int length = 0, midiRootNote = 0;

    juce::ADSR::Parameters params;

    JUCE_LEAK_DETECTOR(HSamplerSound)
};
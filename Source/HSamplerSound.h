/*
  ==============================================================================

    HSamplerSound.h
    Created: 26 Jul 2022 2:07:20pm
    Author:  pitfr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GainControl.h"

class HSamplerSound : public juce::SamplerSound
{
public:
    HSamplerSound(const juce::String& name, juce::AudioFormatReader& source, const juce::BigInteger& midiNotes, int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds) : SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds),
        sourceSampleRate(source.sampleRate), length(source.lengthInSamples), midiRootNote(midiNoteForNormalPitch) {}

    void setGainControlParameters(GainControl::Parameters parametersToUse) { gainParams = parametersToUse; }

private:
    friend class HSamplerVoice;

    juce::String name;
    std::unique_ptr<juce::AudioBuffer<float>> data;
    double sourceSampleRate;
    juce::BigInteger midiNotes;
    int length = 0, midiRootNote = 0;


    juce::ADSR::Parameters params;
    GainControl::Parameters gainParams;

    juce::ADSR adsr;
    GainControl gaincontrol;

};

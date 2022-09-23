/*
  ==============================================================================

    HSamplerSound3.h
    Created: 16 Sep 2022 3:07:39pm
    Author:  pitfr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GainControl.h"

class HSamplerSound3 : public juce::SamplerSound
{
public:
    HSamplerSound3(const juce::String& name, juce::AudioFormatReader& source, const juce::BigInteger& midiNotes, int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds) : SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds),
        sourceSampleRate(source.sampleRate), length(source.lengthInSamples), midiRootNote(midiNoteForNormalPitch) {}

private:
    friend class HSamplerVoice3;

    juce::String name;
    std::unique_ptr<juce::AudioBuffer<float>> data;
    double sourceSampleRate;
    juce::BigInteger midiNotes;
    int length{ 0 }, midiRootNote{ 0 };


    juce::ADSR::Parameters params;

    GainControl::Parameters gainParams;

};

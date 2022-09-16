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
#include "HSamplerSound.h"

class HSamplerSound1 : public HSamplerSound
{
    HSamplerSound1(const juce::String& name, juce::AudioFormatReader& source, const juce::BigInteger& midiNotes, int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds) : SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds),
        sourceSampleRate(source.sampleRate), length(source.lengthInSamples), midiRootNote(midiNoteForNormalPitch) {}

private:
    friend class HSamplerVoice1;

};

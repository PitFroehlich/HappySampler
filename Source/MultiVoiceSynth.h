#pragma once
#include <JuceHeader.h>
class MultiVoiceSynth : public juce::Synthesiser {
    virtual void noteOn(int midiChannel,
        int midiNoteNumber,
        float velocity);
};
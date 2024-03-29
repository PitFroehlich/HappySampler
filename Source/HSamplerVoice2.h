/*
  ==============================================================================

    HSamplerVoice2.h
    Created: 1 Aug 2022 10:50:05am
    Author:  pitfr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GainControl.h"


class HSamplerVoice2 : public juce::SamplerVoice {
public:
	bool canPlaySound(juce::SynthesiserSound*) override;

	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;

	void stopNote(float velocity, bool allowTailOff) override;

	//void pitchWheelMoved(int newValue) override;

	//void controllerMoved(int controllerNumber, int newValue) override;
	void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;

	void setGainControlParameters(GainControl::Parameters parametersToUse) { gainParams = parametersToUse; }

//==============================================================================
private:
	//Variables from samplerVoice base class
	double pitchRatio{ 0 };
	double sourceSamplePosition{ 0 };
	float lgain{ 0 }, rgain{ 0 };
	double filterCutoff;
	double levelSlider;
	int length{ 0 }, midiRootNote{ 0 };

	juce::ADSR adsr;

	GainControl gainControl;

	GainControl::Parameters gainParams;

	JUCE_LEAK_DETECTOR(HSamplerVoice2)

};

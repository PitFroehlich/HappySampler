/*
  ==============================================================================

	HSamplerVoice.h
	Created: 26 Jul 2022 1:38:31pm
	Author:  pitfr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GainControl.h"


class HSamplerVoice : public juce::SamplerVoice {
public:


	bool canPlaySound(juce::SynthesiserSound*) override;

	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int pitchWheel) override;

	void stopNote(float velocity, bool allowTailOff) override;

	//void pitchWheelMoved(int newValue) override;

	//void controllerMoved(int controllerNumber, int newValue) override;
	void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
	
	void setGainControlParameters(GainControl::Parameters parametersToUse) {
		gainParams = parametersToUse;  
	}
	
//==============================================================================
private:
	int counter{ 0 };
	int loopMidiNoteNumber;
	float loopVelocity;
	juce::SynthesiserSound* loopS;
	int loopPitchWheelPosition;

	//Variables from samplerVoice base class
	double pitchRatio{ 0 };
	double sourceSamplePosition{ 0 };
	float lgain = 0, rgain{ 0 };
	double filterCutoff;
	double levelSlider;
	int length{ 0 }, midiRootNote{ 0 };

	juce::ADSR adsr;

	GainControl gainControl;

	GainControl::Parameters gainParams;


	JUCE_LEAK_DETECTOR(HSamplerVoice)

};

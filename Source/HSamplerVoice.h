/*
  ==============================================================================

	HSamplerVoice.h
	Created: 26 Jul 2022 1:38:31pm
	Author:  pitfr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class HSamplerVoice : public juce::SamplerVoice {
public:
	bool canPlaySound(juce::SynthesiserSound*) override;

	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;

	void stopNote(float velocity, bool allowTailOff) override;

	//void pitchWheelMoved(int newValue) override;

	//void controllerMoved(int controllerNumber, int newValue) override;
	void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
	//using SynthesiserVoice::renderNextBlock;

	double gainControl1{ 1 };
	
//==============================================================================
private:
	//Variables from samplerVoice base class
	double pitchRatio = 0;
	double sourceSamplePosition = 0;
	float lgain = 0, rgain = 0;
	double filterCutoff;
	double levelSlider;
	int length = 0, midiRootNote = 0;

	juce::ADSR adsr;

	JUCE_LEAK_DETECTOR(HSamplerVoice)

};

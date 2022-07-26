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
	void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
	using SynthesiserVoice::renderNextBlock;
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

};

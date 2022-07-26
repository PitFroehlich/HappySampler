/*
  ==============================================================================

	HSamplerVoice.h
	Created: 25 Jul 2022 4:24:09pm
	Author:  pitfr

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class HSamplerVoice : public juce::SamplerVoice {
private:
	//private variables from base class
	double pitchRatio = 0;
	double sourceSamplePosition = 0;
	float lgain = 0, rgain = 0;
	double filterCutoff;
	double levelSlider;
	

	juce::ADSR adsr;

public:
	void renderNextBlock(
		juce::AudioBuffer<float>&,
		int startSample,
		int numSamples) override;
};
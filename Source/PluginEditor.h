/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "HSamplerVoice.h"
#include "HSamplerVoice2.h"


//==============================================================================
/**
*/
class HappySamplerAudioProcessorEditor : public juce::AudioProcessorEditor,
										 public juce::Slider::Listener

{
public:
	HappySamplerAudioProcessorEditor(HappySamplerAudioProcessor&);
	~HappySamplerAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	void sliderValueChanged(juce::Slider* slider) override;
	void sliderGainControl1Changed(juce::Slider* slider);
	void sliderGainControl2Changed(juce::Slider* slider);

private:
	juce::TextButton loadButton{ "Load1" };
	juce::TextButton loadButton2{ "Load2" };
	juce::TextButton exportButton{ "Export" };
	juce::TextButton buttonApply{ "Apply" };

	juce::Slider sliderChangeSample{ "Sample Start" };
	juce::Slider sliderGainControl1{ "Gain Control" };
	juce::Slider sliderGainControl2{ "Gain Control" };

	double sliderChangeSampleValue;



	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	HappySamplerAudioProcessor& audioProcessor;
	HSamplerVoice& hSamplerVoice;
	HSamplerVoice2& hSamplerVoice2;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HappySamplerAudioProcessorEditor)
};

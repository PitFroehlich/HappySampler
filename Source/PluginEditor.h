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
#include "GainControl.h"


//==============================================================================
/**
*/
class HappySamplerAudioProcessorEditor : 
	public juce::AudioProcessorEditor,
	public juce::Slider::Listener


{
public:
	HappySamplerAudioProcessorEditor(HappySamplerAudioProcessor&);
	~HappySamplerAudioProcessorEditor() override;



	void paint(juce::Graphics&) override;
	void resized() override;

	void sliderValueChanged(juce::Slider* slider) override;

	void setPaintWaveFormToTrue();

	void waveformIsAvailableToTrue();

	void HappySamplerAudioProcessorEditor::paintAudioThumbnail(
		juce::Graphics& g,
		const juce::Rectangle<int>& audioThumbnailBounds);

	void paintPlayHead(juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailBounds);
	void paintPlayHead1(juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailBounds);
	void paintPlayHead2(juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailBounds);

private:

	//juce::AudioFormatManager audioFormatManager;

	bool waveformIsAvailable{ false };
	bool firstCall{ true };

	juce::TextButton loadButton{ "Load" };
	juce::TextButton buttonApply{ "Apply" }; //grün
	juce::TextButton buttonApply1{ "Apply" }; //blau 
	juce::TextButton buttonApply2{ "Apply" }; //rot

	juce::Slider sliderChangeSample{ "Sample Start" };
	juce::Slider sliderChangeSample1{ "Sample Start" };
	juce::Slider sliderChangeSample2{ "Sample Start" };

	juce::Slider sliderGainControl{ "Gain Control" };
	juce::Slider sliderGainControl1{ "Gain Control" };
	juce::Slider sliderGainControl2{ "Gain Control" };

	std::vector<float> audioPointsFromWaveForm;

	bool paintWaveForm{ false };

	double sliderChangeSampleValue;
	//==============================================================================
	double audioPosition{ 0 };
	double audioPosition1{ 0 };
	double audioPosition2{ 0 };
	//==============================================================================

	GainControl gainControl;


	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	HappySamplerAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HappySamplerAudioProcessorEditor)
};

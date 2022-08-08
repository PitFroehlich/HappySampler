/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "HSamplerVoice.h"
#include "HSamplerVoice2.h"

//==============================================================================
HappySamplerAudioProcessorEditor::HappySamplerAudioProcessorEditor(HappySamplerAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
	

{
	loadButton.onClick = [&]() { audioProcessor.loadFile(); };
	addAndMakeVisible(loadButton);
	
	loadButton2.onClick = [&]() { audioProcessor.loadFile2(); };
	addAndMakeVisible(loadButton2);

	exportButton.onClick = [&]() { audioProcessor.exportFile(); };
	addAndMakeVisible(exportButton);
	
	buttonApply.onClick = [&]() { 
		audioProcessor.setExportBuffer();
		audioProcessor.exportFile();};
	addAndMakeVisible(buttonApply);

	sliderChangeSample.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderChangeSample.setRange(0, 1, 0.001);
	sliderChangeSample.addListener(this);

	sliderGainControl1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderGainControl1.setRange(0, 1, 0.001);
	sliderGainControl1.addListener(this);

	sliderGainControl2.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderGainControl2.setRange(0, 1, 0.001);
	sliderGainControl2.addListener(this);
	
	addAndMakeVisible(sliderChangeSample);
	addAndMakeVisible(sliderGainControl1);
	addAndMakeVisible(sliderGainControl2);
	setSize(800, 600);

	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
}

HappySamplerAudioProcessorEditor::~HappySamplerAudioProcessorEditor()
{
}

//==============================================================================
void HappySamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
	//g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void HappySamplerAudioProcessorEditor::resized()
{
	loadButton.setBounds(getWidth() - 110, getHeight() / 2 - 300, 100, 50);
	loadButton2.setBounds(getWidth() - 220, getHeight() / 2 - 300, 100, 50);
	exportButton.setBounds(getWidth() - 330 ,getHeight() / 2 - 300, 100, 50);
	buttonApply.setBounds(getWidth() - 440,getHeight() / 2 - 300, 100, 50);
	sliderChangeSample.setBoundsRelative(0.01,0.30,0.4,0.2);
	sliderGainControl1.setBoundsRelative(0.01,0.0,0.4,0.2);
	sliderGainControl2.setBoundsRelative(0.01,0.15,0.4,0.2);
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

void HappySamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
	if (slider == &sliderChangeSample)
	{
		audioProcessor.sampleStart = sliderChangeSample.getValue()
			* audioProcessor.sampleAmountOfLoadedSample;
		DBG(audioProcessor.sampleStart;);
	}
	if (slider == &sliderGainControl1)
	{
		audioProcessor.getGainControlParameters().gainValue1 = sliderGainControl1.getValue();
	}
	if (slider == &sliderGainControl2)
	{	
		audioProcessor.getGainControlParameters().gainValue2 = sliderGainControl2.getValue();
	}
	audioProcessor.updateGainControl();
}


	






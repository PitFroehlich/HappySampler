/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HappySamplerAudioProcessorEditor::HappySamplerAudioProcessorEditor(HappySamplerAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	loadButton.onClick = [&]() { audioProcessor.loadFile(); };
	addAndMakeVisible(loadButton);
	
	loadButton2.onClick = [&]() { audioProcessor.loadFile(); };
	addAndMakeVisible(loadButton2);

	exportButton.onClick = [&]() { audioProcessor.exportFile(); };
	addAndMakeVisible(exportButton);
	
	buttonApply.onClick = [&]() {  };
	addAndMakeVisible(buttonApply);

	sliderChangeSample.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderChangeSample.setRange(0, 1, 0.001);
	
	addAndMakeVisible(sliderChangeSample);
	setSize(400, 300);

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
	g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void HappySamplerAudioProcessorEditor::resized()
{
	loadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
	loadButton2.setBounds(getWidth() / 2 - 200, getHeight() / 2 - 50, 100, 100);
	exportButton.setBounds(getWidth() / 2 + 100 ,getHeight() / 2 - 50, 100, 100);
	buttonApply.setBoundsRelative(0.5, 0.9, 0.4, 0.1);
	sliderChangeSample.setBoundsRelative(0.5,0.7,0.4,0.2);
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

void HappySamplerAudioProcessorEditor::sliderValueChanged()
{

}

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

	exportButton.onClick = [&]() { audioProcessor.exportFile(); };
	addAndMakeVisible(exportButton);
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);
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
	exportButton.setBounds(getWidth() / 2 + 100 ,getHeight() / 2 - 50, 100, 100);
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

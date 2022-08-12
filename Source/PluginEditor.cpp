/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "HSamplerVoice.h"
#include "HSamplerVoice2.h"
#include "WaveForm.h"

//==============================================================================
HappySamplerAudioProcessorEditor::HappySamplerAudioProcessorEditor(HappySamplerAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{	// Ich glaube das Problem liegt darin, dass er den Audioformatmanager aufruft, bevor die Daten da sind
	loadButton.onClick = [&]() { 
		audioProcessor.loadFile();
		setPaintWaveFormToTrue();
		repaint();  
	};

	addAndMakeVisible(loadButton);

	buttonApply.onClick = [&]() {
		audioProcessor.setExportBuffer();
		audioProcessor.exportFile();
		audioProcessor.reloadFile();
	};
	buttonApply1.onClick = [&]() {
		audioProcessor.setExportBuffer();
		audioProcessor.exportFile();
		audioProcessor.reloadFile();
	};
	buttonApply2.onClick = [&]() {
		audioProcessor.setExportBuffer();
		audioProcessor.exportFile();
		audioProcessor.reloadFile();
	};

	buttonApply.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::green);
	buttonApply1.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);
	buttonApply2.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::blue);

	addAndMakeVisible(buttonApply);
	addAndMakeVisible(buttonApply1);
	addAndMakeVisible(buttonApply2);

	sliderChangeSample.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderChangeSample.setRange(0, 1, 0.001);
	sliderChangeSample.addListener(this);
	sliderChangeSample.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::green);
	
	sliderChangeSample1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderChangeSample1.setRange(0, 1, 0.001);
	sliderChangeSample1.addListener(this);
	sliderChangeSample1.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::red);
	
	sliderChangeSample2.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderChangeSample2.setRange(0, 1, 0.001);
	sliderChangeSample2.addListener(this);
	sliderChangeSample2.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::blue);

	sliderGainControl.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderGainControl.setRange(0, 1, 0.001);
	sliderGainControl.addListener(this);
	sliderGainControl.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::green);

	sliderGainControl1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderGainControl1.setRange(0, 1, 0.001);
	sliderGainControl1.addListener(this);
	sliderGainControl1.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::red);
	
	sliderGainControl2.setSliderStyle(juce::Slider::RotaryVerticalDrag);
	sliderGainControl2.setRange(0, 1, 0.001);
	sliderGainControl2.addListener(this);
	sliderGainControl2.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::blue);



	addAndMakeVisible(sliderChangeSample);
	addAndMakeVisible(sliderChangeSample1);
	addAndMakeVisible(sliderChangeSample2);

	addAndMakeVisible(sliderGainControl);
	addAndMakeVisible(sliderGainControl1);
	addAndMakeVisible(sliderGainControl2);

	setSize(800, 600);

	//audioFormatManager = &audioProcessor.getAudioFormatManager();

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


	if (paintWaveForm == true)
	{
		juce::Rectangle<int> audioThumbnailBounds(5, getHeight() - 200, getWidth() - 20, getHeight() - 500);

		paintAudioThumbnail(g, audioThumbnailBounds);

		paintPlayHead(g, audioThumbnailBounds);
	}
}

void HappySamplerAudioProcessorEditor::resized()
{
	loadButton.setBounds(getWidth() - 110, getHeight() / 2 - 300, 100, 50);
	buttonApply.setBounds(getWidth() - 440, getHeight() / 2 - 300, 100, 50);
	buttonApply1.setBounds(getWidth() - 330, getHeight() / 2 - 300, 100, 50);
	buttonApply2.setBounds(getWidth() - 220, getHeight() / 2 - 300, 100, 50);
	sliderChangeSample.setBoundsRelative(0.01, 0.00, 0.2, 0.2);
	sliderChangeSample1.setBoundsRelative(0.01, 0.15, 0.2, 0.2);
	sliderChangeSample2.setBoundsRelative(0.01, 0.30, 0.2, 0.2);
	sliderGainControl.setBoundsRelative(0.21, 0.0, 0.2, 0.2);
	sliderGainControl1.setBoundsRelative(0.21, 0.15, 0.2, 0.2);
	sliderGainControl2.setBoundsRelative(0.21, 0.30, 0.2, 0.2);
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

void HappySamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
	if (slider == &sliderChangeSample)
	{
		audioProcessor.sampleStart = sliderChangeSample.getValue()
			* audioProcessor.sampleAmountOfLoadedSample;

		audioPosition = sliderChangeSample.getValue();
		repaint();
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

void HappySamplerAudioProcessorEditor::setPaintWaveFormToTrue()
{
	paintWaveForm = true;
}

void HappySamplerAudioProcessorEditor::paintAudioThumbnail(
	juce::Graphics& g, 
	const juce::Rectangle<int>& audioThumbnailBounds) 
{
	g.setColour(juce::Colours::white);
	g.fillRect(audioThumbnailBounds);

	g.setColour(juce::Colours::red);

	audioProcessor.audioThumbnail.drawChannels(
		g,
		audioThumbnailBounds,
		0.0,
		audioProcessor.audioThumbnail.getTotalLength(),
		1.0f);
}

void HappySamplerAudioProcessorEditor::paintPlayHead(juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailBounds)
{
	g.setColour(juce::Colours::green);

	auto drawPosition = (audioPosition) * (float)audioThumbnailBounds.getWidth()
		+ (float)audioThumbnailBounds.getX();
	g.drawLine(drawPosition, (float)audioThumbnailBounds.getY(), drawPosition,
		(float)audioThumbnailBounds.getBottom(), 4.0f);
}







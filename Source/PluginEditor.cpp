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
		waveformIsAvailable == true;
		repaint();  
	};

	addAndMakeVisible(loadButton);

	buttonApply.onClick = [&]() {
		audioProcessor.setExportBuffer(audioProcessor.audioFormatReader, audioProcessor.sampleStart);
		audioProcessor.exportFile("sample");
		audioProcessor.reloadFile(audioProcessor.soundToRemove, "sample", audioProcessor.audioFormatReader, "green");
		
		sliderChangeSample1.setBoundsRelative(0.01, 0.15, 0.2, 0.2);


		
	};
	buttonApply1.onClick = [&]() {
		audioProcessor.setExportBuffer(audioProcessor.audioFormatReader1, audioProcessor.sampleStart1);
		audioProcessor.exportFile("sample1");
		audioProcessor.reloadFile(audioProcessor.soundToRemove2, "sample1", audioProcessor.audioFormatReader1, "red");

		sliderGainControl1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
		sliderGainControl1.setRange(0, 2, 0.001);
		sliderGainControl1.addListener(this);
		sliderGainControl1.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::red);

		addAndMakeVisible(sliderGainControl1);
	};
	buttonApply2.onClick = [&]() {
		audioProcessor.setExportBuffer(audioProcessor.audioFormatReader2, audioProcessor.sampleStart2);
		audioProcessor.exportFile("sample2");
		audioProcessor.reloadFile(audioProcessor.soundToRemove3, "sample2", audioProcessor.audioFormatReader2, "blue");

		sliderGainControl2.setSliderStyle(juce::Slider::RotaryVerticalDrag);
		sliderGainControl2.setRange(0, 2, 0.001);
		sliderGainControl2.addListener(this);
		sliderGainControl2.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::blue);

		addAndMakeVisible(sliderGainControl2);
	};

	buttonApply.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::green);
	buttonApply1.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);
	buttonApply2.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::blue);

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
	sliderGainControl.setRange(0, 2, 0.001);
	sliderGainControl.addListener(this);
	sliderGainControl.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::green);


	
	
	

	addAndMakeVisible(sliderGainControl);


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


	if (paintWaveForm == true)
	{
		juce::Rectangle<int> audioThumbnailBounds(5, getHeight() - 200, getWidth() - 20, getHeight() - 500);

		paintAudioThumbnail(g, audioThumbnailBounds);

		paintPlayHead(g, audioThumbnailBounds);
		paintPlayHead1(g, audioThumbnailBounds);
		paintPlayHead2(g, audioThumbnailBounds);

		sliderChangeSample.setBoundsRelative(0.01, 0.00, 0.2, 0.2);
		sliderChangeSample1.setBoundsRelative(0.01, 0.15, 0.2, 0.2);
		sliderChangeSample2.setBoundsRelative(0.01, 0.30, 0.2, 0.2);

		addAndMakeVisible(sliderChangeSample);
		addAndMakeVisible(sliderChangeSample1);
		addAndMakeVisible(sliderChangeSample2);

	}

	if (waveformIsAvailable == true)
	{
		
	}
}

void HappySamplerAudioProcessorEditor::resized()
{
	loadButton.setBounds(getWidth() - 110, getHeight() / 2 - 300, 100, 50);
	buttonApply.setBounds(getWidth() - 440, getHeight() / 2 - 300, 100, 50);
	buttonApply1.setBounds(getWidth() - 330, getHeight() / 2 - 300, 100, 50);
	buttonApply2.setBounds(getWidth() - 220, getHeight() / 2 - 300, 100, 50);

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
		addAndMakeVisible(buttonApply);
		repaint();
	} 
	if (slider == &sliderChangeSample1)
	{
		audioProcessor.sampleStart1 = sliderChangeSample.getValue()
			* audioProcessor.sampleAmountOfLoadedSample;

		audioPosition1 = sliderChangeSample1.getValue();
		addAndMakeVisible(buttonApply1);
		repaint();
	}   
	if (slider == &sliderChangeSample2)
	{
		audioProcessor.sampleStart2 = sliderChangeSample.getValue()
			* audioProcessor.sampleAmountOfLoadedSample;

		audioPosition2 = sliderChangeSample2.getValue();
		addAndMakeVisible(buttonApply2);
		repaint();
	}   
	if (slider == &sliderGainControl)
	{
		audioProcessor.getGainControlParameters().gainValue1 = sliderGainControl.getValue();
	}
	if (slider == &sliderGainControl1)
	{
		audioProcessor.getGainControlParameters().gainValue2 = sliderGainControl1.getValue();
	}
	audioProcessor.updateGainControl();
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

void HappySamplerAudioProcessorEditor::paintPlayHead1(juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailBounds)
{
	g.setColour(juce::Colours::red);

	auto drawPosition = (audioPosition1) * (float)audioThumbnailBounds.getWidth()
		+ (float)audioThumbnailBounds.getX();
	g.drawLine(drawPosition, (float)audioThumbnailBounds.getY(), drawPosition,
		(float)audioThumbnailBounds.getBottom(), 4.0f);
}

void HappySamplerAudioProcessorEditor::paintPlayHead2(juce::Graphics& g, const juce::Rectangle<int>& audioThumbnailBounds)
{
	g.setColour(juce::Colours::blue);

	auto drawPosition = (audioPosition2) * (float)audioThumbnailBounds.getWidth()
		+ (float)audioThumbnailBounds.getX();
	g.drawLine(drawPosition, (float)audioThumbnailBounds.getY(), drawPosition,
		(float)audioThumbnailBounds.getBottom(), 4.0f);
}







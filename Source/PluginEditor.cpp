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
	: AudioProcessorEditor(&p), audioProcessor(p),
	audioThumbnailCache(5),
	audioThumbnail(512, *audioProcessor.getAudioFormatManager(), audioThumbnailCache)
{	// Ich glaube das Problem liegt darin, dass er den Audioformatmanager aufruft, bevor die Daten da sind
	loadButton.onClick = [&]() { 
		audioProcessor.loadFile();
		setPaintWaveFormToTrue();
	};

	addAndMakeVisible(loadButton);

	loadButton2.onClick = [&]() { audioProcessor.loadFile2();  };
	addAndMakeVisible(loadButton2);

	exportButton.onClick = [&]() { audioProcessor.exportFile(); };
	addAndMakeVisible(exportButton);

	buttonApply.onClick = [&]() {
		audioProcessor.setExportBuffer();
		audioProcessor.exportFile();
		};
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
	
		juce::Path path;

		path.clear();

		auto waveForm = audioProcessor.getWaveFormBuffer();

		auto ratioXAxis = waveForm.getNumSamples() / getWidth();

		auto buffer = waveForm.getReadPointer(0);

		//audio file scaled on x-axis 
		for (int sample = 0; sample < waveForm.getNumSamples(); sample += ratioXAxis)
		{
			audioPointsFromWaveForm.push_back(buffer[sample]);
		}

		//path.startNewSubPath(getWidth() / 2, getHeight() / 2);

		juce::File file = audioProcessor.getFile();

		audioThumbnail.setSource(new juce::FileInputSource(file));

		juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);

		if (audioThumbnail.getNumChannels() == 0)
			paintIfNoFileLoaded(g, thumbnailBounds);
		else
			paintIfFileLoaded(g, thumbnailBounds);

		//scale on yAxis  and connects the points with lines 
		for (int sample = 0; sample < audioPointsFromWaveForm.size(); ++sample)
		{
			auto point = juce::jmap<float>(audioPointsFromWaveForm[sample], -1.0f, 1.0f, 200, 0);
			path.lineTo(sample, point);
		}

		// draws the waveform
		g.strokePath(path, juce::PathStrokeType(2));

	}
}

void HappySamplerAudioProcessorEditor::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
	g.setColour(juce::Colours::white);
	g.fillRect(thumbnailBounds);

	g.setColour(juce::Colours::red);                               // [8]

	audioThumbnail.drawChannels(g,                                      // [9]
		thumbnailBounds,
		0.0,                                    // start time
		audioThumbnail.getTotalLength(),             // end time
		1.0f);                                  // vertical zoom
}

void HappySamplerAudioProcessorEditor::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
	g.setColour(juce::Colours::darkgrey);
	g.fillRect(thumbnailBounds);
	g.setColour(juce::Colours::white);
	g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void HappySamplerAudioProcessorEditor::resized()
{
	loadButton.setBounds(getWidth() - 110, getHeight() / 2 - 300, 100, 50);
	loadButton2.setBounds(getWidth() - 220, getHeight() / 2 - 300, 100, 50);
	exportButton.setBounds(getWidth() - 330, getHeight() / 2 - 300, 100, 50);
	buttonApply.setBounds(getWidth() - 440, getHeight() / 2 - 300, 100, 50);
	sliderChangeSample.setBoundsRelative(0.01, 0.30, 0.4, 0.2);
	sliderGainControl1.setBoundsRelative(0.01, 0.0, 0.4, 0.2);
	sliderGainControl2.setBoundsRelative(0.01, 0.15, 0.4, 0.2);
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

void HappySamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
	if (slider == &sliderChangeSample)
	{
		audioProcessor.sampleStart = sliderChangeSample.getValue()
			* audioProcessor.sampleAmountOfLoadedSample;
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









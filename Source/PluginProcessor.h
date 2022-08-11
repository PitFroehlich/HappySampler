/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MultiVoiceSynth.h"
#include "GainControl.h"




//==============================================================================
/**
*/
class HappySamplerAudioProcessor : public juce::AudioProcessor
{
public:
	//==============================================================================
	HappySamplerAudioProcessor();
	~HappySamplerAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
	//==============================================================================
	void loadFile();
	void loadFile2();
	void exportFile();
	int getCurrentSampleLength();
	void getSampleStartValue();
	void exportAndReloadEditedSample();

	void updateGainControl();

	void setExportBuffer();

	void fillWaveFormBuffer();
	//==============================================================================
	juce::AudioThumbnail getAudioThumbnail();
	//==============================================================================


	juce::File getFile();

	juce::AudioFormatManager* getAudioFormatManager();

	juce::AudioBuffer<float>& getWaveFormBuffer() { return waveFormBuffer; }

	//==============================================================================
	//Dieser Teil hier sollte die geladene Datei wiedergeben
	juce::AudioBuffer<float>& getLoadedSample(){ return loadedSample; };
	juce::AudioBuffer<float> exportbuffer;
	juce::AudioBuffer<float> waveFormBuffer;

	int sampleStart{ 0 };
	int sampleAmountOfLoadedSample = 1;

	int thisIsTheNumberofSample1;
	int thisIsTheNumberofSample2;

	GainControl::Parameters& getGainControlParameters() { return gainControlParams; } 

	//==============================================================================
	//Waveform Visualization 
	juce::AudioThumbnailCache audioThumbnailCache;
	juce::AudioThumbnail audioThumbnail;
	//==============================================================================
private: 

	

	juce::File loadedFile1;

	juce::WavAudioFormat wavAudioFormat;

	juce::AudioBuffer<float> loadedSample;
	juce::AudioBuffer<float> editedSample;
	

	MultiVoiceSynth synthesiser;
	const int synthesiserVoices{ 24 };

	const int numberOfSkippedSamples = 200000;

	GainControl::Parameters gainControlParams;


	juce::AudioFormatManager audioFormatManager;
	//This is a pointer so we do not have to create a new audioFormatReader each time we
	//change the File
	juce::AudioFormatReader* audioFormatReader{ nullptr };
	juce::AudioFormatReader* audioFormatReader2{ nullptr };
	juce::AudioFormatReader* audioFromatReaderFromReader{ nullptr };

	juce::AudioFormatWriter* audioFormatWriter{ nullptr };

	juce::MemoryOutputStream* memoryOutputStream{ nullptr };

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HappySamplerAudioProcessor)

};

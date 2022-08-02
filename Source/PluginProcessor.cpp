/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include <iostream>
#include <string>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MultiVoiceSynth.h"
#include "HSamplerSound.h"
#include "HSamplerSound2.h"
#include "HSamplerVoice.h"
#include "HSamplerVoice2.h"


//==============================================================================
HappySamplerAudioProcessor::HappySamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif

//This is the constructor 
{
	//This makes basic audio formats available
	audioFormatManager.registerBasicFormats();
	// Adds SamplerVoices to the first half
	for (int i = 0; i < synthesiserVoices / 2; i++)
	{
		synthesiser.addVoice(new HSamplerVoice);
	}

	// Adds SamplerVoices2 to the second half
	for (int i = synthesiserVoices / 2; i < synthesiserVoices; i++)
	{
		synthesiser.addVoice(new HSamplerVoice2);
	}
}
// This is the destructor 
HappySamplerAudioProcessor::~HappySamplerAudioProcessor()
{
	audioFormatReader = nullptr;
}

//==============================================================================
const juce::String HappySamplerAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool HappySamplerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool HappySamplerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool HappySamplerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double HappySamplerAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int HappySamplerAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int HappySamplerAudioProcessor::getCurrentProgram()
{
	return 0;
}

void HappySamplerAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String HappySamplerAudioProcessor::getProgramName(int index)
{
	return {};
}

void HappySamplerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void HappySamplerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	//gets the sample rate before anything starts
	
	synthesiser.setCurrentPlaybackSampleRate(sampleRate);

	

}

void HappySamplerAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HappySamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void HappySamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool HappySamplerAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HappySamplerAudioProcessor::createEditor()
{
	return new HappySamplerAudioProcessorEditor(*this);
}

//==============================================================================
void HappySamplerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void HappySamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

void HappySamplerAudioProcessor::exportFile()
{
	//exportbuffer.setSize(2, 10000);

	// do something with buffer

	juce::File file("C:/Users/pitfr/Desktop/test.wav");
	file.deleteFile();

	juce::WavAudioFormat format;
	std::unique_ptr<juce::AudioFormatWriter> writer;

	writer.reset(format.createWriterFor(new juce::FileOutputStream(file),
		44100,
		exportbuffer.getNumChannels(),
		16,
		{},
		0));

	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(exportbuffer, 0, exportbuffer.getNumSamples());

}

//loads a file and adds it as a sound to the sampler
void HappySamplerAudioProcessor::loadFile2()
{
	//creates a dialog box to choose a file 
	juce::FileChooser filechooser2{ "Please load a file" };

	

	if (filechooser2.browseForFileToOpen())
	{
		auto choosenFile2 = filechooser2.getResult();
		audioFormatReader2 = audioFormatManager.createReaderFor(choosenFile2);
		/*	loadedSample.setSize(1, numberOfLoadedSample);
			auto buffer = loadedSample.getReadPointer(0);*/
	}

	auto numberOfLoadedSample = static_cast<int>(audioFormatReader2->lengthInSamples);
	auto sampleRateFromSample = static_cast<int>(audioFormatReader2->sampleRate);
	auto bitsPerSampleFromSample = static_cast<int>(audioFormatReader2->bitsPerSample);

	juce::BigInteger samplerSoundRange;
	samplerSoundRange.setRange(0, 128, true);

	HSamplerSound2 *samplerSound2 = new HSamplerSound2(
		"Sample2",
		*audioFormatReader2,
		samplerSoundRange,
		60,
		0.1,
		0.1,
		5.0);

	synthesiser.addSound(samplerSound2);

	//Setting right size for exportBuffer
	exportbuffer.setSize(
		audioFormatReader2->numChannels,
		numberOfLoadedSample - sampleStart
	);

	//fill exportBuffer with audio 
	audioFormatReader2->read(
		&exportbuffer,
		0,
		numberOfLoadedSample - sampleStart,
		sampleStart,
		true,
		false
	);

}

void HappySamplerAudioProcessor::loadFile() 
{	//creates a dialog box to choose a file 
	juce::FileChooser filechooser{ "Please load a file" };

	if (filechooser.browseForFileToOpen())
	{
		auto choosenFile = filechooser.getResult();
		audioFormatReader = audioFormatManager.createReaderFor(choosenFile);
	/*	loadedSample.setSize(1, numberOfLoadedSample);
		auto buffer = loadedSample.getReadPointer(0);*/
	}

	sampleAmountOfLoadedSample = static_cast<int>(audioFormatReader->lengthInSamples);
	double sampleRateFromSample = static_cast<double>(audioFormatReader->sampleRate);
	int bitsPerSampleFromSample = static_cast<int>(audioFormatReader->bitsPerSample);

	juce::BigInteger samplerSoundRange;
	samplerSoundRange.setRange(0, 128, true);

	HSamplerSound* samplerSound = new HSamplerSound(
		"Sample",
		*audioFormatReader,
		samplerSoundRange,
		60,
		0.1,
		0.1,
		5.0);

	synthesiser.addSound(samplerSound);
} 

void HappySamplerAudioProcessor::exportAndReloadEditedSample() {

	//Setting right size for exportBuffer
	exportbuffer.setSize(
		audioFormatReader->numChannels,
		sampleAmountOfLoadedSample - sampleStart
	);

	//fill exportBuffer with audio 
	audioFormatReader->read(
		&exportbuffer,
		0,
		sampleAmountOfLoadedSample - sampleStart,
		sampleStart,
		true,
		false
	);

	exportFile();
	loadFile();

}

void HappySamplerAudioProcessor::debugger()
{
	DBG("This is the number of sounds");
	DBG(synthesiser.getNumSounds(););
}

void HappySamplerAudioProcessor::updateGainControl()
{
	
		// gets sounds an makes sure it is a SamplerSound (so setGainControlParameters works)
		// prevents program from crashing in case of exceptions
		if (auto voice = dynamic_cast<HSamplerVoice*>(synthesiser.getVoice(0)))
		{
			voice->setGainControlParameters(gainControlParams);
		}

		double debug = gainControlParams.gainValue2;
		

		if (auto voice2 = dynamic_cast<HSamplerVoice2*>(synthesiser.getVoice(20)))
		{
			voice2->setGainControlParameters(gainControlParams);
		}

	
}

int HappySamplerAudioProcessor::getCurrentSampleLength() {
	return sampleAmountOfLoadedSample;
}
//
//double HappySamplerAudioProcessor::getGainControl1() {
//	return gainControl1;
//}
//
//double HappySamplerAudioProcessor::getGainControl2() {
//	return gainControl2;
//}
//==============================================================================

//void HappySamplerAudioProcessor::getSampleStartValue() {
//}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new HappySamplerAudioProcessor();
}

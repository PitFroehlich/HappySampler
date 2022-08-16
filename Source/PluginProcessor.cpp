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
#include "HSamplerSound3.h"
#include "HSamplerVoice3.h"


//==============================================================================
HappySamplerAudioProcessor::HappySamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: audioThumbnailCache(5),
	audioThumbnail(100, audioFormatManager, audioThumbnailCache),
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif

	//This is the constructor ex
{

	////This makes basic audio formats available
	audioFormatManager.registerBasicFormats();


	for (int i = 0; i < synthesiserVoices / 3; i++)
	{
		synthesiser.addVoice(new HSamplerVoice);
	}

	// Adds SamplerVoices2 to the second half
	for (int i = synthesiserVoices / 3; i < ((synthesiserVoices / 3) * 2); i++)
	{
		synthesiser.addVoice(new HSamplerVoice2);
	}

	for (int i = ((synthesiserVoices / 3) * 2); i < synthesiserVoices; i++)
	{
		synthesiser.addVoice(new HSamplerVoice3);
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

void HappySamplerAudioProcessor::exportFile(std::string fileName)
{
	DBG("C:/Users/pitfr/Desktop/" + fileName + ".wav");

	juce::File file("C:/Users/pitfr/Desktop/" + fileName + ".wav");
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

void HappySamplerAudioProcessor::setExportBuffer(
	int sampleStartToUse) {
	//Setting right size for exportBuffer
	exportbuffer.setSize(
		originalFileAudioFormatReader->numChannels,
		originalFileSampleAmountOfLoadedSample - sampleStartToUse,
		false,
		false,
		false
	);

	//fill exportBuffer with audio 
	originalFileAudioFormatReader->read(
		&exportbuffer,
		0,
		originalFileSampleAmountOfLoadedSample - sampleStartToUse,
		sampleStartToUse,
		true,
		false
	);

}

void HappySamplerAudioProcessor::loadFile()
{
	//removes existing sounds or everything gets messy 
	synthesiser.clearSounds();
	
	//creates a dialog box to choose a file 
	juce::FileChooser filechooser{ "Please load a file" };

	if (filechooser.browseForFileToOpen())
	{
		auto choosenFile = filechooser.getResult();
		originalFileAudioFormatReader = audioFormatManager.createReaderFor(choosenFile);
		//audioFormatReader = audioFormatManager.createReaderFor(choosenFile);
		//audioFormatReader1 = audioFormatManager.createReaderFor(choosenFile);
		//audioFormatReader2 = audioFormatManager.createReaderFor(choosenFile);

		loadedFile1 = choosenFile;

		//sets loaded file as source for low resolution thumbnail
		audioThumbnail.setSource(new juce::FileInputSource(choosenFile));
	}

	originalFileSampleAmountOfLoadedSample = static_cast<int>(originalFileAudioFormatReader->lengthInSamples);
	double sampleRateFromSample = static_cast<double>(originalFileAudioFormatReader->sampleRate);
	int bitsPerSampleFromSample = static_cast<int>(originalFileAudioFormatReader->bitsPerSample);

	juce::BigInteger samplerSoundRange;
	samplerSoundRange.setRange(0, 128, true);


		HSamplerSound* samplerSound = new HSamplerSound(
			"Sample",
			*originalFileAudioFormatReader,
			samplerSoundRange,
			60,
			0.1,
			0.1,
			5.0);

		synthesiser.addSound(samplerSound);
		soundToRemove = synthesiser.getNumSounds() - 1;

}

void HappySamplerAudioProcessor::updateGainControl()
{
	for (int i = 0; i < synthesiser.getNumVoices(); i++)
	{
		// gets sounds an makes sure it is a SamplerSound (so setGainControlParameters works)
		// prevents program from crashing in case of exceptions
		if (auto voice = dynamic_cast<HSamplerVoice*>(synthesiser.getVoice(i)))
		{
			voice->setGainControlParameters(gainControlParams);
		}

		if (auto voice2 = dynamic_cast<HSamplerVoice2*>(synthesiser.getVoice(i)))
		{
			voice2->setGainControlParameters(gainControlParams);
		}
		if (auto voice3 = dynamic_cast<HSamplerVoice3*>(synthesiser.getVoice(i)))
		{
			voice3->setGainControlParameters(gainControlParams);
		}
	}

}
//==============================================================================
void HappySamplerAudioProcessor::reloadFile(
	int soundToRemove, 
	std::string fileToOpen, 
	std::string colourOfSample)
{
	synthesiser.removeSound(soundToRemove);

	auto choosenFile = juce::File() = "C:/Users/pitfr/Desktop/" + fileToOpen + ".wav";

		audioFormatReader = audioFormatManager.createReaderFor(choosenFile);

		loadedFile1 = choosenFile;

	sampleAmountOfLoadedSample = static_cast<int>(audioFormatReader->lengthInSamples);
	double sampleRateFromSample = static_cast<double>(audioFormatReader->sampleRate);
	int bitsPerSampleFromSample = static_cast<int>(audioFormatReader->bitsPerSample);

	juce::BigInteger samplerSoundRange;
	samplerSoundRange.setRange(0, 128, true);

	if (colourOfSample == "green")
	{
	HSamplerSound* samplerSound = new HSamplerSound(
		"Sample" + colourOfSample,
		*audioFormatReader,
		samplerSoundRange,
		60,
		0.1,
		0.1,
		5.0);

	synthesiser.addSound(samplerSound);
	soundToRemove = synthesiser.getNumSounds() - 1;

	}
	if (colourOfSample == "red")
	{ 
	HSamplerSound2* samplerSound2 = new HSamplerSound2(
		"Sample" + colourOfSample,
		*audioFormatReader,
		samplerSoundRange,
		60,
		0.1,
		0.1,
		5.0);

	synthesiser.addSound(samplerSound2);
	soundToRemove2 = synthesiser.getNumSounds() - 1;
	}
	if (colourOfSample == "blue")
	{ 
	HSamplerSound3* samplerSound3 = new HSamplerSound3(
		"Sample" + colourOfSample,
		*audioFormatReader,
		samplerSoundRange,
		60,
		0.1,
		0.1,
		5.0);
	synthesiser.addSound(samplerSound3);
	soundToRemove3 = synthesiser.getNumSounds() - 1;
	}



	DBG("Tis is the number of sounds at the end of reload");
	DBG(synthesiser.getNumSounds());
}

int HappySamplerAudioProcessor::getCurrentSampleLength() {
	return sampleAmountOfLoadedSample;
}
//==============================================================================
juce::AudioFormatManager* HappySamplerAudioProcessor::getAudioFormatManager() {
	return &audioFormatManager;
}
//==============================================================================
juce::File HappySamplerAudioProcessor::getFile() {
	return loadedFile1;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new HappySamplerAudioProcessor();
}


/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
	// Adds the maximal number of voices to our synthersiser
	for (int i = 0; i < synthesiserVoices; i++)
	{
		synthesiser.addVoice(new juce::SamplerVoice);
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
	synthesiser.renderNextBlock(loadedSample, midiMessages, 0, loadedSample.getNumSamples());
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

//loads a file and adds it as a sound to the sampler
void HappySamplerAudioProcessor::loadFile() 
{	//creates a dialog box to choose a file 
	juce::FileChooser filechooser{ "Please load a file" };

	if (filechooser.browseForFileToOpen())
	{
		auto choosenFile = filechooser.getResult();
		audioFormatReader = audioFormatManager.createReaderFor(choosenFile);

		auto sampleLengthOfLoadedSample = static_cast<int>(audioFormatReader->lengthInSamples);

		loadedSample.setSize(1, sampleLengthOfLoadedSample);
		audioFormatReader->read(&loadedSample, numberOfSkippedSamples, (sampleLengthOfLoadedSample - numberOfSkippedSamples), numberOfSkippedSamples, true, false);
		auto buffer = loadedSample.getReadPointer(0);
	}

	juce::BigInteger samplerSoundRange;
	samplerSoundRange.setRange(0, 128, true);
	//Adds a new sound to our HappySampler
	synthesiser.addSound(new juce::SamplerSound("Sample", *audioFormatReader, samplerSoundRange, 60, 0.1, 0.1, 5.0));
} 

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new HappySamplerAudioProcessor();
}

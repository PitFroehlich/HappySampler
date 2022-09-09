/*
  ==============================================================================

	HSamplerVoice.cpp
	Created: 26 Jul 2022 1:38:31pm
	Author:  pitfr

  ==============================================================================
*/

#include "HSamplerVoice.h"
#include "HSamplerSound.h"
#include "MultiVoiceSynth.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "GainControl.h"

bool HSamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<const HSamplerSound*> (sound) != nullptr;
}

void HSamplerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int pitchWheelPosition)
{
	if (auto* sound = dynamic_cast<const HSamplerSound*> (s))
	{
		
		pitchRatio = std::pow(2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
			* sound->sourceSampleRate / getSampleRate();

		sourceSamplePosition = 0.0;
		lgain = velocity;
		rgain = velocity;

		adsr.setSampleRate(sound->sourceSampleRate);
		adsr.setParameters(sound->params);

		adsr.noteOn();
	}
	else
	{
		jassertfalse; // this object can only play HSamplerSounds!
	}
}

void HSamplerVoice::stopNote(float /*velocity*/, bool allowTailOff)
{

	if (allowTailOff)
	{
		adsr.noteOff();
	}
	else
	{
		clearCurrentNote();
		adsr.reset();
	}
}

//void HSamplerVoice::pitchWheelMoved(int /*newValue*/) {}

//void HSamplerVoice::controllerMoved(int /*controllerNumber*/, int /*newValue*/) {}

void HSamplerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{   
	if (auto* playingSound = dynamic_cast<HSamplerSound*> (getCurrentlyPlayingSound().get()))
	{	
		
		auto& data = *playingSound->getAudioData();
		const float* const inL = data.getReadPointer(0);
		const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;

		float* outL = outputBuffer.getWritePointer(0, startSample);
		float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

		int initalNumSamples = numSamples;

		while (--numSamples >= 0)
		{	
		
			

			auto pos = (int)sourceSamplePosition;
			auto alpha = (float)(sourceSamplePosition - pos);
			auto invAlpha = 1.0f - alpha;

			// just using a very simple linear interpolation here..
			float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
			float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
				: l;

			auto envelopeValue = adsr.getNextSample();


			auto gain = gainParams.gainValue1;

			l *= lgain * envelopeValue * gain;
			r *= rgain * envelopeValue * gain;

			if (outR != nullptr)
			{
				*outL++ += l;
				*outR++ += r;
			}
			else
			{
				*outL++ += (l + r) * 0.5f;
			}

			sourceSamplePosition += pitchRatio;

			if (adsr.getNextSample() == 0 || sourceSamplePosition > playingSound->length)
			{
				clearCurrentNote();
				stopNote(0.0f, false);
				break;
			}
		} 
	}
}


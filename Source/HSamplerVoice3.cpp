/*
  ==============================================================================

    HSamplerVoice3.cpp
    Created: 16 Sep 2022 3:08:04pm
    Author:  pitfr

  ==============================================================================
*/

#include "HSamplerVoice3.h"
#include "HSamplerSound3.h"
#include "MultiVoiceSynth.h"
#include "GainControl.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

bool HSamplerVoice3::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<const HSamplerSound3*> (sound) != nullptr;
}

void HSamplerVoice3::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{
	if (auto* sound = dynamic_cast<const HSamplerSound3*> (s))
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

void HSamplerVoice3::stopNote(float /*velocity*/, bool allowTailOff)
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

void HSamplerVoice3::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (auto* playingSound = dynamic_cast<HSamplerSound3*> (getCurrentlyPlayingSound().get()))
	{
		auto& data = *playingSound->getAudioData();
		const float* const inL = data.getReadPointer(0);
		const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;

		float* outL = outputBuffer.getWritePointer(0, startSample);
		float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

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

			auto gain = gainParams.gainValue3;

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
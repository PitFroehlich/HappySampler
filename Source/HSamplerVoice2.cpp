/*
  ==============================================================================

    HSamplerVoice2.cpp
    Created: 1 Aug 2022 10:50:05am
    Author:  pitfr

  ==============================================================================
*/

#include "HSamplerVoice2.h"
#include "HSamplerSound2.h"
#include "MultiVoiceSynth.h"

bool HSamplerVoice2::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<const HSamplerSound2*> (sound) != nullptr;
}

void HSamplerVoice2::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{
    if (auto* sound = dynamic_cast<const HSamplerSound2*> (s))
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

void HSamplerVoice2::stopNote(float /*velocity*/, bool allowTailOff)
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

void HSamplerVoice2::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (auto* playingSound = dynamic_cast<HSamplerSound2*> (getCurrentlyPlayingSound().get()))
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

            l *= lgain * envelopeValue * 0.001;
            r *= rgain * envelopeValue * 0.001;

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


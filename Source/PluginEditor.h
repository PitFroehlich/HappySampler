/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HappySamplerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HappySamplerAudioProcessorEditor (HappySamplerAudioProcessor&);
    ~HappySamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton loadButton{ "Load" };

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HappySamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HappySamplerAudioProcessorEditor)
};

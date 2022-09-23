///*
//  ==============================================================================
//
//    HSLoopTimer.h
//    Created: 30 Aug 2022 7:15:17pm
//    Author:  pitfr
//
//  ==============================================================================
//*/
//
//#pragma once
//#include<JuceHeader.h>
//#include"HSamplerVoice.h"
//#include"HSamplerSound.h"
//
//class HSLoopTimer : public juce::Timer
//{
//public:
//
//    int LoopMidiNoteNumber;
//    float loopVelocity; 
//    juce::SynthesiserSound* loopS; 
//    int loopPitchWheel;
//    
//    HSLoopTimer(int loopLength, int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int pitchWheel)
//  {
//        LoopMidiNoteNumber = midiNoteNumber;
//        loopVelocity = velocity;
//        loopS = s;
//        loopPitchWheel = pitchWheel;
//        startTimer(loopLength);
//
//  };
//
//  void timerCallback() override
//  {
//    HSamplerVoice::startNote
//  };
//
//  void paint(Graphics& g) override
//  {
//      // paint here
//      };
//
//};
/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class SimpleDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SimpleDelayAudioProcessor();
    ~SimpleDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float getDelayFromBPM(int index);
    void fillDelayBuffer (int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData);
    void getFromDelayBuffer (AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData);
    void feedbackDelay (int channel, const int bufferLength, const int delayBufferLength, float* dryBuffer);
    
    //Init Value Trees
    AudioProcessorValueTreeState tree;
    
private:
    //Init Delay Buffer, Write Position, Sample Rate
    AudioBuffer<float> mDelayBuffer;
    int mWritePosition {0};
    int mSampleRate {44100};
    float lastDelayTime;
    
    //AudioProcessorValueTreeState::Parameter param; 
    
    //Init Reverb, Parameters
    Reverb cavern;
    Reverb::Parameters cavernParameters;
    
    //Delay by BPM Menu
    AudioPlayHead::CurrentPositionInfo playHead;
    
    bool delayByBPM = false;
    
    //Value Smoothing
    SmoothedValue<float, ValueSmoothingTypes::Linear> delayMS {0.00001f};
    SmoothedValue<float, ValueSmoothingTypes::Linear> delayGain {0.1f};
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDelayAudioProcessor)
};

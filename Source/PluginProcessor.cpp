/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelayAudioProcessor::SimpleDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
<<<<<<< HEAD
<<<<<<< HEAD
                       ), tree(*this, nullptr)    //Add Tree
#endif
{
    //initialize delay parameters
//    auto BPM = playHead.bpm;
    auto BPM = 120;
    float maxDelay = 60000 / BPM * 4;
    
    using delayValue = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<delayValue> ("delayValue", "DelayValue", String(), NormalisableRange<float>(0.0f, maxDelay), 20.0f, nullptr, nullptr));
    using feedbackValue = AudioProcessorValueTreeState::Parameter;
    
    using delayMS = SmoothedValue<float, ValueSmoothingTypes::Linear>;
    

    
    tree.createAndAddParameter(std::make_unique<feedbackValue> ("feedbackValue", "FeedbackValue", String(), NormalisableRange<float>(0.1f, 0.8f), 0.1f, nullptr, nullptr));
    
    //initialize reverb parameters
    using dryWet = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<dryWet> ("dryWet", "DryWet", String(), NormalisableRange<float> (0.0f, 1.0f), 0.5f, nullptr, nullptr));
    using damping = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<damping> ("damping", "Damping", String(), NormalisableRange<float> (0.0f, 1.0f), 0.1f, nullptr, nullptr));
    using roomSize = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<roomSize> ("roomSize", "RoomSize", String(), NormalisableRange<float> (0.0f, 1.0f), 0.4f, nullptr, nullptr));
    using roomWidth = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<roomWidth> ("roomWidth", "RoomWidth", String(), NormalisableRange<float> (0.0f, 1.0f), 0.4f, nullptr, nullptr));
    
    using delayChoice = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<delayChoice> ("delayChoice", "DelayChoice", String(), NormalisableRange<float> (0.0f, 12.0f), 0.0f, nullptr, nullptr));
    
    using bpmToggle = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<bpmToggle> ("bpmToggle", "BpmToggle", String(), NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr));
=======
                       ), tree(*this, nullptr)  //Add Trees
#endif
{
    //initialize delay parameters
=======
                       ), tree(*this, nullptr)  //Add Trees
#endif
{
    //initialize delay parameters
>>>>>>> origin/master
    using delayValue = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<delayValue> ("delayValue", "DelayValue", String(), NormalisableRange<float>(20.0f, 1000.0f), 500.0f, nullptr, nullptr));
    using feedbackValue = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<feedbackValue> ("feedbackValue", "FeedbackValue", String(), NormalisableRange<float>(0.1f, 0.8f), 0.1f, nullptr, nullptr));
    
    //initialize reverb parameters
    using dryWet = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<dryWet> ("dryWet", "DryWet", String(), NormalisableRange<float> (0.0f, 1.0f), 0.5f, nullptr, nullptr));
    using damping = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<damping> ("damping", "Damping", String(), NormalisableRange<float> (0.0f, 1.0f), 0.1f, nullptr, nullptr));
    using roomSize = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<roomSize> ("roomSize", "RoomSize", String(), NormalisableRange<float> (0.0f, 1.0f), 0.4f, nullptr, nullptr));
    using roomWidth = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<roomWidth> ("roomWidth", "RoomWidth", String(), NormalisableRange<float> (0.0f, 1.0f), 0.4f, nullptr, nullptr));
<<<<<<< HEAD
>>>>>>> origin/master
=======
>>>>>>> origin/master
    
    //Set Plugin State
    tree.state = ValueTree(Identifier("DelayState"));
    //params.state = ValueTree(Identifier("ReverbState"));
}

SimpleDelayAudioProcessor::~SimpleDelayAudioProcessor()
{
}

//==============================================================================
const String SimpleDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String SimpleDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SimpleDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //Find number of channels
    const int numInputChannels = getTotalNumInputChannels();
    
    //Find Delay Buffer Size
    const int delayBufferSize = 2 * (sampleRate + samplesPerBlock);
    mSampleRate = sampleRate;
    
    //Update Delay Buffer
<<<<<<< HEAD
<<<<<<< HEAD
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    
    //Set sample rate for reverb
    cavern.setSampleRate(sampleRate);
    lastDelayTime = 0.01;
    
=======
=======
>>>>>>> origin/master
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    
    //Set sample rate for reverb
    cavern.setSampleRate(sampleRate);
<<<<<<< HEAD
>>>>>>> origin/master
=======
>>>>>>> origin/master
}

void SimpleDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void SimpleDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
<<<<<<< HEAD
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    delayMS.setCurrentAndTargetValue(0.0);
=======
    auto totalNumOutputChannels = getTotalNumOutputChannels();
<<<<<<< HEAD
>>>>>>> origin/master
=======
>>>>>>> origin/master

    //Get current values for reverb parameters
    const float lastDryWet = *tree.getRawParameterValue("dryWet");
    const float lastRoomSize = *tree.getRawParameterValue("roomSize");
    const float lastDamping = *tree.getRawParameterValue("damping");
    const float lastWidth = *tree.getRawParameterValue("roomWidth");
    
    //Eliminate Feedback Loop
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //Find number of channels
    const auto numChannels = jmin(totalNumInputChannels, totalNumOutputChannels);
    
    //Update Parameters, Merge Dry/Wet Levels
    cavernParameters.dryLevel = 1 - lastDryWet;
    cavernParameters.wetLevel = lastDryWet;
    cavernParameters.roomSize = lastRoomSize;
    cavernParameters.damping = lastDamping;
    cavernParameters.width = lastWidth;
    
    //Set Reverb Parameters
    cavern.setParameters(cavernParameters);
    
    //Process Mono/Stereo
    if (numChannels == 1)
        cavern.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    else if (numChannels == 2)
        cavern.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    
    //Get Buffer Lengths
    const int bufferLength = buffer.getNumSamples();
<<<<<<< HEAD
<<<<<<< HEAD
    const int delayBufferLength = mDelayBuffer.getNumSamples();
    
//    std::cout << "Buffer Length is: " << bufferLength << std::endl;
//    std::cout << "Delay Buffer Length is: " << delayBufferLength << std::endl;
=======
    const int delayBufferLength = mDelayBuffer.getNumSamples();
>>>>>>> origin/master
=======
    const int delayBufferLength = mDelayBuffer.getNumSamples();
>>>>>>> origin/master
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //Read Pointers
        const float* bufferData = buffer.getReadPointer(channel);
        const float* delayBufferData = mDelayBuffer.getReadPointer(channel);
        float* dryBuffer = buffer.getWritePointer(channel);
        
        //Call Functions
        fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
        getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
        feedbackDelay(channel, bufferLength, delayBufferLength, dryBuffer);
    }
    
    //Update write position
    mWritePosition += bufferLength;
    mWritePosition %= delayBufferLength;
}

void SimpleDelayAudioProcessor::fillDelayBuffer (int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    const float gain = 0.3;
    
    //Write Data From Main Buffer to Delay Buffer
    if (delayBufferLength > bufferLength + mWritePosition)
    {
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
    }
    else {
        const int bufferRemaining = delayBufferLength - mWritePosition;
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
        mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, gain, gain);
    }
    
}
<<<<<<< HEAD
<<<<<<< HEAD


float SimpleDelayAudioProcessor::getDelayFromBPM(int index){
    float delayinMS = 0.0;
//    auto BPM = playHead.bpm;
    int BPM=120;
    
    switch (index) {
        case 0: // 1/64T
            break;
        case 1: // 1/64
            delayinMS = 60000 / BPM / 16;
            break;
        case 2: // 1/32T
            break;
        case 3: // 1/32
            delayinMS = 60000 / BPM / 8;
            break;
        case 4: // 1/16T
            break;
        case 5: // 1/16
            delayinMS = 60000 / BPM / 4;
            break;
        case 6: // 1/8T
            break;
        case 7: // 1/8
            delayinMS = 60000 / BPM / 2;
            break;
        case 8: // 1/4T
            break;
        case 9: // 1/4
            delayinMS = 60000 / BPM;
            break;
        case 10:// 1/2T
            break;
        case 11:// 1/2
            delayinMS = 60000 / BPM * 2;
            break;
        case 12:// 1 Bar
            delayinMS = 60000 / BPM * 4;
            break;
        default:
            break;
    }
    
    return delayinMS;
}


void SimpleDelayAudioProcessor::getFromDelayBuffer (AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    //Get delay time from ValueTree
    float currentDelayTime;
    float toggle = *tree.getRawParameterValue("bpmToggle");
    
    if (toggle==1){
        int subdivision = *tree.getRawParameterValue("delayChoice");
        currentDelayTime = getDelayFromBPM(subdivision);
        Value delayMSKnob = tree.getParameterAsValue("delayValue");
        delayMSKnob.setValue(currentDelayTime);
    }
    else{
        currentDelayTime = *tree.getRawParameterValue("delayValue");
    }
    std::cout << currentDelayTime << std::endl;
    
    if (lastDelayTime != currentDelayTime){ //if you turn the knob
        delayMS.setTargetValue(currentDelayTime);
        currentDelayTime = delayMS.getNextValue();
        lastDelayTime = currentDelayTime;
        delayMS.reset(0.01);
    }
    
    //Initialize Read Position
    const int readPosition = static_cast<int> (delayBufferLength + mWritePosition - (mSampleRate * currentDelayTime / 1000)) % delayBufferLength;
=======
=======
>>>>>>> origin/master

void SimpleDelayAudioProcessor::getFromDelayBuffer (AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    //Get delay time from ValueTree
    int delayTime = *tree.getRawParameterValue("delayValue");
    
    //Initialize Read Position
    const int readPosition = static_cast<int> (delayBufferLength + mWritePosition - (mSampleRate * delayTime / 1000)) % delayBufferLength;
<<<<<<< HEAD
>>>>>>> origin/master
=======
>>>>>>> origin/master
    
    //Read data from Main Buffer to Delay Buffer
    if (delayBufferLength > bufferLength + readPosition)
    {
        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferLength);
    }
    
    else
    {
        const int bufferRemaining = delayBufferLength - readPosition;
        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.copyFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
    }
}

void SimpleDelayAudioProcessor::feedbackDelay (int channel, const int bufferLength, const int delayBufferLength, float* dryBuffer)
{
    //Get feedback value from ValueTree
    const float feedbackGain = *tree.getRawParameterValue("feedbackValue");
<<<<<<< HEAD
<<<<<<< HEAD
    delayGain.setCurrentAndTargetValue(feedbackGain);
    //Copy Delayed Signal to Main Buffer
    if (delayBufferLength > bufferLength + mWritePosition)
    {
        mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLength, delayGain.getNextValue(), delayGain.getNextValue());
=======
=======
>>>>>>> origin/master
    
    //Copy Delayed Signal to Main Buffer
    if (delayBufferLength > bufferLength + mWritePosition)
    {
        mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLength, feedbackGain, feedbackGain);
<<<<<<< HEAD
>>>>>>> origin/master
=======
>>>>>>> origin/master
    }
    
    else
    {
        const int bufferRemaining = delayBufferLength - mWritePosition;
        
<<<<<<< HEAD
<<<<<<< HEAD
        mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, delayGain.getNextValue(), delayGain.getNextValue());
        mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, delayGain.getNextValue(), delayGain.getNextValue());
=======
        mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, feedbackGain, feedbackGain);
        mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, feedbackGain, feedbackGain);
>>>>>>> origin/master
=======
        mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, feedbackGain, feedbackGain);
        mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, feedbackGain, feedbackGain);
>>>>>>> origin/master
    }
}

//==============================================================================
bool SimpleDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimpleDelayAudioProcessor::createEditor()
{
    return new SimpleDelayAudioProcessorEditor (*this, tree);
}

//==============================================================================
void SimpleDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleDelayAudioProcessor();
}

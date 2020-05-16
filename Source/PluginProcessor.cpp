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
                       ), tree(*this, nullptr)    //Add Tree
#endif
{
//    auto BPM = playHead.bpm;
    auto BPM = 120;
    float maxDelay = 60000 / BPM * 4; // maxDelay is the delay amount of 1 bar
    
    //initialize reverb parameters
    using dryWet = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<dryWet> ("dryWet", "DryWet", String(), NormalisableRange<float> (0.0f, 1.0f), 0.5f, nullptr, nullptr));
    using damping = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<damping> ("damping", "Damping", String(), NormalisableRange<float> (0.0f, 1.0f), 0.1f, nullptr, nullptr));
    using roomSize = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<roomSize> ("roomSize", "RoomSize", String(), NormalisableRange<float> (0.0f, 1.0f), 0.4f, nullptr, nullptr));
    using roomWidth = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<roomWidth> ("roomWidth", "RoomWidth", String(), NormalisableRange<float> (0.0f, 1.0f), 0.4f, nullptr, nullptr));
    
    // initialize delay parameters
    using delayChoice = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<delayChoice> ("delayChoice", "DelayChoice", String(), NormalisableRange<float> (0.0f, 12.0f), 0.0f, nullptr, nullptr));
    using bpmToggle = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<bpmToggle> ("bpmToggle", "BpmToggle", String(), NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr));
    using delayValue = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<delayValue> ("delayValue", "DelayValue", String(), NormalisableRange<float>(0.0f, maxDelay), 20.0f, nullptr, nullptr));
    using feedbackValue = AudioProcessorValueTreeState::Parameter;
    tree.createAndAddParameter(std::make_unique<feedbackValue> ("feedbackValue", "FeedbackValue", String(), NormalisableRange<float>(0.1f, 0.8f), 0.1f, nullptr, nullptr));
    
    // Set inital delay time
    currentDelayTime = *tree.getRawParameterValue("delayValue");
    
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
    //Find number of input channels
    const int numInputChannels = getTotalNumInputChannels();
    
    //Find Delay Buffer Size, with 2 seconds of audio to access
    const int delayBufferSize = 2 * (sampleRate + samplesPerBlock);
    mSampleRate = sampleRate;
    
    //Update Delay Buffer
    mDelayBuffer.setSize(numInputChannels, delayBufferSize);
    mSampleRate = sampleRate;
    
    // Set initial values for smoothing parameters
    delayMS.setCurrentAndTargetValue(currentDelayTime);
    delayGain.setCurrentAndTargetValue(currentFeedbackGain);
    
    //Set sample rate for reverb
    cavern.setSampleRate(sampleRate);
}

void SimpleDelayAudioProcessor::reset(double sampleRate)
{
    delayMS.reset(sampleRate, 0.0001);
    delayGain.reset(sampleRate, 0.0001);
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
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //Get current values for reverb parameters
    const float lastDryWet = *tree.getRawParameterValue("dryWet");
    const float lastRoomSize = *tree.getRawParameterValue("roomSize");
    const float lastDamping = *tree.getRawParameterValue("damping");
    const float lastWidth = *tree.getRawParameterValue("roomWidth");
    
    //Eliminate Feedback Loop
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //Find min number of channels
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
    const int delayBufferLength = mDelayBuffer.getNumSamples();
    
    // Iterate DSP over number of input channels
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
    mWritePosition += bufferLength; // Move write position by length of buffer size
    mWritePosition %= delayBufferLength; // Wrap around when end of dry buffer is reached
}

void SimpleDelayAudioProcessor::fillDelayBuffer (int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    const float gain = 0.0;
    
    // Write Data From Main Buffer to Delay Buffer if the delay buffer length is greater than the dry buffer length plus the write position (until delay buffer samples are exhausted).
    if (delayBufferLength > bufferLength + mWritePosition)
    {
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
    }
    else
    {
        // wrap remaining buffer samples around into next iteration
        const int bufferRemaining = delayBufferLength - mWritePosition;
        mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
        mDelayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, gain, gain);
    }
    
}


float SimpleDelayAudioProcessor::getDelayFromBPM(int index){
    float delayinMS = 0.0;
    int base, factor;
    auto BPM = playHead.bpm;
//    int BPM=120;
    
    // Calculate Delay from BPM, based upon 60,000 ms / BPM, which gives us the delay amount in 1 beat after receiving the subdivision from our user-set menu index.
    switch (index) {
        case 0: // 1/64T
            break;
        case 1: // 1/64
            delayinMS = 60000 / BPM / 16;
            break;
        case 2: // 1/32T
            delayinMS = 60000 / BPM / 12;
            break;
        case 3: // 1/32
            delayinMS = 60000 / BPM / 8;
            break;
        case 4: // 1/16T
            delayinMS = 60000 / BPM / 6;
            break;
        case 5: // 1/16
            delayinMS = 60000 / BPM / 4;
            break;
        case 6: // 1/8T
            delayinMS = 60000 / BPM / 3;
            break;
        case 7: // 1/8
            delayinMS = 60000 / BPM / 2;
            break;
        case 8: // 1/4T
            delayinMS = 60000 / BPM / 1.5;
            break;
        case 9: // 1/4
            delayinMS = 60000 / BPM;
            break;
        case 10:// 1/2T
            base = 60000 / BPM;
            factor = base / 3;
            delayinMS = factor + base;
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
    float toggle = *tree.getRawParameterValue("bpmToggle");
    
    if (toggle==1){
        // Retrieve delay value from subdivision, relay to delay Knob for visual response
        int subdivision = *tree.getRawParameterValue("delayChoice");
        currentDelayTime = getDelayFromBPM(subdivision);
        Value delayMSKnob = tree.getParameterAsValue("delayValue");
        delayMSKnob.setValue(currentDelayTime);
    }
    
    if (lastDelayTime != currentDelayTime){
        // Apply Value Smoothing when knob is turned
        delayMS.setTargetValue(currentDelayTime);
        currentDelayTime = delayMS.getNextValue();
        lastDelayTime = currentDelayTime;
    }
    
    // Calculate Read Position (sample) using delay time in ms
    readPosition = static_cast<int> (delayBufferLength + mWritePosition - (mSampleRate * currentDelayTime / 1000)) % delayBufferLength;
    
    // Read data from Main Buffer to Delay Buffer if the delay buffer length is greater than the dry buffer length plus the read position (until delay buffer samples are exhausted).
    if (delayBufferLength > bufferLength + readPosition)
    {
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferLength);
    }
    
    else
    {
        // wrap remaining buffer samples around into next iteration
        const int bufferRemaining = delayBufferLength - readPosition;
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
    }
    
}

void SimpleDelayAudioProcessor::feedbackDelay (int channel, const int bufferLength, const int delayBufferLength, float* dryBuffer)
{
    //Get feedback value from ValueTree
    currentFeedbackGain = *tree.getRawParameterValue("feedbackValue");
    
    if (lastFeedbackGain != currentFeedbackGain){
        // Apply Value Smoothing when knob is turned
        delayGain.setTargetValue(currentFeedbackGain);
        currentFeedbackGain = delayGain.getNextValue();
        lastFeedbackGain = currentFeedbackGain;
    }
    
    // Copy Delayed Signal to Main Buffer (using addFrom as we want both the dry and wet signals)  if the delay buffer length is greater than the dry buffer length plus the write position (until delay buffer samples are exhausted).
    if (delayBufferLength > bufferLength + mWritePosition)
    {
        mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLength, currentFeedbackGain, currentFeedbackGain);
    }

    else
    {
        // wrap remaining buffer samples around into next iteration
        const int bufferRemaining = delayBufferLength - mWritePosition;
        
        mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, currentFeedbackGain, currentFeedbackGain);
        mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, currentFeedbackGain, currentFeedbackGain);
    }
}

void SimpleDelayAudioProcessor::updateDelayTime()
{
    currentDelayTime = *tree.getRawParameterValue("delayValue");
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

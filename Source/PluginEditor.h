/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Images.h"

//==============================================================================
/**
*/
class SimpleDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SimpleDelayAudioProcessorEditor (SimpleDelayAudioProcessor&, AudioProcessorValueTreeState&);
    ~SimpleDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    AudioProcessorValueTreeState& valueStateTree;
    
    // Create Silders, Labels and Attachments
    Slider numDelayDial;
    Label numDelayLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> numDelayValue;
    
    Slider feedbackDial;
    Label feedbackLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> feedbackValue;
    
    Slider roomSizeSlider;
    Label roomSizeLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment;
    
    Slider roomWidthSlider;
    Label roomWidthLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> roomWidthAttachment;
    
    Slider dampingSlider;
    Label dampingLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    
    Slider dryWetSlider;
    Label dryWetLabel;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;
    
    Image backgroundImage;
    
    ToggleButton BPM_Toggle;
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> bpmButtonAttachment;
    
    ComboBox delayMenu;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachment;
    StringArray menuSelections {"1/64T", "1/64", "1/32T", "1/32", "1/16T", "1/16", "1/8T" ,"1/8" ,"1/4T", "1/4", "1/2T", "1/2", "1 Bar"};
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleDelayAudioProcessor& processor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleDelayAudioProcessorEditor)
};

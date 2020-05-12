/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelayAudioProcessorEditor::SimpleDelayAudioProcessorEditor (SimpleDelayAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueStateTree (vts), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
    //Delay Dial
    AudioPlayHead::CurrentPositionInfo playHead;
    
    auto BPM = playHead.bpm;
//    auto BPM = 120;
    auto maxDelay = 60000 / BPM * 4;

    //Delay Dial
    numDelayDial.setSliderStyle(Slider::RotaryVerticalDrag);
    numDelayDial.setRange(20, maxDelay, 1);
    numDelayDial.setValue(20.0);
    numDelayDial.setNumDecimalPlacesToDisplay(1);
    numDelayDial.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    numDelayDial.setTextValueSuffix(" ms");
    addAndMakeVisible(numDelayDial);
    numDelayValue = new AudioProcessorValueTreeState::SliderAttachment(valueStateTree, "delayValue", numDelayDial);
    
    //Delay Label
    addAndMakeVisible(numDelayLabel);
    numDelayLabel.setText("Delay Time", dontSendNotification);
    numDelayLabel.setJustificationType(Justification::centred);
    
    //Feedback Dial
    feedbackDial.setSliderStyle(Slider::RotaryVerticalDrag);
    feedbackDial.setRange(0.1, 0.8, 0.1);
    feedbackDial.setValue(0.6);
    feedbackDial.setNumDecimalPlacesToDisplay(1);
    feedbackDial.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    addAndMakeVisible(feedbackDial);
    feedbackValue = new AudioProcessorValueTreeState::SliderAttachment(valueStateTree, "feedbackValue", feedbackDial);
    
    //Feedback Label
    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback Gain", dontSendNotification);
    feedbackLabel.setJustificationType(Justification::centred);
    
    //Dry Wet Slider
    addAndMakeVisible(dryWetSlider);
    dryWetSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    dryWetSlider.setRange(0, 1, 0.01);
    dryWetAttachment = new AudioProcessorValueTreeState::SliderAttachment(valueStateTree, "dryWet", dryWetSlider);
    
    //Dry Wet Label
    addAndMakeVisible(dryWetLabel);
    dryWetLabel.setText("Dry/Wet", dontSendNotification);
    dryWetLabel.setJustificationType(Justification::centred);
    
    //Damping Slider
    addAndMakeVisible(dampingSlider);
    dampingSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    dampingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    dampingSlider.setRange(0, 1, 0.01);
    dampingAttachment = new AudioProcessorValueTreeState::SliderAttachment(valueStateTree, "damping", dampingSlider);
    
    //Damping Label
    addAndMakeVisible(dampingLabel);
    dampingLabel.setText("Damping", dontSendNotification);
    dampingLabel.setJustificationType(Justification::centred);
    
    //Room Size Slider
    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    roomSizeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    roomSizeSlider.setRange(0, 1, 0.01);
    roomSizeAttachment = new AudioProcessorValueTreeState::SliderAttachment(valueStateTree, "roomSize", roomSizeSlider);
    
    //Room Size Label
    addAndMakeVisible(roomSizeLabel);
    roomSizeLabel.setText("Room Size", dontSendNotification);
    roomSizeLabel.setJustificationType(Justification::centred);
    
    //Room Width Slider
    addAndMakeVisible(roomWidthSlider);
    roomWidthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    roomWidthSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    roomWidthSlider.setRange(0, 1, 0.01);
    roomWidthAttachment = new AudioProcessorValueTreeState::SliderAttachment(valueStateTree, "roomWidth", roomWidthSlider);
    addAndMakeVisible(feedbackDial);
    
    //Room Width Label
    addAndMakeVisible(roomWidthLabel);
    roomWidthLabel.setText("Room Width", dontSendNotification);
    roomWidthLabel.setJustificationType(Justification::centred);
    
    // BPM Toggle
    addAndMakeVisible(BPM_Toggle);
    BPM_Toggle.setButtonText("Tempo Sync");
    bpmButtonAttachment = new AudioProcessorValueTreeState::ButtonAttachment (valueStateTree, "bpmToggle", BPM_Toggle);
    
    // Delay Combobox
    addAndMakeVisible(delayMenu);
    delayMenu.addItemList(menuSelections, 1);
    delayMenu.setJustificationType(Justification::centred);

    comboBoxAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment (valueStateTree, "delayChoice", delayMenu);

}

SimpleDelayAudioProcessorEditor::~SimpleDelayAudioProcessorEditor()
{
}

//==============================================================================
void SimpleDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Variable Delay Line Stereo Reverb", getLocalBounds(), Justification::centred, 1);
    
    //Add and Draw Background Image
    Image background = ImageCache::getFromMemory (Images::cloudyBlue_jpg, Images::cloudyBlue_jpgSize);
    g.drawImageWithin (background, 0, 0, 600, 400, RectanglePlacement::fillDestination, false);
    
}

void SimpleDelayAudioProcessorEditor::resized()
{
    const int labelWidth = getWidth() / 6;
    const int labelHeight = 25;
    const int sliderWidth = getWidth() / 6;
    const int sliderHeight = getHeight() - labelHeight;
    
    
    numDelayDial.setBounds(0, 0, sliderWidth, sliderHeight);
    numDelayLabel.setBounds(0, sliderHeight, labelWidth, labelHeight);
    
    feedbackDial.setBounds(sliderWidth, 0, sliderWidth, sliderHeight);
    feedbackLabel.setBounds(sliderWidth, sliderHeight, labelWidth, labelHeight);
    
    dryWetSlider.setBounds(sliderWidth * 2, 0, sliderWidth, sliderHeight);
    dryWetLabel.setBounds(sliderWidth * 2, sliderHeight, labelWidth, labelHeight);
    
    roomSizeSlider.setBounds(sliderWidth * 3, 0, sliderWidth, sliderHeight);
    roomSizeLabel.setBounds(sliderWidth * 3, sliderHeight, labelWidth, labelHeight);
    
    dampingSlider.setBounds(sliderWidth * 4, 0, sliderWidth, sliderHeight);
    dampingLabel.setBounds(sliderWidth * 4, sliderHeight, labelWidth, labelHeight);
    
    roomWidthSlider.setBounds(sliderWidth * 5, 0, sliderWidth, sliderHeight);
    roomWidthLabel.setBounds(sliderWidth * 5, sliderHeight, labelWidth, labelHeight);
    
    delayMenu.setBounds(sliderWidth * 2, 80, 120, 40);
    BPM_Toggle.setBounds((sliderWidth * 2) + 120, 80, 100, 40);
}

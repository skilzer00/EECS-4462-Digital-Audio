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
class A1StarterAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener,private juce::ComboBox::Listener
{
public:
    A1StarterAudioProcessorEditor(A1StarterAudioProcessor&);
    ~A1StarterAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* combobox) override;
   
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    A1StarterAudioProcessor& audioProcessor;
   

    juce::ComboBox modeselect, octaveselect;
    


    juce::Slider arpSlider, durationslider; // [1]

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(A1StarterAudioProcessorEditor)

    
};

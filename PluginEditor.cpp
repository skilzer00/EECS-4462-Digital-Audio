/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A1StarterAudioProcessorEditor::A1StarterAudioProcessorEditor(A1StarterAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(320, 250);

    //Arpggiator Speed Slider
    arpSlider.setSliderStyle(juce::Slider::Rotary);
    arpSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    arpSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    arpSlider.setRange(0.0, 1.0, 0.05);
    arpSlider.setPopupDisplayEnabled(true, false, this);
    arpSlider.setTextValueSuffix(" Speed");
    arpSlider.setValue(0.5);
    addAndMakeVisible(arpSlider);
    arpSlider.addListener(this);

    //Arpggiator Duration Increase/Decrease Slider
    durationslider.setSliderStyle(juce::Slider::Rotary);
    durationslider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    durationslider.setRange(1.0, 5.0, 0.50);
    durationslider.setPopupDisplayEnabled(true, false, this);
    durationslider.setTextValueSuffix(" times Duration");
    durationslider.setValue(0.5);
    addAndMakeVisible(durationslider);
    durationslider.addListener(this);


    //Modes for Arpggiator
    modeselect.addItem("Ascending Order", 1);
    modeselect.addItem("Descending Order", 2);
    modeselect.addItem("Repeated Notes", 3);
    modeselect.addItem("Switch First Note With Note Before The Last", 4);
    addAndMakeVisible(&modeselect);
    modeselect.addListener(this);

    //Octave
    octaveselect.addItem("Increase Octave Cycle by 1", 1);
    octaveselect.addItem("Increase Octave Cycle by 2", 2);
    octaveselect.addItem("Increase Octave Cycle by 3", 3);
    octaveselect.addItem("Increase Octave Cycle by 4", 4);
    octaveselect.addItem("Increase Octave Cycle by 5", 5);
    addAndMakeVisible(&octaveselect);
    octaveselect.addListener(this);
}

A1StarterAudioProcessorEditor::~A1StarterAudioProcessorEditor()
{
}

//==============================================================================
void A1StarterAudioProcessorEditor::paint(juce::Graphics& g)
{
    // fill the whole window white
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // set the current drawing colour to black
    g.setColour(juce::Colours::floralwhite);

    // set the font size and draw text to the screen
    g.setFont(15.0f);

    g.drawFittedText("Arpeggiator", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
    g.drawFittedText("Arpeggiator Speed", 30, 90, getWidth(), 30, juce::Justification::bottomLeft, 1);
    g.drawFittedText("Duration", 220, 90, getWidth(), 30, juce::Justification::bottomLeft, 1);

}

void A1StarterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // sets the position and size of the slider with arguments (x, y, width, height)
   /* arpSlider.setBounds(40, 30, 100, getHeight() - 60);
    durationslider.setBounds(100, 30, 40, getHeight() - 60);*/

    auto border = 4;

    auto area = getLocalBounds();

    auto dialArea = area.removeFromTop(area.getHeight() / 2);
    arpSlider.setBounds(dialArea.removeFromLeft(dialArea.getWidth() / 2).reduced(border));
    durationslider.setBounds(dialArea.reduced(border));

    auto buttonHeight = 30;

    modeselect.setBounds(area.removeFromTop(buttonHeight).reduced(border));
    modeselect.setText("Please Select A Mode To Begin");
    octaveselect.setBounds(area.removeFromTop(buttonHeight).reduced(border));
    octaveselect.setText("To Increase Octave Cycle Please Use This ");
}

void A1StarterAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.arpSpeed = arpSlider.getValue();
    audioProcessor.duration = durationslider.getValue();
}

void A1StarterAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combobox)
{
     audioProcessor.mode = modeselect.getSelectedId();
     audioProcessor.octave = octaveselect.getSelectedId();

}
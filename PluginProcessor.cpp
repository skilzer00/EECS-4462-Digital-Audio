/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
A1StarterAudioProcessor::A1StarterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

A1StarterAudioProcessor::~A1StarterAudioProcessor()
{
}

//==============================================================================
const juce::String A1StarterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool A1StarterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool A1StarterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool A1StarterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double A1StarterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int A1StarterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int A1StarterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void A1StarterAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String A1StarterAudioProcessor::getProgramName(int index)
{
    return {};
}

void A1StarterAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void A1StarterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    notes.clear();                          // [1]
    currentNote = 0;                        // [2]
    lastNoteValue = -1;                     // [3]
    time = 0;                               // [4]
    rate = static_cast<float> (sampleRate); // [5]
    arpSpeed = 0.5;
    std::cout << arpSpeed;
    
}

void A1StarterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool A1StarterAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void A1StarterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{

    // We use the audio buffer to get timing information
    auto numSamples = buffer.getNumSamples();                                                       // [7]

    // get note duration
    auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - (arpSpeed)))));   // [8]

    for (const auto metadata : midi)                                                                // [9]
    {

        const auto msg = metadata.getMessage();

        

        for (int i = 0; i <= octave; i++) {
            if (mode == 2) {
                if (msg.isNoteOn()) {
                    notes.add(msg.getNoteNumber() + (i * -12));
                }
                else if (msg.isNoteOff()) {
                    notes.removeValue(msg.getNoteNumber() + (i * -12));
                }
            }

            else {
                if (msg.isNoteOn()) {
                    notes.add(msg.getNoteNumber() + (i * 12));
                }
                else if (msg.isNoteOff()) {
                    notes.removeValue(msg.getNoteNumber() + (i * 12));
                }
            }
        }
    }

        midi.clear();                                                                                 // [10]

        if (lastNoteValue == notes.getLast()) {
            noteDuration = noteDuration * duration;
        }

        if ((time + numSamples) >= noteDuration)                                                        // [11]
        {
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));     // [12]

            if (lastNoteValue > 0)                                                                      // [13]
            {
                midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0)                                                                       // [14]
            {
                if (mode == 1) {
                    /* Ascending Order*/
                    currentNote = (currentNote + 1) % notes.size();
                    lastNoteValue = notes[currentNote];
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);

                }
                else if (mode == 2) {
                    /*Descending Order*/
                    currentNote = (currentNote - 1) % notes.size();
                    if (currentNote < 0) {
                        currentNote = notes.size() - 1;
                    }
                    lastNoteValue = notes[currentNote];
                    midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                }
                else if (mode == 3) {
                    firstNote = notes.getFirst();
                    if (repeat == true) {
                        lastNoteValue = firstNote;
                        midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                        repeat = false;
                    }
                    else {
                        currentNote = (currentNote + 1) % notes.size();
                        if (currentNote == 0) {
                            currentNote = 1;
                        }
                        lastNoteValue = notes[currentNote];
                        midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                        repeat = true;
                    }

                }
                else if (mode == 4) {
                    
                    currentNote = (currentNote + 1) % notes.size();
                    notebeforelast = notes.size()-2;
  
                    if (notes[currentNote] == notes[firstNote]) {
                        lastNoteValue = notes[notebeforelast];
                        midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                    }
                    else if (notes[currentNote] == notes[notebeforelast]) {
                        lastNoteValue = notes[firstNote];
                        midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                    }
                    else {                      
                        lastNoteValue = notes[currentNote];
                        midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8) 127), offset);
                    }

                }
                

            }
        }
        time = (time + numSamples) % noteDuration;                                                      // [15]
    
}

//==============================================================================
bool A1StarterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* A1StarterAudioProcessor::createEditor()
{
    return new A1StarterAudioProcessorEditor(*this);
}

//==============================================================================
void A1StarterAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void A1StarterAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new A1StarterAudioProcessor();
}
/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleOscillorAudioProcessor::SimpleOscillorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SimpleOscillorAudioProcessor::~SimpleOscillorAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleOscillorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleOscillorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleOscillorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleOscillorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleOscillorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleOscillorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleOscillorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleOscillorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleOscillorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleOscillorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleOscillorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    wtOscillor.prepareSound(sampleRate, samplesPerBlock);
}

void SimpleOscillorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleOscillorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void SimpleOscillorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    auto numFrame = buffer.getNumSamples();

    if (midiMessages.getNumEvents() == 0)
    {
        wtOscillor.getNextAudioBlock(buffer, 0, numFrame);
    }
    else
    {
        int currentSample = 0;
        int nextSample = 0;

        for (auto midi : midiMessages)
        {
            auto msg = midi.getMessage();

            if (!msg.isNoteOnOrOff())
            {
                continue;
            }

            nextSample = midi.samplePosition;

            if (msg.isNoteOn())
            {
                wtOscillor.noteOn(juce::MidiMessage::getMidiNoteInHertz(msg.getNoteNumber()));
            }
            else
            {
                wtOscillor.noteOff();
            }

            wtOscillor.getNextAudioBlock(buffer, currentSample, nextSample - currentSample);
            currentSample = nextSample;
        }
        
        wtOscillor.getNextAudioBlock(buffer, currentSample, numFrame - currentSample);
    }
}

//==============================================================================
bool SimpleOscillorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleOscillorAudioProcessor::createEditor()
{
    return new SimpleOscillorAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleOscillorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleOscillorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleOscillorAudioProcessor();
}

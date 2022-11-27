/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ui/WaveTableOscillorUI.h"

//==============================================================================
/**
*/
class SimpleOscillorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleOscillorAudioProcessorEditor (SimpleOscillorAudioProcessor&);
    ~SimpleOscillorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleOscillorAudioProcessor& audioProcessor;

    WaveTableOscillorUI waveTableOscUI_;
    juce::Slider slider_wtPos_;
    juce::Slider slider_phase_;
    juce::ToggleButton toggle_retrig_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleOscillorAudioProcessorEditor)
};

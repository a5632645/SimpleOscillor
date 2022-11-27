/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleOscillorAudioProcessorEditor::SimpleOscillorAudioProcessorEditor(SimpleOscillorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    waveTableOscUI_(audioProcessor.wtOscillor)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(waveTableOscUI_);

    addAndMakeVisible(slider_wtPos_);
    slider_wtPos_.setRange(0.0f, 1.0f, 0.01f);
    slider_wtPos_.setValue(audioProcessor.wtOscillor.getWTposition(), juce::dontSendNotification);
    slider_wtPos_.onValueChange = [this]() {
        audioProcessor.wtOscillor.setWTposition(slider_wtPos_.getValue());
        waveTableOscUI_.repaint();
    };

    addAndMakeVisible(slider_phase_);
    slider_phase_.setRange(0.0f, 0.99f, 0.01f);
    slider_phase_.setValue(audioProcessor.wtOscillor.getPhase(), juce::dontSendNotification);
    slider_phase_.onValueChange = [this]() {
        audioProcessor.wtOscillor.setPhase(slider_phase_.getValue());
    };

    addAndMakeVisible(toggle_retrig_);
    toggle_retrig_.setToggleState(audioProcessor.wtOscillor.getReTrig(), juce::dontSendNotification);
    toggle_retrig_.onStateChange = [this]() {
        audioProcessor.wtOscillor.setReTrig(toggle_retrig_.getToggleState());
    };
}

SimpleOscillorAudioProcessorEditor::~SimpleOscillorAudioProcessorEditor()
{
}

//==============================================================================
void SimpleOscillorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.drawSingleLineText("wt pos", 0, slider_wtPos_.getBottom());
    g.drawSingleLineText("phase", 0, slider_phase_.getBottom());
    g.drawSingleLineText("reTrig", 0, toggle_retrig_.getBottom());
}

void SimpleOscillorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    waveTableOscUI_.setBounds(0, 0, getWidth(), getHeight() / 2);
    slider_wtPos_.setBounds(40, waveTableOscUI_.getBottom(), getWidth() - 50, 20);
    slider_phase_.setBounds(40, slider_wtPos_.getBottom(), getWidth() - 50, 20);
    toggle_retrig_.setBounds(40, slider_phase_.getBottom(), 20, 20);
}

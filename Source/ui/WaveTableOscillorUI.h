/*
  ==============================================================================

    WaveTableOscillorUI.h
    Created: 25 Nov 2022 8:26:43pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "..\data\WaveTableOscillor.h"
#include "TransparentButton.h"

//==============================================================================
/*
*/
class WaveTableOscillorUI  : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    WaveTableOscillorUI(WaveTableOscillor& osc);
    ~WaveTableOscillorUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    WaveTableOscillor& osc_;
    std::vector<double> sampleIndex_;
    juce::PathStrokeType brush_;

    juce::Label waveTableName_;
    juce::Label WTposition_;

    juce::AudioFormatManager manager_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveTableOscillorUI)
};

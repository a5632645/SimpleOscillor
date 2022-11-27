/*
  ==============================================================================

    TransparentButton.h
    Created: 26 Nov 2022 8:11:09pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TransparentButton  : public juce::TextButton
{
public:
    TransparentButton()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        setColour(juce::TextButton::buttonColourId, juce::Colours::transparentWhite);
        
    }

    ~TransparentButton() override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransparentButton)
};

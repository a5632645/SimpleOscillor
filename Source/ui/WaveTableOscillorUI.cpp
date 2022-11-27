/*
  ==============================================================================

    WaveTableOscillorUI.cpp
    Created: 25 Nov 2022 8:26:43pm
    Author:  mana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveTableOscillorUI.h"

//==============================================================================
WaveTableOscillorUI::WaveTableOscillorUI(WaveTableOscillor& osc)
    :osc_(osc), brush_(1.0f)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(waveTableName_);
    waveTableName_.setFont(juce::Font{ "Microsoft YaHei",16,juce::Font::FontStyleFlags::plain });
    waveTableName_.setText(osc_.getWaveTable()->getWaveTableName(), juce::dontSendNotification);

    addAndMakeVisible(WTposition_);

    manager_.registerBasicFormats();
}

WaveTableOscillorUI::~WaveTableOscillorUI()
{
}

void WaveTableOscillorUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    int wtpos = static_cast<int>(floor(osc_.getWTposition() * osc_.getWaveTable()->getWaveTableMaxPosition()));
    WTposition_.setText(juce::String{ wtpos }, juce::dontSendNotification);

    g.fillAll(juce::Colours::black);   // clear the background

    g.setColour(juce::Colours::grey); // a grey framework
    g.drawRect(getLocalBounds());

    for (int i = 0; i < getWidth(); ++i)
    {
        sampleIndex_[i] = static_cast<double>(i) / getWidth();
    }
    
    osc_.getWaveTable()->getWaveSamplesFixedWTpos(
        sampleIndex_,
        sampleIndex_,
        getWidth(),
        osc_.getWTposition()
    );

    int center = getHeight() / 2;
    int scale = center - 1;

    juce::Path p;
    p.startNewSubPath(0, center - scale * sampleIndex_[0]);

    int size = sampleIndex_.size();
    for (int i = 1; i < size; ++i) 
    {
        p.lineTo(i, center - scale * sampleIndex_[i]);
    }
    
    g.setColour(juce::Colours::lightgreen);
    g.strokePath(p, brush_); // draw waveforms

    g.setColour(juce::Colours::grey);
    g.drawRect(waveTableName_.getBounds());
    g.drawRect(WTposition_.getBounds());
}

void WaveTableOscillorUI::resized()
{
    sampleIndex_.resize(getWidth());

    waveTableName_.setBounds(getLocalBounds().withHeight(30));
    WTposition_.setBounds(5, getHeight() - 25, 40, 20);
}

bool WaveTableOscillorUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto& s : files)
    {
        if (s.contains(".wav"))
        {
            return true;
        }
    }

    return false;
}

void WaveTableOscillorUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto& s : files)
    {
        if (s.contains(".wav"))
        {
            juce::File f{ s };
            auto* reader = manager_.createReaderFor(f);
            if (reader == nullptr)
            {
                return;
            }

            auto* newTable = WaveTable::createFromAudioFile(reader);
            delete reader;

            newTable->setWaveTableName(f.getFileName());

            waveTableName_.setText(f.getFileName(), juce::dontSendNotification);

            osc_.setWaveTable(newTable);

            repaint();

            return;
        }
    }
}

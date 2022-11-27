/*
  ==============================================================================

    WaveTableOscillor.h
    Created: 25 Nov 2022 8:26:14pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RampGenerator.h"
#include "waveTables/BasicShape.h"

class WaveTableOscillor
{
public:
    WaveTableOscillor()
        :phase_(0.0), isNoteOn_(false), isRetrig_(true), waveTable_(new BasicShape)
    {
    }

    void prepareSound(double samplerate, int nFrameExpect)
    {
        rampGenerator_.prepareSound(samplerate);
        
        wtIndexs.resize(nFrameExpect);
        sampleIndexs.resize(nFrameExpect);
    }

    void getNextAudioBlock(juce::AudioBuffer<float>& buffer, int start, int nFrame)
    {
        if (!isNoteOn_)
        {
            return;
        }

        juce::ScopedLock sl(locker_);

        rampGenerator_.getNextRamp(sampleIndexs, nFrame);
        for (auto& wt : wtIndexs)
        {
            wt = waveTablePosition_.getNextValue();
        }

        int channelCount = buffer.getNumChannels();
        for (int i = 0; i < channelCount; ++i)
        {
            auto* pChannel = buffer.getWritePointer(i, start);
            waveTable_->getWaveSamples(sampleIndexs, wtIndexs, pChannel, nFrame);
        }
    }

    void noteOn(double frequency)
    {
        isNoteOn_ = true;
        rampGenerator_.setFrequency(frequency);
        if (isRetrig_)
        {
            rampGenerator_.resetPhase(phase_);
        }
    }

    void noteOff()
    {
        isNoteOn_ = false;
    }

    WaveTable* getWaveTable() const
    {
        return waveTable_.get();
    }

    void setWaveTable(WaveTable* newWaveTable)
    {
        juce::ScopedLock sl(locker_);

        waveTable_.reset(newWaveTable);
    }

    //=================================================parameters========================
    void setWTposition(double val)
    {
        val = juce::jlimit(0.0, 0.999, val);
        waveTablePosition_.setTargetValue(val);
    }

    double getWTposition() const
    {
        return waveTablePosition_.getTargetValue();
    }

    void setReTrig(bool val)
    {
        isRetrig_ = val;
    }

    double getReTrig() const
    {
        return isRetrig_;
    }

    void setPhase(double val)
    {
        phase_ = val;
    }

    double getPhase() const
    {
        return phase_;
    }

private:
    juce::SmoothedValue<double> waveTablePosition_;
    double phase_;
    bool isNoteOn_;
    bool isRetrig_;
    RampGenerator rampGenerator_;
    std::unique_ptr<WaveTable> waveTable_;

    std::vector<double> wtIndexs;
    std::vector<double> sampleIndexs;

    juce::CriticalSection locker_;
};
/*
  ==============================================================================

    Oscillor.h
    Created: 24 Nov 2022 9:27:56pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include "Phase.h"
#include <JuceHeader.h>

class RampGenerator
{
public:
    RampGenerator()
        :sampleRate_(48000.0), frequency_(50.0)
    {
    }

    void prepareSound(double samplerate)
    {
        sampleRate_ = samplerate;
    }

    /* set ramp frequency
    */
    void setFrequency(double new_frequency)
    {
        frequency_.setTargetValue(new_frequency);
    }

    /* get ramp frequency
    */
    double getFrequency() const
    {
        return frequency_.getTargetValue();
    }

    /* set phase
    */
    void resetPhase(double phase)
    {
        phase_.reset(phase);
    }

    /* please note that the buffer should have operator[] method
    * 
    *  @param buffer : the buffer to fill
    *  @param nFrame : the ramp length
    */
    void getNextRamp(auto& buffer, int nFrame)
    {
        for (int i = 0; i < nFrame; ++i)
        {
            buffer[i] = phase_.phase_;
            phase_.increase(frequency_.getNextValue() / sampleRate_);
        }
    }
private:
    Phase phase_;
    double sampleRate_;
    juce::SmoothedValue<double> frequency_;
};
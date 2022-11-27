/*
  ==============================================================================

    Phase.h
    Created: 24 Nov 2022 9:28:04pm
    Author:  mana

  ==============================================================================
*/

#pragma once

/* a phase struct that output 0.0 to 1.0
*/

struct Phase
{
    double phase_;

    Phase()
        :phase_(0.0) {}

    inline void reset(double p = 0.0)
    {
        phase_ = p;
    }

    inline void increase(double val)
    {
        phase_ += val;

        while (phase_ > 1.0)
        {
            phase_ -= 1.0;
        }
        while (phase_ < 0.0)
        {
            phase_ += 1.0;
        }
    }
};
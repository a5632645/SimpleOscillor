/*
  ==============================================================================

    BasicShape.h
    Created: 24 Nov 2022 9:36:28pm
    Author:  mana

  ==============================================================================
*/

#pragma once
#include "WaveTable.h"
#include <cmath>

struct BasicShape : public WaveTable
{
    static constexpr double twopi = 6.283185307179586476925286766559;
    static constexpr int kBasicShapeNum = 4;

    BasicShape()
        :WaveTable(kBasicShapeNum)
    {
        init();

        setWaveTableName("Basic Shapes");
    }

private:
    void init()
    {
        for (int i = 0; i < WaveTable::kMaxSamples; ++i)
        {
            double phase = i / (double)kMaxSamples;
            table_[0][i] = sin(phase * twopi); // sine
            table_[1][i] = asin(table_[0][i]) * 0.63661977236758134307553505349006; // tri
            table_[2][i] = -2 * (phase - 0.5); // saw
            table_[3][i] = phase < 0.5 ? 1.0 : -1.0; // square
        }
    }
};
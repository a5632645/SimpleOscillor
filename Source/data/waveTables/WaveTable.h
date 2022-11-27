/*
  ==============================================================================

    WaveTable.h
    Created: 24 Nov 2022 9:41:10pm
    Author:  mana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct WaveTable
{
public:
    static constexpr int kMaxSamples = 2048;
    static constexpr int kMaxWaveNum = 256;

    WaveTable(int numWaves = 0)
    {
        table_.resize(numWaves);
        for (auto& p : table_)
        {
            p = new double[kMaxSamples + 1];
        }
    }

    virtual ~WaveTable()
    {
        for (auto* w : table_)
        {
            delete[] w;
        }
    }

    /* @param index : from 0 -> 1
    *  @param wtpos : from 0 -> 1
    *  @param buffer: audio buffer to fill
    *  @param nFrame: num of sample to fill
    */
    void getWaveSamples(auto& index, auto& wtpos, auto& buffer, int nFrame)
    {
        for (int i = 0; i < nFrame; ++i)
        {
            auto tableSize = table_.size() - 1;
            double wtpostion = wtpos[i] * tableSize;
            double samplePos = index[i] * WaveTable::kMaxSamples;

            int INTwtpos = static_cast<int>(floor(wtpostion));
            int NextINTwtpos = 
            INTwtpos = INTwtpos < table_.size() ? INTwtpos : (table_.size() - 1);
            auto* wavesCur = table_[INTwtpos];

            int cur = static_cast<int>(floor(samplePos));
            int next = (cur + 1) % WaveTable::kMaxSamples;
            double frac = samplePos - static_cast<float>(cur);

            double curSample = waves[cur];
            double nexSample = waves[next];

            buffer[i] = curSample + frac * (nexSample - curSample);
        }
    }

    void getWaveSamplesFixedWTpos(auto& index, auto& buffer, int nFrame, double wtpos)
    {
        double wtpostion = wtpos * table_.size();
        for (int i = 0; i < nFrame; ++i)
        {
            double samplePos = index[i] * WaveTable::kMaxSamples;

            int INTwtpos = static_cast<int>(floor(wtpostion));
            auto* waves = table_[INTwtpos];

            int cur = static_cast<int>(floor(samplePos));
            int next = (cur + 1) % WaveTable::kMaxSamples;
            double frac = samplePos - static_cast<float>(cur);

            double curSample = waves[cur];
            double nexSample = waves[next];

            buffer[i] = curSample + frac * (nexSample - curSample);
        }
    }

    auto& getWaveTableName() const 
    {
        return waveTableName_;
    }

    void setWaveTableName(const juce::String& newName)
    {
        waveTableName_ = newName;
    }

    int getWaveTableMaxPosition() const
    {
        return static_cast<int>(table_.size());
    }

    /* create a wave table from audio file
    *  @param reader : the reader,please note this variable will not be deleted
    */
    static WaveTable* createFromAudioFile(juce::AudioFormatReader* reader)
    {
        constexpr auto kmaxLength = kMaxWaveNum * kMaxSamples;
        WaveTable* wt = nullptr;

        if (reader->lengthInSamples <= kmaxLength)
        {
            // no need for interp
            int numWaves = static_cast<int>(ceil(reader->lengthInSamples / static_cast<double>(kMaxSamples)));
            wt = new WaveTable(numWaves);
            float* tempBuffer = new float[kMaxSamples];

            for (int wtpos = 0; wtpos < numWaves; ++wtpos)
            {
                reader->read(&tempBuffer, 1, static_cast<juce::int64>(wtpos * kMaxSamples), kMaxSamples);
                for (int j = 0; j < kMaxSamples; ++j)
                {
                    double* wtBuffer = wt->table_[wtpos];
                    wtBuffer[j] = static_cast<double>(tempBuffer[j]);
                }
            }
            
            delete[] tempBuffer;
        }
        else
        {
            // need interp
            wt = new WaveTable(kMaxWaveNum);
            
            juce::AudioBuffer<float> tempBuffer;
            tempBuffer.setSize(1, reader->lengthInSamples);

            reader->read(&tempBuffer, 0, reader->lengthInSamples, 0, true, true);

            double chunkSize = reader->lengthInSamples / static_cast<double>(kMaxWaveNum);
            auto* pTempBuffer = tempBuffer.getReadPointer(0);
            for (int wtpos = 0; wtpos < kMaxWaveNum; ++wtpos)
            {
                for (int j = 0; j < kMaxSamples; ++j)
                {
                    double index = wtpos * chunkSize + chunkSize * j / kMaxSamples;
                    int current = static_cast<int>(floor(index)) % reader->lengthInSamples;
                    int next = (current + 1) % reader->lengthInSamples;
                    double factor = index - current;

                    double* pBuffer = wt->table_[wtpos];
                    pBuffer[j] = pTempBuffer[current] + factor * (pTempBuffer[next] - pTempBuffer[current]);
                }
            }
        }

        return wt;
    }

protected:
    std::vector<double*> table_;
    juce::String waveTableName_;

};
#pragma once
#include <vector>
#include <algorithm>

// Interpolated circular buffer used by all delay engines
class DelayLine
{
public:
    void prepare(double sampleRate, double maxDelaySeconds)
    {
        size = static_cast<int>(sampleRate * maxDelaySeconds) + 2;
        buffer.assign(size, 0.0f);
        writePos = 0;
    }

    void reset() { std::fill(buffer.begin(), buffer.end(), 0.0f); }

    void write(float sample)
    {
        buffer[writePos] = sample;
        if (++writePos >= size) writePos = 0;
    }

    // Linear interpolation read at fractional delay (samples)
    float read(float delaySamples) const
    {
        float readPos = static_cast<float>(writePos) - delaySamples;
        while (readPos < 0) readPos += static_cast<float>(size);
        int i0 = static_cast<int>(readPos) % size;
        int i1 = (i0 + 1) % size;
        float frac = readPos - static_cast<float>(static_cast<int>(readPos));
        return buffer[i0] + frac * (buffer[i1] - buffer[i0]);
    }

private:
    std::vector<float> buffer;
    int size = 0;
    int writePos = 0;
};

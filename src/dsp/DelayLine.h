#pragma once
#include <vector>
#include <algorithm>
#include <cstddef>

// Interpolated circular buffer used by all delay engines
class DelayLine
{
public:
    void prepare(double sampleRate, double maxDelaySeconds)
    {
        size = static_cast<size_t>(sampleRate * maxDelaySeconds) + 2;
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
        while (readPos < 0.0f) readPos += static_cast<float>(size);
        size_t i0 = static_cast<size_t>(readPos) % size;
        size_t i1 = (i0 + 1) % size;
        float frac = readPos - static_cast<float>(static_cast<size_t>(readPos));
        return buffer[i0] + frac * (buffer[i1] - buffer[i0]);
    }

private:
    std::vector<float> buffer;
    size_t size = 0;
    size_t writePos = 0;
};

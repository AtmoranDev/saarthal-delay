#pragma once
#include "DelayLine.h"

// Clean digital delay — transparent repeats, no coloration
class DigitalDelay
{
public:
    void prepare(double sampleRate)
    {
        sr = sampleRate;
        line.prepare(sampleRate, 3.0);
    }

    void reset() { line.reset(); }

    // delaySec: 0.01–3.0, feedback: 0.0–0.95, mix: 0.0–1.0
    float process(float input, float delaySec, float feedback, float mix)
    {
        float delaySamples = static_cast<float>(delaySec * sr);
        float delayed = line.read(delaySamples);
        line.write(input + delayed * feedback);
        return input * (1.0f - mix) + delayed * mix;
    }

private:
    DelayLine line;
    double sr = 44100.0;
};

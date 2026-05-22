#pragma once
#include "DelayLine.h"
#include <cmath>

static constexpr float kTwoPi = 6.28318530718f;
static constexpr float kPi    = 3.14159265359f;

// Tape delay: wow/flutter modulation + HF rolloff per repeat (Echoplex-style)
class TapeDelay
{
public:
    void prepare(double sampleRate)
    {
        sr = sampleRate;
        line.prepare(sampleRate, 3.0);

        // One-pole lowpass on feedback path (tape head HF loss ~4.5 kHz)
        lpCoeff = std::exp(-kTwoPi * (4500.0f / static_cast<float>(sampleRate)));
        lpState = 0.0f;

        lfoPhase = 0.0f;
        flutterPhase = 0.0f;
    }

    void reset()
    {
        line.reset();
        lpState = 0.0f;
        lfoPhase = flutterPhase = 0.0f;
    }

    // wowAmount: 0.0–1.0, flutterAmount: 0.0–1.0
    float process(float input, float delaySec, float feedback, float mix,
                  float wowAmount, float flutterAmount)
    {
        // Wow: ~1.2 Hz slow pitch variation
        lfoPhase += kTwoPi * 1.2f / static_cast<float>(sr);
        if (lfoPhase >= kTwoPi) lfoPhase -= kTwoPi;
        float wow = std::sin(lfoPhase) * wowAmount * 30.0f;

        // Flutter: ~9 Hz faster variation
        flutterPhase += kTwoPi * 9.0f / static_cast<float>(sr);
        if (flutterPhase >= kTwoPi) flutterPhase -= kTwoPi;
        float flutter = std::sin(flutterPhase) * flutterAmount * 5.0f;

        float delaySamples = static_cast<float>(delaySec * sr) + wow + flutter;
        delaySamples = std::max(1.0f, delaySamples);

        float delayed = line.read(delaySamples);

        // HF rolloff on feedback path
        lpState = delayed * (1.0f - lpCoeff) + lpState * lpCoeff;
        line.write(input + lpState * feedback);

        return input * (1.0f - mix) + delayed * mix;
    }

private:
    DelayLine line;
    double sr = 44100.0;
    float lpCoeff = 0.0f;
    float lpState = 0.0f;
    float lfoPhase = 0.0f;
    float flutterPhase = 0.0f;
};

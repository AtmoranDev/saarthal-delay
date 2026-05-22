#pragma once
#include "DelayLine.h"
#include <cmath>

// Analog BBD-style delay: input/output lowpass + mild saturation on feedback
class AnalogDelay
{
public:
    void prepare(double sampleRate)
    {
        sr = sampleRate;
        line.prepare(sampleRate, 3.0);

        // BBD input/output anti-aliasing filter (~3.5 kHz)
        const float fc = 3500.0f / static_cast<float>(sampleRate);
        const float twoPi = 6.28318530718f;
        inputLpCoeff  = std::exp(-twoPi * fc);
        outputLpCoeff = std::exp(-twoPi * fc);

        inputLp = outputLp = feedbackLp = 0.0f;

        // Slightly brighter cutoff for feedback path (~5 kHz)
        feedbackLpCoeff = std::exp(-twoPi * (5000.0f / static_cast<float>(sampleRate)));
    }

    void reset()
    {
        line.reset();
        inputLp = outputLp = feedbackLp = 0.0f;
    }

    float process(float input, float delaySec, float feedback, float mix)
    {
        // Pre-filter (BBD bandwidth limit)
        inputLp = input * (1.0f - inputLpCoeff) + inputLp * inputLpCoeff;

        float delaySamples = static_cast<float>(delaySec * sr);
        float delayed = line.read(delaySamples);

        // Post-filter on delayed signal
        outputLp = delayed * (1.0f - outputLpCoeff) + outputLp * outputLpCoeff;

        // Soft saturation + lowpass on feedback (BBD charge leakage)
        feedbackLp = outputLp * (1.0f - feedbackLpCoeff) + feedbackLp * feedbackLpCoeff;
        float satFeedback = std::tanh(feedbackLp * feedback * 1.5f) / 1.5f;

        line.write(inputLp + satFeedback);

        return input * (1.0f - mix) + outputLp * mix;
    }

private:
    DelayLine line;
    double sr = 44100.0;
    float inputLpCoeff = 0.0f,  inputLp  = 0.0f;
    float outputLpCoeff = 0.0f, outputLp = 0.0f;
    float feedbackLpCoeff = 0.0f, feedbackLp = 0.0f;
};

#pragma once
#include <JuceHeader.h>
#include "dsp/DigitalDelay.h"
#include "dsp/TapeDelay.h"
#include "dsp/AnalogDelay.h"

class SaarthalDelayProcessor : public juce::AudioProcessor
{
public:
    SaarthalDelayProcessor();
    ~SaarthalDelayProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Saarthal Delay"; }
    bool   acceptsMidi()  const override { return false; }
    bool   producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 3.0; }

    int  getNumPrograms()    override { return 1; }
    int  getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    enum class Mode { Digital = 0, Tape, Analog };

private:
    DigitalDelay digitalL, digitalR;
    TapeDelay    tapeL,    tapeR;
    AnalogDelay  analogL,  analogR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaarthalDelayProcessor)
};

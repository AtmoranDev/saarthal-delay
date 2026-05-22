#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout SaarthalDelayProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "time", "Time", juce::NormalisableRange<float>(0.01f, 2.0f, 0.001f, 0.5f), 0.5f,
        juce::AudioParameterFloatAttributes().withLabel("s")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "feedback", "Feedback", juce::NormalisableRange<float>(0.0f, 0.95f, 0.01f), 0.4f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "wow", "Wow", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.3f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "flutter", "Flutter", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.2f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "mode", "Mode",
        juce::StringArray{ "Digital", "Tape", "Analog" }, 0));

    return layout;
}

SaarthalDelayProcessor::SaarthalDelayProcessor()
    : AudioProcessor(BusesProperties()
          .withInput ("Input",  juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

void SaarthalDelayProcessor::prepareToPlay(double sampleRate, int)
{
    digitalL.prepare(sampleRate); digitalR.prepare(sampleRate);
    tapeL.prepare(sampleRate);    tapeR.prepare(sampleRate);
    analogL.prepare(sampleRate);  analogR.prepare(sampleRate);
}

void SaarthalDelayProcessor::releaseResources()
{
    digitalL.reset(); digitalR.reset();
    tapeL.reset();    tapeR.reset();
    analogL.reset();  analogR.reset();
}

void SaarthalDelayProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const float time     = *apvts.getRawParameterValue("time");
    const float feedback = *apvts.getRawParameterValue("feedback");
    const float mix      = *apvts.getRawParameterValue("mix");
    const float wow      = *apvts.getRawParameterValue("wow");
    const float flutter  = *apvts.getRawParameterValue("flutter");
    const int   modeIdx  = static_cast<int>(*apvts.getRawParameterValue("mode"));
    const auto  mode     = static_cast<Mode>(modeIdx);

    auto* left  = buffer.getWritePointer(0);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float inL = left[i];
        float inR = right ? right[i] : inL;

        float outL, outR;

        switch (mode)
        {
            case Mode::Digital:
                outL = digitalL.process(inL, time, feedback, mix);
                outR = digitalR.process(inR, time, feedback, mix);
                break;
            case Mode::Tape:
                outL = tapeL.process(inL, time, feedback, mix, wow, flutter);
                outR = tapeR.process(inR, time, feedback, mix, wow, flutter);
                break;
            case Mode::Analog:
                outL = analogL.process(inL, time, feedback, mix);
                outR = analogR.process(inR, time, feedback, mix);
                break;
            default:
                outL = inL;
                outR = inR;
        }

        left[i] = outL;
        if (right) right[i] = outR;
    }
}

void SaarthalDelayProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SaarthalDelayProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState && xmlState->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorEditor* SaarthalDelayProcessor::createEditor()
{
    return new SaarthalDelayEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaarthalDelayProcessor();
}

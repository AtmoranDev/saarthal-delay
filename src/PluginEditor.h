#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ui/KnobLookAndFeel.h"

class SaarthalDelayEditor : public juce::AudioProcessorEditor
{
public:
    explicit SaarthalDelayEditor(SaarthalDelayProcessor&);
    ~SaarthalDelayEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SaarthalDelayProcessor& processor;
    KnobLookAndFeel knobLnF;

    // Knobs
    juce::Slider timeKnob, feedbackKnob, mixKnob, wowKnob, flutterKnob;
    juce::Label  timeLabel, feedbackLabel, mixLabel, wowLabel, flutterLabel;

    // Mode selector
    juce::ComboBox modeBox;
    juce::Label    modeLabel;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<Attachment>      timeAtt, feedbackAtt, mixAtt, wowAtt, flutterAtt;
    std::unique_ptr<ComboAttachment> modeAtt;

    void setupKnob(juce::Slider& knob, juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaarthalDelayEditor)
};

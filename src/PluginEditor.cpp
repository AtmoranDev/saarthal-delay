#include "PluginEditor.h"

SaarthalDelayEditor::SaarthalDelayEditor(SaarthalDelayProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(520, 220);
    setLookAndFeel(&knobLnF);

    setupKnob(timeKnob,     timeLabel,     "TIME");
    setupKnob(feedbackKnob, feedbackLabel, "FEEDBACK");
    setupKnob(mixKnob,      mixLabel,      "MIX");
    setupKnob(wowKnob,      wowLabel,      "WOW");
    setupKnob(flutterKnob,  flutterLabel,  "FLUTTER");

    // Mode selector
    modeBox.addItemList({ "Digital", "Tape", "Analog" }, 1);
    modeBox.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(modeBox);

    modeLabel.setText("MODE", juce::dontSendNotification);
    modeLabel.setJustificationType(juce::Justification::centred);
    modeLabel.setFont(juce::Font("Courier New", 11.0f, juce::Font::plain));
    modeLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFCCCCCC));
    addAndMakeVisible(modeLabel);

    // APVTS attachments
    auto& apvts = processor.apvts;
    timeAtt     = std::make_unique<Attachment>(apvts, "time",     timeKnob);
    feedbackAtt = std::make_unique<Attachment>(apvts, "feedback", feedbackKnob);
    mixAtt      = std::make_unique<Attachment>(apvts, "mix",      mixKnob);
    wowAtt      = std::make_unique<Attachment>(apvts, "wow",      wowKnob);
    flutterAtt  = std::make_unique<Attachment>(apvts, "flutter",  flutterKnob);
    modeAtt     = std::make_unique<ComboAttachment>(apvts, "mode", modeBox);
}

SaarthalDelayEditor::~SaarthalDelayEditor()
{
    setLookAndFeel(nullptr);
}

void SaarthalDelayEditor::setupKnob(juce::Slider& knob, juce::Label& label, const juce::String& text)
{
    knob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 16);
    addAndMakeVisible(knob);

    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::Font("Courier New", 11.0f, juce::Font::plain));
    label.setColour(juce::Label::textColourId, juce::Colour(0xFFCCCCCC));
    addAndMakeVisible(label);
}

void SaarthalDelayEditor::paint(juce::Graphics& g)
{
    // Dark pedal background
    g.fillAll(juce::Colour(0xFF111111));

    // Top banner
    g.setColour(juce::Colour(0xFF1E1E1E));
    g.fillRect(0, 0, getWidth(), 36);

    g.setColour(juce::Colour(0xFFE8A020));
    g.setFont(juce::Font("Courier New", 20.0f, juce::Font::bold));
    g.drawText("SAARTHAL DELAY", getLocalBounds().withHeight(36), juce::Justification::centred);

    // Subtle separator line under banner
    g.setColour(juce::Colour(0xFF333333));
    g.drawHorizontalLine(36, 0.0f, static_cast<float>(getWidth()));
}

void SaarthalDelayEditor::resized()
{
    const int top     = 44;
    const int knobW   = 80;
    const int knobH   = 80;
    const int labelH  = 18;
    const int spacing = 8;
    const int totalKnobs = 5;
    const int modeW   = 90;

    // Five knobs evenly across left portion, mode combo on right
    const int usableW = getWidth() - modeW - spacing * 2;
    const int step    = usableW / totalKnobs;

    auto placeKnob = [&](juce::Slider& knob, juce::Label& label, int idx)
    {
        int x = spacing + idx * step + (step - knobW) / 2;
        knob.setBounds(x, top, knobW, knobH);
        label.setBounds(x, top + knobH, knobW, labelH);
    };

    placeKnob(timeKnob,     timeLabel,     0);
    placeKnob(feedbackKnob, feedbackLabel, 1);
    placeKnob(mixKnob,      mixLabel,      2);
    placeKnob(wowKnob,      wowLabel,      3);
    placeKnob(flutterKnob,  flutterLabel,  4);

    const int modeX = getWidth() - modeW - spacing;
    modeBox.setBounds(modeX, top + 20, modeW, 28);
    modeLabel.setBounds(modeX, top, modeW, 18);
}

#pragma once
#include <JuceHeader.h>

// Dark hardware-pedal aesthetic: matte black knobs with amber indicator line
class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    KnobLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId,             juce::Colours::transparentBlack);
        setColour(juce::Slider::rotarySliderFillColourId,  juce::Colour(0xFFE8A020));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF1A1A1A));
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override
    {
        const float radius   = std::min(width, height) * 0.5f - 4.0f;
        const float centreX  = static_cast<float>(x) + static_cast<float>(width)  * 0.5f;
        const float centreY  = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        const float angle    = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Outer ring
        g.setColour(juce::Colour(0xFF2A2A2A));
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

        g.setColour(juce::Colour(0xFF444444));
        g.drawEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 2.0f);

        // Arc fill
        juce::Path arc;
        arc.addCentredArc(centreX, centreY, radius - 3.0f, radius - 3.0f,
                          0.0f, rotaryStartAngle, angle, true);
        g.setColour(juce::Colour(0xFFE8A020));
        g.strokePath(arc, juce::PathStrokeType(3.0f));

        // Pointer line
        const float lineLength = radius * 0.55f;
        const float lineX = centreX + lineLength * std::sin(angle);
        const float lineY = centreY - lineLength * std::cos(angle);
        g.setColour(juce::Colour(0xFFFFCC44));
        g.drawLine(centreX, centreY, lineX, lineY, 2.5f);
    }

    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        g.setColour(juce::Colour(0xFFCCCCCC));
        g.setFont(juce::Font("Courier New", 11.0f, juce::Font::plain));
        g.drawText(label.getText(), label.getLocalBounds(), juce::Justification::centred, true);
    }
};

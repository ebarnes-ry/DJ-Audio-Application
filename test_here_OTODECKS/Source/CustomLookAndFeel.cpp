/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 11 Sep 2023 3:13:06pm
    Author:  Emmab

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{
    //setColour(juce::Slider::thumbColourId, juce::Colours::red);

}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
{
    //notch intervals
    float notchSpace;
    int notches = 15;
    //if vertical
    if (slider.isVertical()) {//volume sliders and speed sliders
        auto centerX = (x + width) / 2.0f;
        auto sliderWidth = width / 8.0f;

        notchSpace = height / notches;
        for (int i = 0; i < notches + 1; i++) {//iterate through to draw the notches "stack vibes"
            auto vertPos = jmap(i, 0, notches, y, y + height);
            if (i > 0 && i < notches) {
                g.setColour(Colours::beige);
                g.drawLine(x + width / 3.0f, vertPos, x + width / 1.5f, vertPos);
            }
            else {
                g.setColour(juce::Colours::black);
            }
        }

        //background
        g.setColour(Colours::black);
        g.fillRoundedRectangle(centerX - sliderWidth / 2.0f, y, sliderWidth, height, 10.0f);

        //thumb
        g.setColour(Colours::black);
        g.fillRect(x + width / 4.0f, sliderPos - notchSpace * 0.4f, width / 2.0f, notchSpace * 0.75);

        g.setColour(Colours::beige);
        g.drawLine(x + width / 3.0f, sliderPos, x + width / 3.0f * 2.0f, sliderPos, 3.0f);
    }
    //if horizontal (crossfade slider)
    else if (slider.isHorizontal()) {
        auto centerY = (y + height) / 2.0f;
        auto sliderWidth = height/8.0f;

        float initialSliderPos = minSliderPos + (maxSliderPos - minSliderPos) / 2.0f;
        //draw other background


        //draw notches
        notchSpace = width / notches;
        for (int i = 0; i < notches + 1; i++) {
            auto horPos = jmap(i, 0, notches, x, x + width);
            if (i > 0 && i < notches) {
                g.setColour(juce::Colours::beige);
                g.drawLine(horPos, y + height / 5.0f, horPos, y + height, 0.5f);
            }
            else {
                g.setColour(juce::Colours::black);
                g.drawLine(horPos, y + height * 0.25f, horPos, y + height * 0.75f, 4.0f);
            }
        }

        //background
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(x, centerY - sliderWidth / 2.0f, width, sliderWidth, 10.0f);

        

        //thumb
        g.setColour(Colours::black);
        g.fillRect(sliderPos - notchSpace * 0.375f, y + height / 4.0f, notchSpace * 0.75f, height/2.0f);
        g.setColour(Colours::beige);
        g.drawLine(sliderPos, y + height / 3.0f, sliderPos, y + height / 3.0f * 2.0f, 0.75f);
    }
}



/*
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    juce::Rectangle<float> bounds(x, y, static_cast<float>(width), static_cast<float>(height));
    const float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;

    const float centerX = bounds.getCentreX();
    const float centerY = bounds.getCentreY();

    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(juce::Colours::darkblue);
    g.fillEllipse(bounds);

    g.setColour(juce::Colours::black);
    g.drawEllipse(bounds, 1.0f);

    drawNotches(g, centerX, centerY, radius, rotaryStartAngle, rotaryEndAngle, 24);

    g.setColour(juce::Colours::white);
    g.drawLine(centerX, centerY,
        centerX + std::cos(angle - juce::MathConstants<float>::halfPi) * radius,
        centerY + std::sin(angle - juce::MathConstants<float>::halfPi) * radius,
        2.0f);


    //pointer
    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));

    //pointer fill
    g.setColour(juce::Colours::white);
    g.fillPath(p);

}

void CustomLookAndFeel::drawNotches(juce::Graphics& g, const float centerX, const float centerY, const float radius,
    const float startAngle, const float endAngle, const int numNotches)
{
    const float notchSpacing = (endAngle - startAngle) / (numNotches - 1);

    for (int i = 0; i < numNotches; i++) {
        const float angle = startAngle + i * notchSpacing;
        const float cosAngle = std::cos(angle - juce::MathConstants<float>::halfPi);
        const float sinAngle = std::sin(angle - juce::MathConstants<float>::halfPi);

        const float innerX = centerX + cosAngle * (radius - 10.0f);
        const float innerY = centerY + sinAngle * (radius - 10.0f);

        const float outerX = centerX + cosAngle * radius;
        const float outerY = centerY + sinAngle * radius;

        g.drawLine(innerX, innerY, outerX, outerY, 2.0f);
    }
}
*/
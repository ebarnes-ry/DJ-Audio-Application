/*
  ==============================================================================

    CustomPosSlider.cpp
    Created: 12 Sep 2023 3:47:12am
    Author:  Emmab

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomPosSlider.h"

//==============================================================================
CustomPosSlider::CustomPosSlider()
{
    

}

CustomPosSlider::~CustomPosSlider()
{
}


void CustomPosSlider::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider&)
{
    DBG("Custom rotary slider is called");
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;

    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(Colours::darkgrey); 
    g.fillEllipse(rx, ry, rw, rw);

    g.setColour(Colours::black); 
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    // Pointer
    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(Colours::beige); 
    g.fillPath(p);
}

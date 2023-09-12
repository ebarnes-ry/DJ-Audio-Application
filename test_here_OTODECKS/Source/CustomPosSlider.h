/*
  ==============================================================================

    CustomPosSlider.h
    Created: 12 Sep 2023 3:47:12am
    Author:  Emmab

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomPosSlider  : public LookAndFeel_V4
{
public:
    CustomPosSlider();
    ~CustomPosSlider() override;

    //new look for rot sliders
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider&) override;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomPosSlider)
};

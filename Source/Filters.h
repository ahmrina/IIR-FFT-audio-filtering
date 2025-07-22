/*
  ==============================================================================

    Filters.h
    Author:  rinea

  ==============================================================================
*/

#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include "JuceHeader.h"

class Filters {

public:

    Filters();

    float snapToZero(float val);

    float clip_value(float value);

    juce::AudioBuffer<float>iir(juce::AudioBuffer<float>& buffer, float sampling_rate,
                                float cut_off_freq, bool lowpass);

    juce::AudioBuffer<float>fft(juce::AudioBuffer<float>& buffer, int fft_order,
                                float cut_off_freq, float sampling_rate, bool lowpass);

};

#endif // FILTERS_H


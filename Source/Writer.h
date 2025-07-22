/*
  ==============================================================================

    Writer.h
    Author:  rinea

  ==============================================================================
*/

#ifndef WRITER_H
#define WRITER_H

#include <vector>
#include <string>
#include <JuceHeader.h>
#include <iostream>

class Writer {

public: 
    juce::WavAudioFormat wav_format;

    Writer();

    void write(juce::AudioBuffer<float>& buffer,
        juce::File& output,
        double sample_rate);


};



#endif //WRITER_H

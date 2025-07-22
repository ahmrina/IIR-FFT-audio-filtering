/*
  ==============================================================================

    Reader.h
    Author:  rina

  ==============================================================================
*/

#ifndef READER_H
#define READER_H

#include <vector>
#include <string>
#include <JuceHeader.h>
#include <iostream>


class Reader {

	std::vector<std::string> file_names;

public:

	Reader(std::vector<std::string>input);

	juce::File create_single_file(const std::string& name);

	void create_files(std::vector<juce::File>* audio_files, bool print_elements);

	double read_audio(juce::AudioBuffer<float>& buffer, juce::File& audio_file,
		std::unique_ptr<juce::AudioFormatReader>& reader, juce::AudioFormatManager& format_manager);

	//double get_sample_Rate(std::unique_ptr<juce::AudioFormatReader>& reader);

};
#endif //READER_H

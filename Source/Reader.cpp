/*
  ==============================================================================

    Reader.cpp
    Author:  rina

  ==============================================================================
*/
#include <iostream>
#include <JuceHeader.h>
#include "Reader.h"
#include <juce_audio_formats/juce_audio_formats.h>

using namespace std;
//using namespace juce;


	/*
	constructor for the reader
	args:
		 String input[]: array of input file names
		 int input_size: size of array  */
	Reader::Reader(std::vector<std::string> input) {

		
		for (int i = 0; i < input.size(); i++) {
		    file_names.push_back(input[i]);
		}
	} // end constructor


	/*
	initializes a file given its name
	args:
	  String name
	 returns:
		   juce::file  */
	juce::File Reader::create_single_file(const string& name) {

		juce::File path = juce::File::getSpecialLocation(juce::File::currentExecutableFile);

		/*juce::File root = path.getParentDirectory();*/
		juce::File projectRoot = path.getParentDirectory()
			.getParentDirectory() 
			.getParentDirectory() 
			.getParentDirectory() 
			.getParentDirectory() 
			.getParentDirectory(); 

		juce::File file = projectRoot.getChildFile("Source").getChildFile(name);

		return file;
	}


	/* creates a vector of JUCE files from the file_names
	* args:
	*      vector<string> file_names (initialized in constructor)
	*      bool print_elements (prints elements looked at to verify all were read)
	* returns:
	*      juce::vector of files*/
	void Reader::create_files(std::vector<juce::File> *audio_files, bool print_elements) {
		// create files for each audio
		audio_files->clear();

		for (int i = 0; i < file_names.size(); i++) {

			audio_files->push_back(create_single_file(file_names[i]));
			std::cout << audio_files->at(i).getFullPathName() << std::endl;
			if (print_elements) {
				cout << "looked at " << file_names[i] << endl;
			}
		}
	}


	/*
	reads audio into an audio buffer used for transformations
	args: 
	     AudioBuffer<float> buffer 
		 File audio_file 
		 AudioFormatReader reader
		 AudioFormatManager format_manager
    returns:
	      void (only writes the audio file into the buffer)*/
	//double Reader::read_audio(juce::AudioBuffer<float> &buffer, juce::File &audio_file, 
	//	                    std::unique_ptr<juce::AudioFormatReader> &reader, 
	//	                    juce::AudioFormatManager &format_manager) {
	//	
	//	if (audio_file.exists()) {

	//		reader.reset(format_manager.createReaderFor(audio_file));


	//			int num_channels = (int)(reader->numChannels);
	//			int num_samples = static_cast<int>(reader->lengthInSamples);
	//			double sample_rate = reader->sampleRate;
	//		/*	cout << "num_channels: " << num_channels << "num_samples: " << num_samples << endl;*/
	//			buffer.setSize(num_channels, num_samples);

	//			reader->read(&buffer, 0, num_samples, 0, true, true);
	//			cout << "audio file read " << endl;
	//		
	//			reader.reset();

	//			return sample_rate;
	//		
	//	}
	//	else return 0.0;
	//
	//}

	double Reader::read_audio(juce::AudioBuffer<float>& buffer,
		juce::File& audio_file,
		std::unique_ptr<juce::AudioFormatReader>& reader,
		juce::AudioFormatManager& format_manager)
	{
		if (!audio_file.existsAsFile())
		{
			std::cout << "File does not exist: " << audio_file.getFullPathName() << std::endl;
			return 0.0;
		}

		reader.reset(format_manager.createReaderFor(audio_file));

		if (reader == nullptr)
		{
			std::cout << "Failed to create reader for: " << audio_file.getFullPathName() << std::endl;
			return 0.0;
		}

		int num_channels = static_cast<int>(reader->numChannels);
		int num_samples = static_cast<int>(reader->lengthInSamples);
		double sample_rate = reader->sampleRate;

		buffer.setSize(num_channels, num_samples, false, true, true); // clear extra flags
		buffer.clear();

		reader->read(&buffer, 0, num_samples, 0, true, true);

		/*std::cout << "read: " << audio_file.getFileName()<< "first sample: " << buffer.getSample(0, 0) << std::endl;*/

		return sample_rate;
	}


	
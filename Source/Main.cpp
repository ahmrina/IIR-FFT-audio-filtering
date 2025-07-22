#include <iostream>
#include <JuceHeader.h>
#include <juce_core/juce_core.h> 
#include "Reader.h"
#include <windows.h>
#include <limits>
#include "Writer.h"
#include "Filters.h"

using namespace std;
using namespace juce;


void print_samples(AudioBuffer<float> buffer, int step) {
    // prints element of a buffer
    for (int i = 0; i < buffer.getNumChannels(); i+=step) {
        for (int j = 0; j < buffer.getNumSamples(); j++) {
            cout << buffer.getSample(i, j) << endl;
           }
             }


}

int main (int argc, char* argv[])
{
    const int num_files = 21;
    
    std::vector<string> zjerm_stems_names = {"beat & clap.wav", //0
                                             "beat 2.wav",      //1
                                             "bridge orchestral.wav",//2
                                             "chorus wo shnrit.wav",//3
                                             "chorus.wav",//4
                                             "ciftelia bridge.wav",//5
                                             "ciftelia intense after intro.wav", //6
                                             "ciftelia intro.wav", //7
                                             "hook.wav",//8
                                             "kola's monologue.wav", //9
                                             "main chorus.wav",//10
                                             "monologue beat 2.wav",//11
                                             "monologue beat.wav", //12
                                             "monologue beat.wav",//13
                                             "orchestra bridge last part.wav",//14
                                             "orchestra bridge.wav", //15
                                             "orchestra chorus.wav",//16
                                             "shnrit faded.wav",//17
                                             "shnrit shnrit.wav",//18
                                             "shnrit short.wav",//19
                                             "verse 1 n chorus.wav"};//20

    //EXAMPLE OF LOADING, FILTERING AND SAVING BACK AUDIO
    juce::AudioFormatManager format_manager;
    std::unique_ptr<juce::AudioFormatReader> format_reader;
    format_manager.registerBasicFormats();

    Reader reader(zjerm_stems_names);
    Writer writer;

    vector<juce::File> audio_files;

    reader.create_files(&audio_files, false);
    std::vector<juce::AudioBuffer<float>> buffers; // buffers where stems are read intog 

    buffers.emplace_back();
    buffers.resize(num_files);
    double sample_rate1 = reader.read_audio(buffers[6], audio_files[6], format_reader, format_manager);

  
    Filters filter;
    juce::String file_name2 = "chorus ciftelia iir 50 cut off - lowpass.wav";
   juce::File output_path2 = juce::File::getSpecialLocation(juce::File::userDesktopDirectory)
        .getChildFile(file_name2);
 
   AudioBuffer<float> fft_filtered = filter.iir(buffers[6], 44100.0, 50.0, true);

 
   writer.write(fft_filtered, output_path2, sample_rate1);

   

    return 0;
}

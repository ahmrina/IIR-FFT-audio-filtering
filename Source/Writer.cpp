/*
  ==============================================================================

    Writer.cpp
    Author:  rinea

  ==============================================================================
*/

#include "Writer.h"
#include <juce_audio_formats/juce_audio_formats.h> // for .wav file

Writer::Writer() {

// empty constructor 

}

/*
writes a new file
args: 
     buffer (where audio is loaded)
     File output (name of file)
     AudioFormatReader reader
returns:
      void (writes a new file of given audio buffer)
 */
//void Writer::write(juce::AudioBuffer<float>& buffer,
//    juce::File& output,
//    double sample_rate)
//{
//    juce::File parentDir = output.getParentDirectory();
//    if (!parentDir.exists())
//        parentDir.createDirectory();
//
//
//    std::unique_ptr<juce::FileOutputStream> stream(output.createOutputStream());
//    //double sample_rate = reader->sampleRate; // read access violation here
//
//    if (stream != nullptr)
//    {
//        juce::WavAudioFormat wav_format;
//
//        std::unique_ptr<juce::AudioFormatWriter> writer(
//            wav_format.createWriterFor(stream.get(),
//                sample_rate,
//                buffer.getNumChannels(),
//                16, {}, 0));
//
//        if (writer != nullptr)
//        {
//            stream.release(); // writer owns the stream 
//            writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
//            std::cout << "saved in path: " << output.getFullPathName() << std::endl;
//        }
//        else
//        {
//            std::cout << "writer is null" << std::endl;
//        }
//    }
//    else
//    {
//        std::cout << "stream null!" << std::endl;
//    }
//}

void Writer::write(juce::AudioBuffer<float>& buffer,
    juce::File& output,
    double sample_rate)
{
    if (buffer.getNumSamples() == 0 || buffer.getNumChannels() == 0)
    {
        std::cout << "buffer is all zeros" <<std::endl;
        return;
    }

    juce::File parentDir = output.getParentDirectory();
    if (!parentDir.exists())
        parentDir.createDirectory();

    juce::WavAudioFormat wav_format;

    std::unique_ptr<juce::FileOutputStream> stream(output.createOutputStream());

    if (stream == nullptr)  std::cout << "stream empty" << std::endl;
    std::unique_ptr<juce::AudioFormatWriter> writer(
        wav_format.createWriterFor(stream.get(),
            sample_rate,
            buffer.getNumChannels(),
            16,
            {},
            0));

    if (writer == nullptr)
    {
        std::cout << "writer creation failed.\n";
        return;
    }
    stream.release(); // hand ownership to writer
    const bool ok = writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    if (ok)
    {
        std::cout << "saved in path: " << output.getFullPathName() << std::endl;
    }
}
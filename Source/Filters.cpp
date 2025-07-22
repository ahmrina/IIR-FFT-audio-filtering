/*
  ==============================================================================

    Filters.cpp
    Author:  rina

  ==============================================================================
*/

#include "Filters.h"
#include <iostream>
#include "JuceHeader.h"
#include <vector>
#include <cmath>
#define USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Filters::Filters() {
    // empty constructor
}



// returns 0 if value passed has no meaning i.e is negative or very close to zero
 float Filters::snapToZero(float val) {

    float epsilon = std::numeric_limits<float>::epsilon();
    if (val <= epsilon) {
        val = 0;
    }
    return val;
}


 // clips values to be between [1.0, -1.0]
 float Filters::clip_value(float value) {
     /* return value > 1.0 ? 1.0 : value;*/
     if (value > 1.0) return 1.0;
     if (value < -1.0) return -1.0;
     else return value;
 }


/*
low/High Pass filter IIR
args:
     AudioBuffer<float> &buffer (buffer where the audio file is loaded)
     float sampling_rate,
     float cut_off_freq
returns:
     a new buffer of the filtered audio
*/
 juce::AudioBuffer<float> Filters::iir(juce::AudioBuffer<float>& buffer, float sampling_rate, 
                                       float cut_off_freq, bool lowpass) {
     using Filter = juce::dsp::IIR::Filter<float>;
     Filters filter;

     auto coeffs = lowpass ? juce::dsp::IIR::Coefficients<float>::makeHighPass(sampling_rate, cut_off_freq) :
                             juce::dsp::IIR::Coefficients<float>::makeLowPass(sampling_rate, cut_off_freq);

     int num_channels = buffer.getNumChannels();
     int num_samples = buffer.getNumSamples();
     juce::AudioBuffer<float> filtered_buffer;
     filtered_buffer.makeCopyOf(buffer);

     for (int i = 0; i < num_channels; i++) {
         Filter iir_filter;
         *iir_filter.coefficients = *coeffs;
         iir_filter.reset(); 

         float* data = filtered_buffer.getWritePointer(i);

         for (int j = 0; j < num_samples; j++) {
             float processed_sample = iir_filter.processSample(data[j]);
           /*  std::cout << processed_sample << std::endl;*/
             processed_sample = clip_value(processed_sample);
             data[j] = filter.snapToZero(processed_sample);
             data[j] = processed_sample;
         }
     }
     return filtered_buffer;
 }

/*FFT with low/high pass frequency domain
args: buffer (where audio is loaded)
      fft_order (determines the number of bins),
      cut off frequency
      sample rate
      lowpass (true or false) 
returns: 
      filtered audio in a buffer*/
 juce::AudioBuffer<float> Filters::fft(juce::AudioBuffer<float>& buffer, int fft_order, 
                                       float cut_off_freq, float sampling_rate, bool lowpass) {
    int fft_size = pow(2, fft_order);
    juce::dsp::FFT fft(fft_size);

    int num_channels = buffer.getNumChannels();
    int num_samples = buffer.getNumSamples();
   
    juce::AudioBuffer<float> filtered_buffer(num_channels, num_samples);
    filtered_buffer.clear();

    for (int i = 0; i < num_channels; i++) {
        std::vector<float> fft_data(fft_size, 0.0f);
        
        for (int block = 0; block < num_samples; block += fft_size) {
            for (int sample = 0; sample < fft_size; sample++) {
                int idx = block + sample;
                if (idx < num_samples) {
                    //apply hanning window (for smoothness) 
                    float w = 0.5f * (1.0f - cos(2.0f * M_PI * sample / (fft_size - 1)));
                    fft_data[sample] = buffer.getSample(i, idx) * w;
                } else {
                    fft_data[sample] = 0.0f;
                }
            }//end for sample

            fft.performFrequencyOnlyForwardTransform(fft_data.data());

            for (int k = 0; k < fft_size / 2; k++) {
                float freq_bin = k * sampling_rate / fft_size;
                
                if (lowpass) {
                    if (freq_bin > cut_off_freq) {
                        fft_data[k] = 0.0f;
                    }
                } else {
                    if (freq_bin <= cut_off_freq) {
                        fft_data[k] = 0.0f;
                    }
                }
            }//end for k

            fft.performRealOnlyInverseTransform(fft_data.data());

            // copy processed data back to buffer
            for (int j = 0; j < fft_size; j++) {
                int sample_idx = block + j;
                if (sample_idx < num_samples) {
                    filtered_buffer.setSample(i, sample_idx, fft_data[j]);
                }
            }//end for j
        } //end for block
    }//end for i (channels)
    
    return filtered_buffer;
}





/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "fifo.h"

//==============================================================================
/**
*/
class AudioPluginprojectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginprojectAudioProcessor();
    ~AudioPluginprojectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    enum class DSP_Option
    {
        Phase,
        Chorus,
        OverDrive,
        LadderFilter,
        End_Of_List
    };

    static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();

    juce::AudioProcessorValueTreeState apvts{ *this,nullptr,"Settings",CreateParameterLayout() };

    using DSP_Order = std::array < DSP_Option, static_cast<size_t>(DSP_Option::End_Of_List)>;

   Fifo<DSP_Order>dsporderfifo;

   /*
        Phaser:
            Rate : Hz
            Depth :  0 to 1
            feedback : -1 to +1
            Center Freq: Hz
            Mix : 0 to 1

   
   
   */

   juce::AudioParameterFloat* phaserRateHz = nullptr;
   juce::AudioParameterFloat* phaserCenterFreqHz = nullptr;
   juce::AudioParameterFloat* phaserFeedbackPercent = nullptr;
   juce::AudioParameterFloat* phaserDepthPercent = nullptr;
   juce::AudioParameterFloat* phaserMixPercent = nullptr;

   /*
      Chours:
          Rate : Hz
          Depth :  0 to 1
          feedback : -1 to +1
          Center delay: ms (1 to 100)
          Mix : 0 to 1

    */

   juce::AudioParameterFloat* chorusRateHz = nullptr;
   juce::AudioParameterFloat* chorusCenterDelayMs = nullptr;
   juce::AudioParameterFloat* chorusFeedbackPercent = nullptr;
   juce::AudioParameterFloat* chorusDepthPercent = nullptr;
   juce::AudioParameterFloat* chorusMixPercent = nullptr;

private:

    DSP_Order dspOrder;

    template<typename DSP>
    struct DSP_Choices :juce::dsp::ProcessorBase
    {
        void prepare(const juce::dsp::ProcessSpec& spec) override
        {
            dsp.prepare(spec);
        }

        void process(const juce::dsp::ProcessContextReplacing<float>& context) override
        {
            dsp.process(context);
        }

        void reset()override
        {
            dsp.reset();
        }

        DSP dsp;

    };

    DSP_Choices<juce::dsp::DelayLine<float>> delay;
    DSP_Choices<juce::dsp::Phaser<float>>pharser;
    DSP_Choices<juce::dsp::Chorus<float>>choruser;
    DSP_Choices<juce::dsp::LadderFilter<float>>overdrive, ladderfilter;

    using DSP_Pointers = std::array < juce::dsp::ProcessorBase*,
        static_cast<size_t>(DSP_Option::End_Of_List)>;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginprojectAudioProcessor)
};

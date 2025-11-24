/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

 // phaser : Parameter Name Funcs
auto getPhaserRateName() { return juce::String("Phaser RateHz"); }
auto getPhaserCenterFreqName() { return juce::String("Phaser Center FreqHz "); }
auto getPhaserFeedbackName() { return juce::String("Phaser Feedback %"); }
auto getPhaserDepthName() { return juce::String("Phaser Depth %"); }
auto getPhaserMixName() { return juce::String("Phaser Mix %"); }


// chorus : Parameter Name Funcs
auto getChorusRateName() { return juce::String("Chorus RateHz"); }
auto getChorusCenterDelayName() { return juce::String("Chorus Center Delay Ms "); }
auto getChorusFeedbackName() { return juce::String("Chorus Feedback %"); }
auto getChorusDepthName() { return juce::String("Chorus Depth %"); }
auto getChorusMixName() { return juce::String("Chorus Mix %"); }

auto getOverdriveSaturationName() { return juce::String("Overdrive Saturation"); }

auto getLadderfilterModeName() { return juce::String("Ladder filter Mode"); }
auto getLadderfilterCutoffName() { return juce::String("Ladder filter CutoffHz "); }
auto getLadderfilterResoneceName() { return juce::String("Ladder filter Resonece"); }
auto getLadderfilterDriveName() { return juce::String("Ladder filter Drive"); }


auto getLadderFilterChoices()
{
    return juce::StringArray
    {
        "LPF12",  // low-pass  12 dB/octave
        "HPF12",  // high-pass 12 dB/octave
        "BPF12",  // band-pass 12 dB/octave
        "LPF24",  // low-pass  24 dB/octave
        "HPF24",  // high-pass 24 dB/octave
        "BPF24"   // band-pass 24 dB/octave
    };
}

//==============================================================================
AudioPluginprojectAudioProcessor::AudioPluginprojectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

    auto floatParams = std::array
    {
        &phaserRateHz,
        &phaserCenterFreqHz,
        &phaserDepthPercent,
        &phaserFeedbackPercent,
        &phaserMixPercent,

        &chorusRateHz,
        &chorusCenterDelayMs,
        &chorusFeedbackPercent,
        &chorusDepthPercent ,
        &chorusMixPercent,

        &overdriveSaturation,

        &LadderFilterCutoffHz,
        &LadderFilterResonence,
        &LadderFilterDrive
    };   

    auto floatNameFuncs = std::array
    {

        &getPhaserRateName,
        &getPhaserCenterFreqName,
        &getPhaserDepthName,
        &getPhaserFeedbackName,
        &getPhaserMixName,

        &getChorusRateName,
        &getChorusCenterDelayName,
        &getChorusFeedbackName,
        &getChorusDepthName,
        &getChorusMixName,

        &getOverdriveSaturationName,

        &getLadderfilterCutoffName,
        &getLadderfilterResoneceName,
        &getLadderfilterDriveName
    };


    for (size_t i = 0; i < floatParams.size(); i++)
    {
        auto ptrToParamptr = floatParams[i];
        *ptrToParamptr = dynamic_cast<juce::AudioParameterFloat*>(
            apvts.getParameter(floatNameFuncs[i]()));

        jassert(*ptrToParamptr != nullptr);
    }

    LadderFilterMode = dynamic_cast<juce::AudioParameterChoice*>(
        apvts.getParameter(getLadderfilterModeName()));

     jassert(LadderFilterMode != nullptr);
}

AudioPluginprojectAudioProcessor::~AudioPluginprojectAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginprojectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginprojectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginprojectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginprojectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginprojectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginprojectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginprojectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginprojectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioPluginprojectAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPluginprojectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioPluginprojectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AudioPluginprojectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPluginprojectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginprojectAudioProcessor::CreateParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    const int VirsionHint = 1;
    /*
       Phaser:
           Rate : Hz
           Depth :  0 to 1
           feedback : -1 to +1
           Center Freq: Hz
           Mix : 0 to 1



  */

    
    // phaser rate hz
    auto name = getPhaserRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.01f, 2.f, 0.01f, 1.f)
        , 0.2f
        , "Hz"));

    // phaser depth 0 -> 1

     name = getPhaserDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f)
        , 0.05f
        , "%"));



    // phaser Center freq Hz
     name = getPhaserCenterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(20.f, 2000.f, 1.f, 1.f)
        , 1000.f
        , "Hz"));

    // phaser feedback -1 -> 1

     name = getPhaserFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f)
        , 0.0f
        , "%"));

    // phaser Mix 0 -> 1

     name = getPhaserMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f)
        , 0.05f
        , "%"));


    /*
    Chours:
        Rate : Hz
        Depth :  0 to 1
        feedback : -1 to +1
        Center delay: ms (1 to 100)
        Mix : 0 to 1



    */


    // chorus Rate : Hz

     name = getChorusRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.01f, 100.f, 0.01f, 1.f)
        , 0.2f
        , "Hz"));


    // chorus depth 0 -> 1

    name = getChorusDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f)
        , 0.05f
        , "%"));

    // chorus Center Delay miliseconds (1 to 100 )
    name = getChorusCenterDelayName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(1.f, 100.f, .1f, 1.f)
        , 7.f
        , "%"));

    // chorus feedback -1 -> 1

    name = getChorusFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f)
        , 0.0f
        , "%"));

    // chorus Mix 0 -> 1

    name = getChorusMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f)
        , 0.05f
        , "%"));


    /*
        overdrive:
        use the drive portion of the ladder filter class for now

        drive : 1 to 100
    
    
    */

    // drive : 1 to 100
    
    name = getOverdriveSaturationName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f)
        , 1.f
        , ""));

    /*
    Ladder filter:
        Mode : LadderFilter Mode enum(int)
        cutoff :  Hz
        resonece :0 to 1
        drive: 1 to 100
        
    */

    // Mode : LadderFilter Mode enum(int)

    name = getLadderfilterModeName();
    auto choices = getLadderFilterChoices();

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID {name,VirsionHint},name,choices,0));

    // Cutoff : Hz

    name = getLadderfilterCutoffName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(20.f, 2000.f, 0.1f, 1.f)
        , 2000.f
        , "Hz"));

    // Resonece 0 to 1

    name = getLadderfilterResoneceName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f)
        , 0.f
        , ""));

    // drive: 1 to 100

    name = getLadderfilterDriveName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f)
        , 1.f
        , ""));

    return layout;
 }

void AudioPluginprojectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    //TODO: add APVTs
    //TODO: create Audio parameters for each dsp choice
    //TODO: update dsp here from audio parameters
    //TODO: save \ load settings
    //TODO: save \ load dsp orders
    //TODO: drag to record GUI
    //TODO: GUI design for each dsp instence ?
    //TODO: metering 
    //TODO: perpare all DSP
    

    auto newDSPOrder =  DSP_Order();


    while (dsporderfifo.pull(newDSPOrder))
    {

    }

    if (newDSPOrder != DSP_Order())
    {
        dspOrder = newDSPOrder;
    }

    DSP_Pointers DspPointers;


    for (size_t i = 0; i < DspPointers.size(); ++i)
    {
        switch (dspOrder[i])
        {
            case DSP_Option::Phase:
                DspPointers[i] = &pharser;
                break;
            case DSP_Option::Chorus:
                DspPointers[i] = &choruser;
                break;
            case DSP_Option::OverDrive:
                DspPointers[i] = &overdrive;
                break;
            case DSP_Option::LadderFilter:
                DspPointers[i] = &ladderfilter;
                break;
            case DSP_Option::End_Of_List:
                jassertfalse;
                break;
        }
    }


    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    for (size_t i = 0; i < DspPointers.size(); ++i)
    {
        if (DspPointers[i] != nullptr)
        {
            DspPointers[i]->process(context);

        }
    }

}

//==============================================================================
bool AudioPluginprojectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginprojectAudioProcessor::createEditor()
{
    return new AudioPluginprojectAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginprojectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioPluginprojectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginprojectAudioProcessor();
}

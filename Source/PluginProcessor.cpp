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
auto getPhaserBypassName() { return juce::String("Phaser Bypass"); }


// chorus : Parameter Name Funcs
auto getChorusRateName() { return juce::String("Chorus RateHz"); }
auto getChorusCenterDelayName() { return juce::String("Chorus Center Delay Ms "); }
auto getChorusFeedbackName() { return juce::String("Chorus Feedback %"); }
auto getChorusDepthName() { return juce::String("Chorus Depth %"); }
auto getChorusMixName() { return juce::String("Chorus Mix %"); }
auto getChorusBypassName() { return juce::String("Chorus Bypass"); }

auto getOverdriveSaturationName() { return juce::String("Overdrive Saturation"); }
auto getOverdriveBypassName() { return juce::String("Overdrive Bypass"); }


auto getLadderfilterModeName() { return juce::String("Ladder filter Mode"); }
auto getLadderfilterCutoffName() { return juce::String("Ladder filter CutoffHz "); }
auto getLadderfilterResoneceName() { return juce::String("Ladder filter Resonece"); }
auto getLadderfilterDriveName() { return juce::String("Ladder filter Drive"); }
auto getLadderfilterBypassName() { return juce::String("Ladder filter Bypass"); }


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

auto getGenralFiltersChoices()
{
    return juce::StringArray
    {
        "peak",
        "bandpass",
        "notch",
        "allpass"
    };
}

auto getGeneralFilterModeName() { return juce::String("General Filter Mode "); }
auto getGeneralFilterFreqName() { return juce::String("General Filter FreqHz "); }
auto getGeneralFilterQualityName() { return juce::String("General Filter Quality"); }
auto getGeneralFilterGainName() { return juce::String("General Filter Gain"); }
auto getGeneralFilterBypassName() { return juce::String("General Filter Bypass"); }



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

    dspOrder =
    { {
        DSP_Option::Phase,
        DSP_Option::Chorus,
        DSP_Option::OverDrive,
        DSP_Option::LadderFilter
    } };

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
        &LadderFilterDrive,

        &GeneralFilterFreqHz,
        &GeneralFilterQuality,
        &GeneralFilterGain
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
        &getLadderfilterDriveName,

        &getGeneralFilterFreqName,
        &getGeneralFilterQualityName,
        &getGeneralFilterGainName
    };


    initialCachedPrarms<juce::AudioParameterFloat*>(floatParams, floatNameFuncs);

    auto choiceParams = std::array
    {
        &LadderFilterMode,
        &GeneralFilterMode
    };

    auto choiceNameFuncs = std::array
    {
        &getLadderfilterModeName,
        &getGeneralFilterModeName
    };

   

    initialCachedPrarms<juce::AudioParameterChoice*>(choiceParams, choiceNameFuncs);

    auto BypassParams = std::array
    {
        &phaserBypass,
        &chorusBypass,
        &overdriveBypass,
        &LadderFilterBypass,
        &GeneralFilterBypass

    };

    auto BypassNameFuncs = std::array
    {
        &getPhaserBypassName,
        &getChorusBypassName,
        &getOverdriveBypassName,
        &getLadderfilterBypassName,
        &getGeneralFilterBypassName
    };


    initialCachedPrarms<juce::AudioParameterBool*>(BypassParams, BypassNameFuncs);
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

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels =1;

    leftChannel.prepare(spec);
    rightChannel.prepare(spec);

   
}

void AudioPluginprojectAudioProcessor::MonoChannelDSP::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.numChannels == 1);
    std::vector<juce::dsp::ProcessorBase*>dsp
    {
        &pharser,
        &choruser,
        &overdrive,
        &ladderfilter,
        &genralfilter
    };

    for (auto p : dsp)
    {
        p->prepare(spec);
        p->reset();
    }
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

    // phaser Bypass

    name = getPhaserBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ name,VirsionHint },
        name, false));


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

    // chorus Bypass

    name = getChorusBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ name,VirsionHint },
        name, false));


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

    // overdrive Bypass

    name = getOverdriveBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ name,VirsionHint },
        name, false));

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

    // ladder filter Bypass

    name = getLadderfilterBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ name,VirsionHint },
        name, false));


    /*
        general filter: https://docs.juce.com/develop/structdsp_1_1IIR_1_1Coefficients.html
        Mode: Peak, bandpass, notch, allpass,
        freq: 20hz - 20,000hz in 1hz steps
        Q: 0.1 - 10 in 0.05 steps
        gain: -24db to +24db in 0.5db increments
    */

    //Mode: Peak, bandpass, notch, allpass
    name = getGeneralFilterModeName();
    choices = getGenralFiltersChoices();
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ name,VirsionHint }, name, choices, 0));

    //freq: 20hz - 20,000hz in 1hz steps
    name = getGeneralFilterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(20.f, 2000.f, 1.f, 1.f)
        , 750.f
        , "Hz"));

    // Q: 0.1 - 10 in 0.05 steps
    name = getGeneralFilterQualityName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f)
        , 1.f
        , ""));

    // gain: -24db to + 24db in 0.5db increments
    name = getGeneralFilterGainName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ name,VirsionHint }
        , name
        , juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f)
        , 0.f
        , "db"));

    // general filter Bypass

    name = getGeneralFilterBypassName();
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ name,VirsionHint },
        name, false));

    return layout;
 }

 void AudioPluginprojectAudioProcessor::MonoChannelDSP::updateDSPFromParams()
 {
     pharser.dsp.setRate(p.phaserRateHz->get());
     pharser.dsp.setCentreFrequency(p.phaserCenterFreqHz->get());
     pharser.dsp.setDepth(p.phaserDepthPercent->get());
     pharser.dsp.setFeedback(p.phaserFeedbackPercent->get());
     pharser.dsp.setMix(p.phaserMixPercent->get());

     choruser.dsp.setRate(p.chorusRateHz->get());
     choruser.dsp.setDepth(p.chorusDepthPercent->get());
     choruser.dsp.setCentreDelay(p.chorusCenterDelayMs->get());
     choruser.dsp.setFeedback(p.chorusFeedbackPercent->get());
     choruser.dsp.setMix(p.chorusMixPercent->get());

     overdrive.dsp.setDrive(p.overdriveSaturation->get());


     ladderfilter.dsp.setMode(static_cast<juce::dsp::LadderFilterMode>(p.LadderFilterMode->getIndex()));
     ladderfilter.dsp.setCutoffFrequencyHz(p.LadderFilterCutoffHz->get());
     ladderfilter.dsp.setResonance(p.LadderFilterResonence->get());
     ladderfilter.dsp.setDrive(p.LadderFilterDrive->get());
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


    
    
    leftChannel.updateDSPFromParams();
    rightChannel.updateDSPFromParams();

    auto newDSPOrder =  DSP_Order();


    while (dsporderfifo.pull(newDSPOrder))
    {
#if VERIFY_BYPASS_FUNCTIONALITY
        jassertfalse;

#endif
    }

    if (newDSPOrder != DSP_Order())
    {
        dspOrder = newDSPOrder;
    }

    auto block = juce::dsp::AudioBlock<float>(buffer);
    leftChannel.process(block.getSingleChannelBlock(0), dspOrder);
    rightChannel.process(block.getSingleChannelBlock(1), dspOrder);
  

}

void AudioPluginprojectAudioProcessor::MonoChannelDSP::process(juce::dsp::AudioBlock<float> block,
    const DSP_Order& dsp_order)
{
    DSP_Pointers DspPointers;
    DspPointers.fill({}); //this was previously that DspPointers.fill(nullptr);

    for (size_t i = 0; i < DspPointers.size(); ++i)
    {
        switch (p.dspOrder[i])
        {
        case DSP_Option::Phase:
            DspPointers[i].processor = &pharser;
            DspPointers[i].bypass = p.phaserBypass->get();
            break;
        case DSP_Option::Chorus:
            DspPointers[i].processor = &choruser;
            DspPointers[i].bypass = p.chorusBypass->get();
            break;
        case DSP_Option::OverDrive:
            DspPointers[i].processor = &overdrive;
            DspPointers[i].bypass = p.overdriveBypass->get();
            break;
        case DSP_Option::LadderFilter:
            DspPointers[i].processor = &ladderfilter;
            DspPointers[i].bypass = p.LadderFilterBypass->get();
            break;
        case DSP_Option::GenralFilter:
            DspPointers[i].processor = &genralfilter;
            DspPointers[i].bypass = p.GeneralFilterBypass->get();
        case DSP_Option::End_Of_List:
            jassertfalse;
            break;
        }
    }


    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    for (size_t i = 0; i < DspPointers.size(); ++i)
    {
        if (DspPointers[i].processor != nullptr)
        {
            juce::ScopedValueSetter<bool>svs(context.isBypassed, DspPointers[i].bypass);
#if VERIFY_BYPASS_FUNCTIONALITY
            if (context.isBypassed)
            {
                jassertfalse;
            }
            if (DspPointers[i].processor == &genralfilter)
            {
                continue;
            }


#endif



            DspPointers[i].bypass = p.phaserBypass->get();
            DspPointers[i].processor->process(context);
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
   //return new AudioPluginprojectAudioProcessorEditor (*this);

    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================

static AudioPluginprojectAudioProcessor::DSP_Order fromVar(const juce::var&v)
{
    using T = AudioPluginprojectAudioProcessor::DSP_Order;
    T dspOrder;

    jassert(v.isBinaryData());

    if (v.isBinaryData() == false)
    {
        dspOrder.fill(AudioPluginprojectAudioProcessor::DSP_Option::End_Of_List);
    }
    else
    {
        auto mb = *v.getBinaryData();
        juce::MemoryInputStream mis(mb, false);
        std::vector<int>arr;
        while (!mis.isExhausted())
        {
            arr.push_back(mis.readInt());
        }

        jassert(arr.size() == dspOrder.size());

        for (size_t i = 0; i < dspOrder.size(); ++i)
        {
            dspOrder[i] = static_cast<AudioPluginprojectAudioProcessor::DSP_Option>(arr[i]);
        }
        return dspOrder;
    }
}

static juce::var ToVar( const AudioPluginprojectAudioProcessor::DSP_Order& t)
{
    juce::MemoryBlock mb;

    {
        juce::MemoryOutputStream mos(mb, false);

        for (auto v : t)
        {
            mos.writeInt(static_cast<int>(v));
        }
    };

     return juce::var(mb);
    
}

void AudioPluginprojectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    
    juce::MemoryBlock mb;
    juce::MemoryOutputStream mos(mb, false);

    for (auto v : dspOrder)
        mos.writeInt(static_cast<int>(v));

 
    apvts.state.setProperty("dspOrder", juce::var(mb), nullptr);

    juce::MemoryOutputStream destStream(destData, false);
    apvts.state.writeToStream(destStream);
}


void AudioPluginprojectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid())
    {
        apvts.replaceState(tree);

        if (apvts.state.hasProperty("dspOrder"))
        {
         
            juce::var v = apvts.state.getProperty("dspOrder");

            if (v.isBinaryData())
            {
                auto mb = *v.getBinaryData();
                juce::MemoryInputStream mis(mb, false);

                AudioPluginprojectAudioProcessor::DSP_Order order;

                for (size_t i = 0; i < order.size() && !mis.isExhausted(); i++)
                {
                    int x = mis.readInt();
                    order[i] = static_cast<AudioPluginprojectAudioProcessor::DSP_Option>(x);
                }

                dsporderfifo.push(order);
            }
        }

        DBG(apvts.state.toXmlString());

    #if VERIFY_BYPASS_FUNCTIONALITY
        juce::Timer::callAfterDelay(1000, [this]()
        {
                DSP_Order order;
                order.fill(DSP_Option::LadderFilter);
                order[0] = DSP_Option::Chorus;

                chorusBypass->setValueNotifyingHost(1.f);
                dsporderfifo.push(order);

        });

    #endif
    }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginprojectAudioProcessor();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginprojectAudioProcessorEditor::AudioPluginprojectAudioProcessorEditor (AudioPluginprojectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    dspOrderButton.onClick = [this]()
        {
            juce::Random r;
            AudioPluginprojectAudioProcessor::DSP_Order dspOrder;

            auto range = juce::Range<int>
                (static_cast<int>(AudioPluginprojectAudioProcessor::DSP_Option::Phase),
                    static_cast<int>(AudioPluginprojectAudioProcessor::DSP_Option::End_Of_List));

            for (auto& v : dspOrder)
            {
                auto entry = r.nextInt(range);
                v = static_cast<AudioPluginprojectAudioProcessor::DSP_Option>(entry);
            }
            DBG(juce::Base64::toBase64(dspOrder.data(), dspOrder.size()));
            jassertfalse;

            audioProcessor.dsporderfifo.push(dspOrder);
        };


    addAndMakeVisible(dspOrderButton);
    setSize (400, 300);
}

AudioPluginprojectAudioProcessorEditor::~AudioPluginprojectAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginprojectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginprojectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    dspOrderButton.setBounds(getLocalBounds().reduced(100));
}

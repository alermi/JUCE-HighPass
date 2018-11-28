/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HighPassAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    HighPassAudioProcessorEditor (HighPassAudioProcessor&);
    ~HighPassAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	Slider frequencySlider;
	Slider qualitySlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> frequencySliderAttachment;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> qualitySliderAttachment;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HighPassAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HighPassAudioProcessorEditor)
};

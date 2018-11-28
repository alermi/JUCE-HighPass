/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HighPassAudioProcessorEditor::HighPassAudioProcessorEditor (HighPassAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (290, 270);

	frequencySlider.setRange(0, 20000);
	frequencySlider.setSliderStyle(Slider::LinearVertical);
	frequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
	frequencySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Frequency", frequencySlider);
	addAndMakeVisible(&frequencySlider);

	qualitySlider.setRange(0, 4);
	qualitySlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	qualitySlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
	addAndMakeVisible(&qualitySlider);
	qualitySliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Quality", qualitySlider);
}

HighPassAudioProcessorEditor::~HighPassAudioProcessorEditor()
{
	frequencySliderAttachment = nullptr;
}

//==============================================================================
void HighPassAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::blanchedalmond);
	g.drawRect(6, 6, 278, 258);
	g.setFont(20.0);
	g.setColour(Colours::aqua);
	g.drawText(TRANS("Frequency"), 30, 216, 100, 20, Justification::centred); // left, top, width, height
	g.drawText(TRANS("Quality"), 160, 216, 100, 20, Justification::centred); // left, top, width, heigh
	Font titleFont(35, 1);
	g.setColour(Colours::floralwhite);
	g.setFont(titleFont);
	g.drawText(processor.getName(), 95, 25, 170, 20, Justification::centredRight);
	Font nameFont(15, 2);
	g.setColour(Colours::floralwhite);
	g.setFont(nameFont);
	String manufacturer("by: ");
	manufacturer += JucePlugin_Manufacturer;
	g.drawText(manufacturer, 90, 60, 170, 20, Justification::centredRight);

	
}

void HighPassAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	frequencySlider.setBounds(30, 46, 100, 170); // left, top, width, height
	qualitySlider.setBounds(160, 96, 100, 120);
}

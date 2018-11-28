/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HighPassAudioProcessor::HighPassAudioProcessor()
	: parameters(*this, nullptr)
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	NormalisableRange<float> frequencyRange = NormalisableRange<float>(0, 20000);
	frequencyRange.setSkewForCentre(900);

	parameters.createAndAddParameter(std::make_unique<AudioParameterFloat>("Frequency",       // parameter ID
																			"Frequency",       // parameter name
																			frequencyRange,    // range
																			0.0f,         // default value
																			"Hz"));

	parameters.createAndAddParameter(std::make_unique<AudioParameterFloat>("Quality",       // parameter ID
																			"Quality",       // parameter name
																			NormalisableRange<float>(0, 4),    // range
																			0.0f,         // default value
																			""));


	parameters.state = ValueTree(Identifier("HighPassVT"));

	frequency = 0.0;
}

HighPassAudioProcessor::~HighPassAudioProcessor()
{
	

}

//==============================================================================
const String HighPassAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HighPassAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HighPassAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HighPassAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HighPassAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HighPassAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HighPassAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HighPassAudioProcessor::setCurrentProgram (int index)
{
}

const String HighPassAudioProcessor::getProgramName (int index)
{
    return {};
}

void HighPassAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HighPassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	twopi = 8.0f * atan(1.0f);
	quality = 0;
	pole1 = 0;
	pole2 = 0;
	pole3 = 0;
	pole4 = 0;
	outputs = (float*)calloc(getTotalNumInputChannels(),sizeof(float));
	pole1 = (float*)calloc(getTotalNumInputChannels(), sizeof(float));
	pole2 = (float*)calloc(getTotalNumInputChannels(), sizeof(float));
	pole3 = (float*)calloc(getTotalNumInputChannels(), sizeof(float));
	pole4 = (float*)calloc(getTotalNumInputChannels(), sizeof(float));
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void HighPassAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	if (outputs) {
		free(outputs);
		outputs = nullptr;
	}
	if (pole1) {
		free(pole1);
		pole1 = nullptr;
	}
	if (pole2) {
		free(pole2);
		pole2 = nullptr;
	}
	if (pole3) {
		free(pole3);
		pole3 = nullptr;
	}
	if (pole4) {
		free(pole4);
		pole4 = nullptr;
	}
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HighPassAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void HighPassAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	int sample;
    ScopedNoDenormals noDenormals;
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	frequency = *parameters.getRawParameterValue("Frequency");
	float c=(float)exp((-1.0)*twopi*frequency/getSampleRate());
	float input;
	quality = *parameters.getRawParameterValue("Quality");
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		auto* outData = buffer.getWritePointer(channel);
		auto* inData = buffer.getReadPointer(channel);
		for (sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			input = *(inData + sample) - pole4[channel] * quality;

			if (input > 1.0f) input = 1.0f;
			if (input < -1.0f) input = -1.0f;
			input = 1.5f * input - 0.5f * (input * input * input);

			pole1[channel] = input * (1.0f - c) + pole1[channel] * c ;
			pole2[channel] = pole1[channel] * (1.0f - c) + pole2[channel] * c;
			pole3[channel] = pole2[channel] * (1.0f - c) + pole3[channel] * c;
			pole4[channel] = pole3[channel] * (1.0f - c) + pole4[channel] * c;
			
			//The subtraction causes the filter to do a highpass
			outputs[channel] = input - pole4[channel];

			
			*(outData + sample) = outputs[channel];
			
			
			

		}
        // ..do something to the data...
    }
}

//==============================================================================
bool HighPassAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HighPassAudioProcessor::createEditor()
{
    return new HighPassAudioProcessorEditor (*this);
}

//==============================================================================
void HighPassAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HighPassAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HighPassAudioProcessor();
}

/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "ProcessorEditor.h"

//==============================================================================
VelocityZoneAudioProcessor::VelocityZoneAudioProcessor()
//:    rawValueTone   (*parameters.getRawParameterValue OJDParameters::Sliders::Tone::id)),
{
    // defaults to MIDI Note 51 / D#2 == Roland TD Ride Bow Tip
    //  addParameter(noteInNumber = new juce::AudioParameterInt("noteIn", "Note # In",
    //                                                          0, 255, 51));
    // defaults to 127, determined by experiment
    //  addParameter(noteVelocityThreshold = new juce::AudioParameterInt(
    //                   "noteVelocityThreshold", "Velocity Threshold", 0, 127, 70));
    // defaults to MIDI Note 53 / F2 == Roland TD Ride Bell
    //  addParameter(noteOutNumber = new juce::AudioParameterInt(
    //                   "noteOut", "Note # Out", 0, 255, 53));
    //    // setup always constant elements in the chain
    //    chain.get<preWaveshaperGain>().setGainLinear (11.0f);
    //
    //    // If the drive or hp/lp mode changes, some allocating biquad coefficient recalculation is done. The parameter
    //    // listener callback is used for this
    //    parameters.addParameterListener (OJDParameters::Sliders::Drive::id, this);
    //    parameters.addParameterListener (OJDParameters::Switches::HpLp::id, this);
    //
    // Add a subtree where the editor stores some states
    parameters.state.appendChild (ProcessorEditor::createUIStateSubtree(), nullptr);
    
    //    // Try to reach the schrammel server to find out if there is e.g. an update message to display
    //    checkForMessageOfTheDay();
}

//VelocityZoneAudioProcessor::~VelocityZoneAudioProcessor() {}

//==============================================================================
const juce::String VelocityZoneAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool VelocityZoneAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool VelocityZoneAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool VelocityZoneAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double VelocityZoneAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int VelocityZoneAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are 0
    // programs,
    // so this should be at least 1, even if you're not really implementing
    // programs.
}

int VelocityZoneAudioProcessor::getCurrentProgram() { return 0; }

void VelocityZoneAudioProcessor::setCurrentProgram(int index) {}

const juce::String VelocityZoneAudioProcessor::getProgramName(int index) {
    return {};
}

void VelocityZoneAudioProcessor::changeProgramName(
                                                   int index, const juce::String &newName) {}

//==============================================================================
void VelocityZoneAudioProcessor::prepareToPlay(double sampleRate,
                                               int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VelocityZoneAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VelocityZoneAudioProcessor::isBusesLayoutSupported(
                                                        const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void VelocityZoneAudioProcessor::processBlock(juce::dsp::AudioBlock<float> &buffer,
                                              juce::MidiBuffer &midiBuffer) {
    // buffer for modified messages
    juce::MidiBuffer processedMidi;
    
    // the audio buffer in a midi effect will have zero channels!
    //jassert(buffer.getNumChannels() == 0);
    
    for (const auto metadata : midiBuffer) {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;
        
        // copy each midi note on event
        if (message.isNoteOn()) {
            //      if (message.getNoteNumber() == *noteInNumber) {
            //        if (message.getVelocity() > *noteVelocityThreshold) {
            //          // velocity zone trigged, change note val
            //          message = juce::MidiMessage::noteOn(
            //              message.getChannel(), *noteOutNumber, message.getVelocity());
            //        }
            //      }
            
            // to new buffer
            processedMidi.addEvent(message, time);
        }
    }
    // swap original buffer with new one
    midiBuffer.swapWith(processedMidi);
}

//==============================================================================
bool VelocityZoneAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

//==============================================================================
void VelocityZoneAudioProcessor::getStateInformation(
                                                     juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VelocityZoneAudioProcessor::setStateInformation(const void *data,
                                                     int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
}

void VelocityZoneAudioProcessor::prepareResources (bool sampleRateChanged, bool maxBlockSizeChanged, bool numChannelsChanged)
{
    
    // Computing the chains latency. The oversampling in the waveshaper might introduce fractional sample delay
    //const auto waveshaperLatency = chain.get<waveshaper>().getLatencyInSamples();
    //setLatencySamples (static_cast<int> (waveshaperLatency));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new VelocityZoneAudioProcessor();
}

juce::AudioProcessorEditor *VelocityZoneAudioProcessor::createEditor() {
    //  return new juce::GenericAudioProcessorEditor(*this);
    return new ProcessorEditor (*this);
}


void VelocityZoneAudioProcessor::parameterChanged (const juce::String& parameterID, float)
{
    //jassert (parameterID == OJDParameters::Sliders::Drive::id || parameterID == OJDParameters::Switches::HpLp::id);
    //juce::ignoreUnused (parameterID);
    
}

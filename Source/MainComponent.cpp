#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    freq=440;
    phase =0;
    waveTableSize=1024;
    increment=freq*waveTableSize/sampleRate;
    amp=0.25;
    
    //calc one sinewave cycle
    for (int i=0; i<waveTableSize; i++) {
        sineWaveTable.insert(i, sin(2.0*juce::double_Pi*i/waveTableSize));
    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    float* const leftSpeaker=bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* const rightSpeaker=bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int sample=0; sample<bufferToFill.numSamples; sample++) {
        leftSpeaker[sample]=sineWaveTable[(int)phase]*amp;
        rightSpeaker[sample]=sineWaveTable[(int)phase]*amp;
        phase=fmod(phase+increment,waveTableSize);

        }
}
void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

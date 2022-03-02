/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include <nlohmann/json.hpp>


//==============================================================================
MainComponent::MainComponent()
{
    loadSettings();
    
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    saveSettings();
}

void MainComponent::saveSettings()
{
    
    var libSettings{ playlistComponent.getSettings() };
    var appSettings{ playlistComponent.getSettings() };

    //This object created with new dies when this function returns, safe as it is not passed anywhere else
    var s{ new juce::DynamicObject{} };
    s.getDynamicObject()->setProperty("library", libSettings);
    s.getDynamicObject()->setProperty("app", appSettings);

    String settingsString = JSON::toString(s);

    DBG(settingsString);
    
    //File settingsFile{ File::getCurrentWorkingDirectory().getChildFile(settingsFile).getFullPathName()};
    FileOutputStream outputStream{settingsFile};
    if (outputStream.openedOk())
    {
        outputStream.setPosition(0);
        outputStream.truncate();
        outputStream << settingsString;
    }

}

void MainComponent::loadSettings()
{
    //File settingsFile{ File::getCurrentWorkingDirectory().getChildFile(settingsFile).getFullPathName() };
    FileInputStream inputStream{ settingsFile };

    if (inputStream.openedOk())
    {
        var input{ JSON::parse(inputStream) };
        if (input == var()) //bad parse
        {
            DBG("Bad settings JSON loaded");
            return;
            //bad parse
        }
        
        //check if property exists
        if (var librarySettings = input.getProperty("library", false))
        {
            DBG(JSON::toString(librarySettings));
            playlistComponent.setSettings(librarySettings);
        }
 
    }

}



//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    double compWidth = getWidth();
    double compHeight = getHeight();

    
    deckGUI1.setBounds(0, 0, compWidth/2, compHeight/2);
    deckGUI2.setBounds(compWidth/2, 0, compWidth/2, compHeight/2);

    playlistComponent.setBounds(0, compHeight/2, compWidth, compHeight/2);


}


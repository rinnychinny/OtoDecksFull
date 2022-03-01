/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(rotatingDeck);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    rotatingDeck.addChangeListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.1, 5.0);
    posSlider.setRange(0.0, 1.0);

    //Set default values
    volSlider.setValue(0.1);
    speedSlider.setValue(1.0);
    posSlider.setValue(0.0);

    //Sets interval in milliseconds between calls to TimerCallback
    startTimer(10);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 10.0; 
    double rowW = getWidth();

    playButton.setBounds(0, 0, rowW, rowH);
    stopButton.setBounds(0, rowH, rowW, rowH);  
    volSlider.setBounds(0, rowH * 2, rowW, rowH);
    speedSlider.setBounds(0, rowH * 3, rowW, rowH);
    posSlider.setBounds(0, rowH * 4, rowW, rowH);
    waveformDisplay.setBounds(0, rowH * 5, rowW, rowH * 2);
    loadButton.setBounds(0, rowH * 7, rowW, rowH);
    
    rotatingDeck.setBounds(0, rowH * 8, rowW / 4, rowH*2);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()});
            
        }
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        //Test to stop a potential loop between this section and timeCallBack posSlider.setValue
        if (slider->getValue() != lastPosRelative)
        {
            player->setPositionRelative(slider->getValue());
        }
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    lastPosRelative = player->getPositionRelative();
    
    waveformDisplay.setPositionRelative(lastPosRelative);
    rotatingDeck.setPosInSeconds(player->getPositionInSecs());
    posSlider.setValue(lastPosRelative);
}

void DeckGUI::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &rotatingDeck)
    {
        //double posInSecs = rotatingDeck.getPosInSeconds();
        
        //Need to debit this value to show that picked up
        //double changePosInSecs = rotatingDeck.fetchChangeRequest();
        //player->changePositionInSecs(changePosInSecs);
        
        player->setPosition(rotatingDeck.getPosInSeconds());
        
        DBG("Change listener called!!");
    }
}



    


/*
  ==============================================================================

    RotatingDeck.cpp
    Created: 14 Feb 2022 2:27:10pm
    Author:  richp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotatingDeck.h"

//==============================================================================
RotatingDeck::RotatingDeck()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //sets the frequency with which update and repaint will be called
    setFramesPerSecond(framesPerSec);

}

RotatingDeck::~RotatingDeck()
{
}

void RotatingDeck::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Component::getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (Component::getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("RotatingDeck", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text

    double w = getWidth();
    double h = getHeight();

    g.setColour(juce::Colours::orange);
    g.fillEllipse(0, 0, w, h);
    g.setColour(juce::Colours::darkslategrey);
    
    juce::Point<float> origin{ (float) w / 2, (float) h / 2 };


    double xpos = w/2 + sin(this->posInRadians) * w/2;
    double ypos = h/2 - cos(this->posInRadians) * h/2;

    juce::Point<float> loc{ (float) xpos, (float) ypos };
    juce::Line<float> vec{ origin, loc };

    g.drawLine(vec, 2);

}

void RotatingDeck::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void RotatingDeck::update()
{
        

}

double RotatingDeck::secondsToRadians(double secs)
{
    return secs * rpm / 60.0 * 2.0 * 3.14;
}


//Called externally from parent component timer loop
void RotatingDeck::setPosInSeconds(double secs)
{
    this->posInSeconds = secs;
    this->posInRadians = secondsToRadians(secs);
}

/*
void RotatingDeck::changePosFromRadians(double changeRadians)
{
    this->posInRadians += changeRadians;
    this->posInSeconds += changeRadians / secondsToRadians(1.0);
}
*/

double RotatingDeck::currentAngle(const MouseEvent& event)
{
    double w = getWidth();
    double h = getHeight();
    juce::Point<int> origin{ (int)w / 2, (int)h / 2 };
    juce::Point<int> thisPos{ event.getPosition() };
    juce::Line<int> thisVec{ origin, thisPos };
    return thisVec.getAngle();
}

void RotatingDeck::mouseDown(const MouseEvent& event)
{
    lastEvent.firstTime = event.eventTime;
    lastEvent.lastTime = event.eventTime;
    lastEvent.Angle = currentAngle(event);
}

void RotatingDeck::mouseDrag(const MouseEvent& event)
{
    double angleCurrent = currentAngle(event);
    double angleChange = angleCurrent - lastEvent.Angle;
    
    DBG("angle change!" << angleChange);
    
    //changeRequestInSecs += angleChange / secondsToRadians(1.0);

    double tMillisecs = (double) (event.eventTime.toMilliseconds() - lastEvent.lastTime.toMilliseconds());
    DBG("Millisecs:" << tMillisecs);

    posInSeconds += (angleChange / secondsToRadians(1.0) - tMillisecs / 1000.0);
    //changePosFromRadians(angleChange);
    
    lastEvent.lastTime= event.eventTime;
    lastEvent.Angle = angleCurrent;


    //repaint();

    //Inform listeners there has been a change in status for the playhead due to this angle change
    sendChangeMessage();

}

/*
double RotatingDeck::fetchChangeRequest()
{
    double req = changeRequestInSecs;
    changeRequestInSecs = 0;
    return req;

}
*/






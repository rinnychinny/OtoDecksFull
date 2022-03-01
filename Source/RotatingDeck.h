/*
  ==============================================================================

    RotatingDeck.h
    Created: 14 Feb 2022 2:27:10pm
    Author:  richp

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotatingDeck  : public juce::AnimatedAppComponent, public juce::ChangeBroadcaster
{
public:
    RotatingDeck();
    ~RotatingDeck() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    //This override is called periodically at the set frames per second
    void update() override;

    //*this function used to set the position in seconds in the track - will affect the drawing of the spinning disc on update/repaint*/
    void setPosInSeconds(double secs);

    /**returns pos in seconds, accessed externally*/
    double getPosInSeconds() { return posInSeconds; };


    //* override from MouseListener virtual function, used to update playhead with disk drag
    void mouseDrag(const MouseEvent& event) override;

    //* used to intialise the angle for disk dragging operation
    void mouseDown(const MouseEvent& event) override;

    //double fetchChangeRequest();
    


private:

    //double changeRequestInSecs{ 0 };


    /**helper function to convert a duration in seconds to a number of radians using rpm*/
    double secondsToRadians(double secs);

    /**helper function to change the position in seconds from a change in radians*/
    //void changePosFromRadians(double changeRadians);

    //frame rate for the animation
    int framesPerSec{ 60 };
    
    //How many times the deck rotates per minute when playing
    double rpm{ 10.0 };
    
    //local variables recording the position of the playhead in secs and radians
    double posInSeconds{ 0.0 };
    double posInRadians{ 0.0 };

    /**data(time, angle) from last mouse event(used in drag update)*/
    struct LastEvent { juce::Time firstTime; juce::Time lastTime; double Angle; };
    LastEvent lastEvent;

    //* helper function to get current angular position of mouse click/drag*/
    double currentAngle(const MouseEvent& event);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotatingDeck)
};

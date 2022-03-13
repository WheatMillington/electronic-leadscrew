// Clough42 Electronic Leadscrew
// https://github.com/clough42/electronic-leadscrew
//
// MIT License
//
// Copyright (c) 2019 James Clough
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef __CORE_H
#define __CORE_H

#include "StepperDrive.h"
#include "Encoder.h"
#include "ControlPanel.h"
#include "Tables.h"

class Core
{
private:
    Encoder *encoder;
    StepperDrive *stepperDrive;

    int32 incrementCount;
    int32 carriageCurrentCount;
    int32 carriagePreviousCount;
    int32 carriagePosition;

    int32 leftStop;
    int32 rightStop;

    int16 jogSpeed;
    float jogFactor;

#ifdef USE_FLOATING_POINT
    float feed;
    float previousFeed;
#else
    const FEED_THREAD *feed;
    const FEED_THREAD *previousFeed;
#endif // USE_FLOATING_POINT

    int16 feedDirection;
    int16 previousFeedDirection;

    Uint32 previousSpindlePosition;

    int32 feedRatio(Uint32 count);

    bool powerOn;
    bool Enabled;

public:
    Core( Encoder *encoder, StepperDrive *stepperDrive );

    void setFeed(const FEED_THREAD *feed);
    void setReverse(bool reverse);
    Uint16 getRPM(void);
    int32 getCarriagePosition(const FEED_THREAD *feed);
    Uint16 getSPosition(void);
    bool isAlarm();

    bool isPowerOn();
    void setPowerOn(bool);
    void setEnabled(bool);

    void ISR( void );

    void zeroCarriagePosition();

    void setLeftStop(bool);
    void setRightStop(bool);

    void feedLeft(void);
    void feedRight(void);

    bool leftStopActive;
    bool rightStopActive;
    bool feedingLeft;
    bool feedingRight;

    bool jog1Active;
    bool jog2Active;
    bool jog3Active;

    bool joggingLeft;
    bool joggingRight;

    void jogLeft(bool);
    void jogRight(bool);

    void setJogSpeed(int16);
};

inline void Core :: setFeed(const FEED_THREAD *feed)
{
#ifdef USE_FLOATING_POINT
        this->feed = (float)feed->numerator / feed->denominator;
#else
    this->feed = feed;
#endif // USE_FLOATING_POINT
}

inline Uint16 Core :: getRPM(void)
{
    return encoder->getRPM();
}

inline Uint16 Core :: getSPosition(void)
{
    return encoder->getSPosition();
}

inline int32 Core :: getCarriagePosition(const FEED_THREAD *feed)
{
    if ( isPowerOn() ) {
        carriageCurrentCount = encoder->getCount();
        incrementCount = (carriageCurrentCount - carriagePreviousCount) * feedDirection;
        int32 feedrate = (float)feed->numerator / (float)STEPPER_RESOLUTION;
        carriagePosition += (incrementCount * feedrate * 10000) / ENCODER_RESOLUTION;
        carriagePreviousCount = carriageCurrentCount;

    } else {  // if power is off, just keep track of count, don't increment
        carriageCurrentCount = encoder->getCount();
        carriagePreviousCount = carriageCurrentCount;
    }

    return carriagePosition / 10000;
}

inline void Core :: setLeftStop(bool leftStopActive)
{
    leftStop = this->carriagePosition;
    this->leftStopActive = leftStopActive;
}

inline void Core :: setRightStop(bool rightStopActive)
{
    rightStop = this->carriagePosition;
    this->rightStopActive = rightStopActive;
}

inline void Core :: feedLeft()
{
    if (this->carriagePosition <= leftStop && leftStopActive == true) {
        setPowerOn(true);
        feedingLeft = true;
    }
}

inline void Core :: feedRight()
{
    if (this->carriagePosition >= this->rightStop && rightStopActive == true) {
        setPowerOn(true);
        feedingRight = true;
    }
}

inline void Core :: zeroCarriagePosition(void)
{
    this->carriageCurrentCount = 0;
    this->carriagePreviousCount = 0;
    this->incrementCount = 0;
    this->carriagePosition = 0;
    encoder->zeroCount();
}

inline void Core :: setJogSpeed(int16 jogSpeed)
{
    this->jogSpeed = jogSpeed;
    if ( this->jogSpeed == 1 ) {
        jogFactor = JOG_SPEED_1;
        jog1Active = true;
        jog2Active = false;
        jog3Active = false;
    } else {
        if ( this->jogSpeed == 2 ) {
            jogFactor = JOG_SPEED_2;
            jog1Active = false;
            jog2Active = true;
            jog3Active = false;
        } else {
            jogFactor = JOG_SPEED_3;
            jog1Active = false;
            jog2Active = false;
            jog3Active = true;
        }
    }
}

inline void Core :: jogLeft(bool joggingLeft)
{
    this->joggingLeft = joggingLeft;
    setPowerOn(joggingLeft);
}

inline void Core :: jogRight(bool joggingRight)
{
    this->joggingRight = joggingRight;
    setPowerOn(joggingRight);
}

inline bool Core :: isAlarm()
{
    return this->stepperDrive->isAlarm();
}

inline bool Core :: isPowerOn()
{
    return this->powerOn;
}

inline int32 Core :: feedRatio(Uint32 count)
{
#ifdef USE_FLOATING_POINT
    if (joggingLeft || joggingRight) {
        return ((float)count) * this->feed * feedDirection * jogFactor;
    } else {
        return ((float)count) * this->feed * feedDirection;
    }
#else // USE_FLOATING_POINT
    return ((long long)count) * feed->numerator / feed->denominator * feedDirection;
#endif // USE_FLOATING_POINT
}

inline void Core :: ISR( void )
{
    if( this->feed != NULL ) {
        if ( (feedingLeft &&  (leftStop <= carriagePosition)) || (feedingRight && (rightStop >= carriagePosition)) ) {
            setPowerOn(false);
            feedingLeft = false;
            feedingRight = false;
        }

        // read the encoder
        Uint32 spindlePosition = encoder->getPosition();

        // calculate the desired stepper position
        int32 desiredSteps = feedRatio(spindlePosition);
        stepperDrive->setDesiredPosition(desiredSteps);

        // compensate for encoder overflow/underflow
        if( spindlePosition < previousSpindlePosition && previousSpindlePosition - spindlePosition > encoder->getMaxCount()/2 ) {
            stepperDrive->incrementCurrentPosition(-1 * feedRatio(encoder->getMaxCount()));
        }
        if( spindlePosition > previousSpindlePosition && spindlePosition - previousSpindlePosition > encoder->getMaxCount()/2 ) {
            stepperDrive->incrementCurrentPosition(feedRatio(encoder->getMaxCount()));
        }

        // if the feed or direction changed, reset sync to avoid a big step
        if( feed != previousFeed || feedDirection != previousFeedDirection) {
            stepperDrive->setCurrentPosition(desiredSteps);
        }

        // remember values for next time
        previousSpindlePosition = spindlePosition;
        previousFeedDirection = feedDirection;
        previousFeed = feed;

        // service the stepper drive state machine
        stepperDrive->ISR();
    }
}


#endif // __CORE_H

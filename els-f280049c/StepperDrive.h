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


#ifndef __STEPPERDRIVE_H
#define __STEPPERDRIVE_H

#include "F28x_Project.h"
#include "Configuration.h"


#define STEP_PIN GPIO0
#define DIRECTION_PIN GPIO1
#define ENABLE_PIN GPIO6
#define ALARM_PIN GPIO7

#define GPIO_SET(pin) GpioDataRegs.GPASET.bit.pin = 1
#define GPIO_CLEAR(pin) GpioDataRegs.GPACLEAR.bit.pin = 1
#define GPIO_GET(pin) GpioDataRegs.GPADAT.bit.pin

#ifdef INVERT_STEP_PIN
#define GPIO_SET_STEP GPIO_CLEAR(STEP_PIN)
#define GPIO_CLEAR_STEP GPIO_SET(STEP_PIN)
#else
#define GPIO_SET_STEP GPIO_SET(STEP_PIN)
#define GPIO_CLEAR_STEP GPIO_CLEAR(STEP_PIN)
#endif

#ifdef INVERT_DIRECTION_PIN
#define GPIO_SET_DIRECTION GPIO_CLEAR(DIRECTION_PIN)
#define GPIO_CLEAR_DIRECTION GPIO_SET(DIRECTION_PIN)
#else
#define GPIO_SET_DIRECTION GPIO_SET(DIRECTION_PIN)
#define GPIO_CLEAR_DIRECTION GPIO_CLEAR(DIRECTION_PIN)
#endif

#ifdef INVERT_ENABLE_PIN
#define GPIO_SET_ENABLE GPIO_CLEAR(ENABLE_PIN)
#define GPIO_CLEAR_ENABLE GPIO_SET(ENABLE_PIN)
#else
#define GPIO_SET_ENABLE GPIO_SET(ENABLE_PIN)
#define GPIO_CLEAR_ENABLE GPIO_CLEAR(ENABLE_PIN)
#endif

#ifdef INVERT_ALARM_PIN
#define GPIO_GET_ALARM (GPIO_GET(ALARM_PIN) == 0)
#else
#define GPIO_GET_ALARM (GPIO_GET(ALARM_PIN) != 0)
#endif


class StepperDrive
{
private:

    //
    // Desired position of the motor, in steps
    //
    int32 desiredPosition;

    //
    // current state-machine state
    // bit 0 - step signal
    // bit 1 - direction signal
    //
    Uint16 state;

    //
    // Is the drive enabled?
    //
    bool enabled;
    bool powerOn;

public:
    //
    // Current position of the motor, in steps
    //
    int32 currentPosition;

    int32 getStepperPosition(void);

    StepperDrive();
    void initHardware(void);

    void setDesiredPosition(int32 steps);
    void incrementCurrentPosition(int32 increment);
    void setCurrentPosition(int32 position);

    bool checkStepBacklog();

    void setEnabled(bool);
    void setPowerOn(bool);

    bool isAlarm();

    void ISR(void);

};

inline void StepperDrive :: setDesiredPosition(int32 steps)
{
    this->desiredPosition = steps;
}

inline void StepperDrive :: incrementCurrentPosition(int32 increment)
{
    this->currentPosition += increment;
}

inline void StepperDrive :: setCurrentPosition(int32 position)
{
    this->currentPosition = position;
}

inline bool StepperDrive :: checkStepBacklog()
{
    if( abs(this->desiredPosition - this->currentPosition) > MAX_BUFFERED_STEPS ) {
        setEnabled(false);
        return true;
    }
    return false;
}

inline void StepperDrive :: setEnabled(bool enabled)
{
    this->enabled = enabled;
    if( this->enabled ) {
        GPIO_SET_ENABLE;
    }
    else
    {
        GPIO_CLEAR_ENABLE;
    }
}

inline void StepperDrive :: setPowerOn(bool powerOn)
{
    this->powerOn = powerOn;
}

inline bool StepperDrive :: isAlarm()
{
#ifdef USE_ALARM_PIN
    return GPIO_GET_ALARM;
#else
    return false;
#endif
}


inline void StepperDrive :: ISR(void)
{
    if(enabled && powerOn) {

        switch( this->state ) {

        case 0:
            // Step = 0; Dir = 0
            if( this->desiredPosition < this->currentPosition ) {
                GPIO_SET_STEP;
                this->state = 2;
            }
            else if( this->desiredPosition > this->currentPosition ) {
                GPIO_SET_DIRECTION;
                this->state = 1;
            }
            break;

        case 1:
            // Step = 0; Dir = 1
            if( this->desiredPosition > this->currentPosition ) {
                GPIO_SET_STEP;
                this->state = 3;
            }
            else if( this->desiredPosition < this->currentPosition ) {
                GPIO_CLEAR_DIRECTION;
                this->state = 0;
            }
            break;

        case 2:
            // Step = 1; Dir = 0
            GPIO_CLEAR_STEP;
            this->currentPosition--;
            this->state = 0;
            break;

        case 3:
            // Step = 1; Dir = 1
            GPIO_CLEAR_STEP;
            this->currentPosition++;
            this->state = 1;
            break;
        }

    } else {
        // not enabled; just keep current position in sync
        this->currentPosition = this->desiredPosition;
    }

}

#endif // __STEPPERDRIVE_H

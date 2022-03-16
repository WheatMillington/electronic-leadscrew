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


#include "UserInterface.h"

const MESSAGE STARTUP_MESSAGE_2 =
{
  .message = { LETTER_E, LETTER_L, LETTER_S, DASH, ONE | POINT, FOUR | POINT, ZERO, ZERO },
  .displayTime = UI_REFRESH_RATE_HZ * .5
};

const MESSAGE STARTUP_MESSAGE_1 =
{
 .message = { LETTER_C, LETTER_L, LETTER_O, LETTER_U, LETTER_G, LETTER_H, FOUR, TWO },
 .displayTime = UI_REFRESH_RATE_HZ * .5,
 .next = &STARTUP_MESSAGE_2
};

const MESSAGE SETTINGS_MESSAGE_RPM =
{
 .message = { BLANK, BLANK, LETTER_R, LETTER_P, LETTER_M, BLANK, BLANK, BLANK },
 .displayTime = UI_REFRESH_RATE_HZ * .25
};

const MESSAGE SETTINGS_MESSAGE_POSITION =
{
 .message = { LETTER_P, LETTER_O, LETTER_S, LETTER_I, LETTER_T, LETTER_I, LETTER_O, LETTER_N},
 .displayTime = UI_REFRESH_RATE_HZ * .25
};

extern const MESSAGE BACKLOG_PANIC_MESSAGE_2;
const MESSAGE BACKLOG_PANIC_MESSAGE_1 =
{
 .message = { LETTER_T, LETTER_O, LETTER_O, BLANK, LETTER_F, LETTER_A, LETTER_S, LETTER_T },
 .displayTime = UI_REFRESH_RATE_HZ * .5,
 .next = &BACKLOG_PANIC_MESSAGE_2
};
const MESSAGE BACKLOG_PANIC_MESSAGE_2 =
{
 .message = { BLANK, LETTER_R, LETTER_E, LETTER_S, LETTER_E, LETTER_T, BLANK, BLANK },
 .displayTime = UI_REFRESH_RATE_HZ * .5,
 .next = &BACKLOG_PANIC_MESSAGE_1
};

};

const MESSAGE FEEDRIGHT =
{
 .message = { LETTER_F, LETTER_E, LETTER_E, LETTER_D, BLANK, LETTER_R, BLANK, BLANK},
 .displayTime = UI_REFRESH_RATE_HZ * .25
};

const MESSAGE FEEDLEFT =
{
 .message = { LETTER_F, LETTER_E, LETTER_E, LETTER_D, BLANK, LETTER_L, BLANK, BLANK},
 .displayTime = UI_REFRESH_RATE_HZ * .25
};

const MESSAGE RIGHT_STOP =
{
 .message = { LETTER_R, BLANK, LETTER_S, LETTER_T, LETTER_O, LETTER_P, BLANK, BLANK},
 .displayTime = UI_REFRESH_RATE_HZ * .25
};

const MESSAGE LEFT_STOP =
{
 .message = { LETTER_L, BLANK, LETTER_S, LETTER_T, LETTER_O, LETTER_P, BLANK, BLANK},
 .displayTime = UI_REFRESH_RATE_HZ * .25
};

UserInterface :: UserInterface(ControlPanel *controlPanel, Core *core, FeedTableFactory *feedTableFactory)
{
    this->controlPanel = controlPanel;
    this->core = core;
    this->feedTableFactory = feedTableFactory;

    this->metric = true; // start out with metric
    this->thread = false; // start out with feeds
    this->reverse = false; // start out going forward
    this->sposition = false; // start out showing RPM
    core->setPowerOn(false); // start out with power off
    core->setEnabled(true);  // keep motor enabled to eliminate start-up lag

    this->feedTable = NULL;

    this->keys.all = 0xff;

    // initialize the core so we start up correctly
    core->setReverse(this->reverse);
    core->setFeed(loadFeedTable());
    this->setJogSpeed(2);  // select the jog speed LED
    core->setJogFactor(1);  // set the jog factor to 1, being the standard (non-jog) number

    setMessage(&STARTUP_MESSAGE_1);
}

const FEED_THREAD *UserInterface::loadFeedTable()
{
    this->feedTable = this->feedTableFactory->getFeedTable(this->metric, this->thread);
    return this->feedTable->current();
}

LED_REG UserInterface::calculateLEDs()
{
    // get the LEDs for this feed
    LED_REG leds = feedTable->current()->leds;

    if( this->core->isPowerOn() )
    {
        // and add a few of our own
        leds.bit.POWER = 1;
        leds.bit.REVERSE = this->reverse;
        leds.bit.FORWARD = ! this->reverse;
    }
  
    leds.bit.LSTOP = core->leftStopActive;
    leds.bit.RSTOP = core->rightStopActive;
    leds.bit.FEEDINGLEFT = core->feedingLeft;
    leds.bit.FEEDINGRIGHT = core->feedingRight;
    leds.bit.JOGGING = leftJogToggle || rightJogToggle;
    leds.bit.JOG1 = jog1Active;
    leds.bit.JOG2 = jog2Active;
    leds.bit.JOG3 = jog3Active;

    return leds;
}

void UserInterface :: setMessage(const MESSAGE *message)
{
    this->message = message;
    this->messageTime = message->displayTime;
}

void UserInterface :: overrideMessage( void )
{
    if( this->message != NULL )
    {
        if( this->messageTime > 0 ) {
            this->messageTime--;
            controlPanel->setMessage(this->message->message);
        }
        else {
            this->message = this->message->next;
            if( this->message == NULL )
                controlPanel->setMessage(NULL);
            else
                this->messageTime = this->message->displayTime;
        }
    }
}

void UserInterface :: clearMessage( void )
{
    this->message = NULL;
    this->messageTime = 0;
    controlPanel->setMessage(NULL);
}

void UserInterface :: panicStepBacklog( void )
{
    setMessage(&BACKLOG_PANIC_MESSAGE_1);
}

void UserInterface :: setJogSpeed(Uint16 jogSpeed)
{
    this->jogSpeed = jogSpeed;
    jog1Active = false;
    jog2Active = false;
    jog3Active = false;

    switch ( this->jogSpeed) {
        case 1:
            this->jog1Active = true;
            jogFactor = JOG_SPEED_1;
        break;

        case 2:
            this->jog2Active = true;
            jogFactor = JOG_SPEED_2;
        break;

        case 3:
            this->jog3Active = true;
            jogFactor = JOG_SPEED_3;
        break;
    }
}

void UserInterface :: loop( void )
{
    // read the RPM up front so we can use it to make decisions
    Uint16 currentRpm = core->getRPM();

    // read the current spindle position to keep this up to date
    Uint16 currentSPosition = core->getSPosition();
  
    // read the current carriage position to keep this up to date
    int32 carriagePosition = core->getCarriagePosition(loadFeedTable());

    // display an override message, if there is one
    overrideMessage();

    // read keypresses from the control panel
    keys = controlPanel->getKeys();

    // respond to keypresses

    // Only respond when power is OFF
    if( !core->isPowerOn() ) {
      
      // Feeding - ie feed until L/R stops are reached
      if (keys.bit.FEEDLEFT) {
        setMessage(&FEEDLEFT);
        core->feedLeft();
        }
      
      if (keys.bit.FEEDRIGHT) {
        setMessage(&FEEDRIGHT);
        core->setJogFactor(-1);
        core->feedRight();
        }
      
      // Jogging - jog while key is held
      if(keys.bit.JOGLEFT || keys.bit.JOGRIGHT) {
        holdKey = true;
        } else {
        holdKey = false;
        }

      if (keys.bit.JOGLEFT) {
        core->setPowerOn(true);
        core->setJogFactor(this->jogFactor);
        this->leftJogToggle = true;
      } else {
        if (leftJogToggle == true) {
            core->setJogFactor(1);
            leftJogToggle = false;
            core->setPowerOn(false);
          }
        }

        if (keys.bit.JOGRIGHT) {
          core->setPowerOn(true);
          core->setJogFactor(-this->jogFactor);
          this->rightJogToggle = true;
        } else {
          if (rightJogToggle == true) {
            core->setJogFactor(1);
            rightJogToggle = false;
            core->setPowerOn(false);
            }
          }
      
          if (keys.bit.JOGSPEED) {
            if (jog1Active ) {
              setJogSpeed(2);
            } else if (jog2Active) {
              setJogSpeed(3);
            } else {
              setJogSpeed(1);
            }
          }
    }

    // respond regardless of machine state
  
    if( keys.bit.POWER )
    {
      if ( !core->isPowerOn() ) {
        this->core->setEnabled(true);  // If turning power on with the key, enable (ie reset) stepper - resets panic stop
      }
      this->core->setPowerOn(!this->core->isPowerOn());
    }

    if( keys.bit.UP )
    {
        core->setFeed(feedTable->next());
    }
  
    if( keys.bit.DOWN )
    {
        core->setFeed(feedTable->previous());
    }
  
    if( keys.bit.SET ) {  // Switch between showing RPM and Spindle Position
        this->sposition =! this->sposition;
        if ( sposition ) {
            setMessage(&SETTINGS_MESSAGE_POSITION);
        } else {
            setMessage(&SETTINGS_MESSAGE_RPM);
        }
    }

    if( keys.bit.ZERODRO ) {
        core->zeroCarriagePosition();
    }

    if (keys.bit.RIGHTSTOP) {
        core->setRightStop(!core->rightStopActive);
        if ( core->rightStopActive ) {
            setMessage(&RIGHT_STOP);
        }
    }

    if (keys.bit.LEFTSTOP) {
        core->setLeftStop(!core->leftStopActive);
        if ( core->leftStopActive ) {
            setMessage(&LEFT_STOP);
        }
    }

    if( currentRpm == 0 )
    {
        // these keys should only be sensitive when the machine is stopped
        if( keys.bit.IN_MM )
        {
            this->metric = ! this->metric;
            core->setFeed(loadFeedTable());
        }
        if( keys.bit.FEED_THREAD )
        {
            this->thread = ! this->thread;
            core->setFeed(loadFeedTable());
        }
        if( keys.bit.FWD_REV )
        {
            this->reverse = ! this->reverse;
            core->setReverse(this->reverse);
        }

    }

// update the control panel
    controlPanel->setLEDs(calculateLEDs());
    controlPanel->setValue(feedTable->current()->display);
    controlPanel->setHoldKey(this->holdKey);

    if ( sposition ) {
        controlPanel->setSPosition(currentSPosition);
    } else {
        controlPanel->setRPM(currentRpm);
    }

    controlPanel->setCarriagePosition(carriagePosition);

    controlPanel->refresh(sposition);
}

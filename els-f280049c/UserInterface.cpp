#include "UserInterface.h"


UserInterface :: UserInterface(ControlPanel *controlPanel, Core *core, FeedTableFactory *feedTableFactory)
{
    this->controlPanel = controlPanel;
    this->core = core;
    this->feedTableFactory = feedTableFactory;

    this->metric = false; // start out with imperial
    this->thread = false; // start out with feeds
    this->reverse = false; // start out going forward

    this->feedTable = NULL;

    this->keys.all = 0xff;
}

void UserInterface :: init( void )
{

}

const FEED_THREAD *UserInterface::loadFeedTable()
{
    this->feedTable = this->feedTableFactory->getFeedTable(this->metric, this->thread);
    return this->feedTable->current();
}

LED_REG UserInterface::calculateLEDs(const FEED_THREAD *selectedFeed)
{
    // get the LEDs for this feed
    LED_REG leds = selectedFeed->leds;

    // and add a few of our own
    leds.bit.POWER = 1;
    leds.bit.REVERSE = this->reverse;
    leds.bit.FORWARD = ! this->reverse;

    return leds;
}

void UserInterface :: loop( void )
{
    const FEED_THREAD *newFeed = NULL;

    // just in case, initialize the first time through
    if( feedTable == NULL ) {
        newFeed = loadFeedTable();
    }

    // read keypresses from the control panel
    keys = controlPanel->getKeys();

    // respond to keypresses
    if( keys.bit.IN_MM )
    {
        this->metric = ! this->metric;
        newFeed = loadFeedTable();
    }
    if( keys.bit.FEED_THREAD )
    {
        this->thread = ! this->thread;
        newFeed = loadFeedTable();
    }
    if( keys.bit.FWD_REV )
    {
        this->reverse = ! this->reverse;
        // feed table hasn't changed, but we need to trigger an update
        newFeed = loadFeedTable();
    }
    if( keys.bit.UP )
    {
        newFeed = feedTable->next();
    }
    if( keys.bit.DOWN )
    {
        newFeed = feedTable->previous();
    }

    // if we have changed the feed
    if( newFeed != NULL ) {
        // update the display
        LED_REG leds = this->calculateLEDs(newFeed);
        controlPanel->setLEDs(leds);
        controlPanel->setValue(newFeed->display);

        // update the core
        core->setFeed(newFeed);
        core->setReverse(this->reverse);
    }

    // update the RPM display
    controlPanel->setRPM(core->getRPM());

    // write data out to the display
    controlPanel->refresh();
}

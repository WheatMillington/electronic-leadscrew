
#include "Tables.h"


//
// INCH THREAD DEFINITIONS
//
// Each row in the table defines a standard imperial thread, with the display data,
// LED indicator states and gear ratio fraction to use.
//
#define TPI_NUMERATOR(tpi) ((Uint32)12*200*8)
#define TPI_DENOMINATOR(tpi) ((Uint32)tpi*4096)
#define TPI_FRACTION(tpi) .numerator = TPI_NUMERATOR(tpi), .denominator = TPI_DENOMINATOR(tpi)

const FEED_THREAD inch_thread_table[] =
{
 { .display = {BLANK, BLANK, BLANK, EIGHT}, .leds = LED_THREAD | LED_TPI, TPI_FRACTION(8) },
 { .display = {BLANK, BLANK, BLANK, NINE},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(9) },
 { .display = {BLANK, BLANK, ONE,   ZERO},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(10) },
 { .display = {BLANK, BLANK, ONE,   ONE},   .leds = LED_THREAD | LED_TPI, TPI_FRACTION(11) },
 { .display = {BLANK, BLANK, ONE,   TWO},   .leds = LED_THREAD | LED_TPI, TPI_FRACTION(12) },
 { .display = {BLANK, BLANK, ONE,   THREE}, .leds = LED_THREAD | LED_TPI, TPI_FRACTION(13) },
 { .display = {BLANK, BLANK, ONE,   FOUR},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(14) },
 { .display = {BLANK, BLANK, ONE,   SIX},   .leds = LED_THREAD | LED_TPI, TPI_FRACTION(16) },
 { .display = {BLANK, BLANK, ONE,   EIGHT}, .leds = LED_THREAD | LED_TPI, TPI_FRACTION(18) },
 { .display = {BLANK, BLANK, TWO,   ZERO},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(20) },
 { .display = {BLANK, BLANK, TWO,   FOUR},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(24) },
 { .display = {BLANK, BLANK, TWO,   EIGHT}, .leds = LED_THREAD | LED_TPI, TPI_FRACTION(28) },
 { .display = {BLANK, BLANK, THREE, TWO},   .leds = LED_THREAD | LED_TPI, TPI_FRACTION(32) },
 { .display = {BLANK, BLANK, THREE, SIX},   .leds = LED_THREAD | LED_TPI, TPI_FRACTION(36) },
 { .display = {BLANK, BLANK, FOUR,  ZERO},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(40) },
 { .display = {BLANK, BLANK, FOUR,  FOUR},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(44) },
 { .display = {BLANK, BLANK, FOUR,  EIGHT}, .leds = LED_THREAD | LED_TPI, TPI_FRACTION(48) },
 { .display = {BLANK, BLANK, FIVE,  SIX},   .leds = LED_THREAD | LED_TPI, TPI_FRACTION(56) },
 { .display = {BLANK, BLANK, SIX,   FOUR},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(64) },
 { .display = {BLANK, BLANK, SEVEN, TWO},   .leds = LED_THREAD | LED_TPI, TPI_FRACTION(72) },
 { .display = {BLANK, BLANK, EIGHT, ZERO},  .leds = LED_THREAD | LED_TPI, TPI_FRACTION(80) },
};



//
// INCH FEED DEFINITIONS
//
// Each row in the table defines a standard imperial feed rate, with the display data,
// LED indicator states and gear ratio fraction to use.
//

#define THOU_IN_NUMERATOR(thou) ((Uint32)thou*12*200*8)
#define THOU_IN_DENOMINATOR(thou) ((Uint32)4096*1000)
#define THOU_IN_FRACTION(thou) .numerator = THOU_IN_NUMERATOR(thou), .denominator = THOU_IN_DENOMINATOR(thou)

const FEED_THREAD inch_feed_table[] =
{
 { .display = {POINT, ZERO, ZERO,  ONE},    .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(1) },
 { .display = {POINT, ZERO, ZERO,  TWO},    .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(2) },
 { .display = {POINT, ZERO, ZERO,  THREE},  .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(3) },
 { .display = {POINT, ZERO, ZERO,  FOUR},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(4) },
 { .display = {POINT, ZERO, ZERO,  FIVE},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(5) },
 { .display = {POINT, ZERO, ZERO,  SIX},    .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(6) },
 { .display = {POINT, ZERO, ZERO,  SEVEN},  .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(7) },
 { .display = {POINT, ZERO, ZERO,  EIGHT},  .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(8) },
 { .display = {POINT, ZERO, ZERO,  NINE},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(9) },
 { .display = {POINT, ZERO, ONE,   ZERO},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(10) },
 { .display = {POINT, ZERO, ONE,   ONE},    .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(11) },
 { .display = {POINT, ZERO, ONE,   TWO},    .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(12) },
 { .display = {POINT, ZERO, ONE,   THREE},  .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(13) },
 { .display = {POINT, ZERO, ONE,   FIVE},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(15) },
 { .display = {POINT, ZERO, ONE,   SEVEN},  .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(17) },
 { .display = {POINT, ZERO, TWO,   ZERO},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(20) },
 { .display = {POINT, ZERO, TWO,   THREE},  .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(23) },
 { .display = {POINT, ZERO, TWO,   SIX},    .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(26) },
 { .display = {POINT, ZERO, THREE, ZERO},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(30) },
 { .display = {POINT, ZERO, THREE, FIVE},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(35) },
 { .display = {POINT, ZERO, FOUR,  ZERO},   .leds = LED_FEED | LED_INCH, THOU_IN_FRACTION(40) },
};




//
// METRIC THREAD DEFINITIONS
//
// Each row in the table defines a standard metric thread, with the display data,
// LED indicator states and gear ratio fraction to use.
//
#define HMM_NUMERATOR(hmm) ((Uint32)hmm*10*12*200*8)
#define HMM_DENOMINATOR(hmm) ((Uint32)4096*254*100)
#define HMM_FRACTION(hmm) .numerator = HMM_NUMERATOR(hmm), .denominator = HMM_DENOMINATOR(hmm)

const FEED_THREAD metric_thread_table[] =
{
 { .display = {BLANK, POINT,         TWO,   BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(20) },
 { .display = {BLANK, POINT,         TWO,   FIVE},  .leds = LED_THREAD | LED_MM, HMM_FRACTION(25) },
 { .display = {BLANK, POINT,         THREE, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(30) },
 { .display = {BLANK, POINT,         THREE, FIVE},  .leds = LED_THREAD | LED_MM, HMM_FRACTION(35) },
 { .display = {BLANK, POINT,         FOUR,  BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(40) },
 { .display = {BLANK, POINT,         FOUR,  FIVE},  .leds = LED_THREAD | LED_MM, HMM_FRACTION(45) },
 { .display = {BLANK, POINT,         FIVE,  BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(50) },
 { .display = {BLANK, POINT,         SIX,   BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(60) },
 { .display = {BLANK, POINT,         SEVEN, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(70) },
 { .display = {BLANK, POINT,         SEVEN, FIVE},  .leds = LED_THREAD | LED_MM, HMM_FRACTION(75) },
 { .display = {BLANK, POINT,         EIGHT, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(80) },
 { .display = {BLANK, ONE,           BLANK, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(100) },
 { .display = {BLANK, ONE | POINT,   TWO,   FIVE},  .leds = LED_THREAD | LED_MM, HMM_FRACTION(125) },
 { .display = {BLANK, ONE | POINT,   FIVE,  BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(150) },
 { .display = {BLANK, ONE | POINT,   SEVEN, FIVE},  .leds = LED_THREAD | LED_MM, HMM_FRACTION(175) },
 { .display = {BLANK, TWO,           BLANK, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(200) },
 { .display = {BLANK, TWO | POINT,   FIVE,  BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(250) },
 { .display = {BLANK, THREE,         BLANK, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(300) },
 { .display = {BLANK, THREE | POINT, FIVE,  BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(350) },
 { .display = {BLANK, FOUR,          BLANK, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(400) },
 { .display = {BLANK, FOUR | POINT,  FIVE,  BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(450) },
 { .display = {BLANK, FIVE,          BLANK, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(500) },
 { .display = {BLANK, FIVE | POINT,  FIVE,  BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(550) },
 { .display = {BLANK, SIX,           BLANK, BLANK}, .leds = LED_THREAD | LED_MM, HMM_FRACTION(600) },
};



//
// METRIC FEED DEFINITIONS
//
// Each row in the table defines a standard metric feed, with the display data,
// LED indicator states and gear ratio fraction to use.
//
const FEED_THREAD metric_feed_table[] =
{
 { .display = {BLANK, POINT,       ZERO,  TWO},   .leds = LED_FEED | LED_MM, HMM_FRACTION(2) },
 { .display = {BLANK, POINT,       ZERO,  FIVE},  .leds = LED_FEED | LED_MM, HMM_FRACTION(5) },
 { .display = {BLANK, POINT,       ZERO,  SEVEN}, .leds = LED_FEED | LED_MM, HMM_FRACTION(7) },
 { .display = {BLANK, POINT,       ONE,   ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(10) },
 { .display = {BLANK, POINT,       ONE,   TWO},   .leds = LED_FEED | LED_MM, HMM_FRACTION(12) },
 { .display = {BLANK, POINT,       ONE,   FIVE},  .leds = LED_FEED | LED_MM, HMM_FRACTION(15) },
 { .display = {BLANK, POINT,       ONE,   SEVEN}, .leds = LED_FEED | LED_MM, HMM_FRACTION(17) },
 { .display = {BLANK, POINT,       TWO,   ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(20) },
 { .display = {BLANK, POINT,       TWO,   TWO},   .leds = LED_FEED | LED_MM, HMM_FRACTION(22) },
 { .display = {BLANK, POINT,       TWO,   FIVE},  .leds = LED_FEED | LED_MM, HMM_FRACTION(25) },
 { .display = {BLANK, POINT,       TWO,   SEVEN}, .leds = LED_FEED | LED_MM, HMM_FRACTION(27) },
 { .display = {BLANK, POINT,       THREE, ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(30) },
 { .display = {BLANK, POINT,       THREE, FIVE},  .leds = LED_FEED | LED_MM, HMM_FRACTION(35) },
 { .display = {BLANK, POINT,       FOUR,  ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(40) },
 { .display = {BLANK, POINT,       FOUR,  FIVE},  .leds = LED_FEED | LED_MM, HMM_FRACTION(45) },
 { .display = {BLANK, POINT,       FIVE,  ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(50) },
 { .display = {BLANK, POINT,       FIVE,  FIVE},  .leds = LED_FEED | LED_MM, HMM_FRACTION(55) },
 { .display = {BLANK, POINT,       SIX,   ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(60) },
 { .display = {BLANK, POINT,       SEVEN, ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(70) },
 { .display = {BLANK, POINT,       EIGHT, FIVE},  .leds = LED_FEED | LED_MM, HMM_FRACTION(85) },
 { .display = {BLANK, ONE | POINT, ZERO,  ZERO},  .leds = LED_FEED | LED_MM, HMM_FRACTION(100) },
};





FeedTable::FeedTable(const FEED_THREAD *table, Uint16 numRows)
{
    this->table = table;
    this->numRows = numRows;
    this->selectedRow = 0;
}

FeedTableFactory::FeedTableFactory(void):
        inchThreads(inch_thread_table, sizeof(inch_thread_table)/sizeof(inch_thread_table[0])),
        inchFeeds(inch_feed_table, sizeof(inch_feed_table)/sizeof(inch_feed_table[0])),
        metricThreads(metric_thread_table, sizeof(metric_thread_table)/sizeof(metric_thread_table[0])),
        metricFeeds(metric_feed_table, sizeof(metric_feed_table)/sizeof(metric_feed_table[0]))
{

}

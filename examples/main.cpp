#include "Gsdc_SSD1306.h"  

const char* oleh_message = "Demo Complete";

Gsdc_SSD1306 _display(0x3c, SDA, SCL);

void setup() 
{
  Wire.begin();
  Serial.begin(250000);
  Serial.println();
  _display.begin();
 
}

void loop() 
{ 
    CALL_AND_WAIT_500_MILLIS(_display.center(MIDDLE, "5"));
    CALL_AND_WAIT_500_MILLIS(_display.center(MIDDLE, "4"));
    CALL_AND_WAIT_500_MILLIS(_display.center(MIDDLE, "3"));
    CALL_AND_WAIT_500_MILLIS(_display.center(MIDDLE, "2"));
    CALL_AND_WAIT_500_MILLIS(_display.center(MIDDLE, "1"));
    _display.clear();

    CALL_AND_WAIT_500_MILLIS(_display.heading("HEADING"));
    CALL_AND_WAIT_500_MILLIS(_display.scrubHeading());
    CALL_AND_WAIT_500_MILLIS(_display.flashHeading("Flash head"));
    CALL_AND_WAIT_500_MILLIS(_display.scrubHeading());
    CALL_AND_WAIT_500_MILLIS(_display.scrollLeftHeading("Scroll left head"));
    CALL_AND_WAIT_500_MILLIS(_display.scrubHeading());
    CALL_AND_WAIT_500_MILLIS(_display.centerHeading("Center head"));
    CALL_AND_WAIT_500_MILLIS(_display.scrubHeading());
    CALL_AND_WAIT_500_MILLIS(_display.leftHeading("Left head"));
    CALL_AND_WAIT_500_MILLIS(_display.scrubHeading());
    CALL_AND_WAIT_500_MILLIS(_display.rightHeading("Right head"));
    
    _display.important("Important msg");
    _display.scrubLine(MIDDLE);

    CALL_AND_WAIT_500_MILLIS(_display.leftHeading("DISPLAY ..."));
    CALL_AND_WAIT_500_MILLIS(_display.center(MIDDLE, "Falling text"));
    CALL_AND_WAIT_500_MILLIS(_display.fall(MIDDLE, "Falling text"));
    CALL_AND_WAIT_500_MILLIS(_display.left(MIDDLE, "left"));
    CALL_AND_WAIT_500_MILLIS(_display.center(BOTTOM, "center"));
    CALL_AND_WAIT_500_MILLIS(_display.right(MIDDLE, "right"));
    _display.scrubInfo();

    _display.clearLine(TOP);
    CALL_AND_WAIT_500_MILLIS(_display.leftHeading("FLASH ..."));
    CALL_AND_WAIT_500_MILLIS(_display.flashRight(BOTTOM, "right"));
    CALL_AND_WAIT_500_MILLIS(_display.flashCenter(MIDDLE, "center"));
    CALL_AND_WAIT_500_MILLIS(_display.flashLeft(BOTTOM, "left"));
    _display.scrubInfo();

    _display.clearLine(TOP);
    CALL_AND_WAIT_500_MILLIS(_display.leftHeading("SCROLL ..."));
    CALL_AND_WAIT_500_MILLIS(_display.scroll(MIDDLE, "...left"));
    CALL_AND_WAIT_500_MILLIS(_display.scrollToCenter(BOTTOM, "...center"));
    CALL_AND_WAIT_500_MILLIS(_display.scroll(MIDDLE, "...left again"));
    CALL_AND_WAIT_500_MILLIS(_display.scrollRight(MIDDLE, "...right"));
    CALL_AND_WAIT_500_MILLIS(_display.scrubLine(BOTTOM));
    _display.scroll(BOTTOM, "Long messages scroll all the way off of the screen");
    
    _display.important(oleh_message);
    _display.scrub();

    delay(3000);
}


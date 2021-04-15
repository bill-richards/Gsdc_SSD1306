#include "Gsdc_SSD1306.h"  

const char* init_message = "IMPORTANT";
const char* scroll_middle = "scroll middle";
const char* scroll_bottom = "scroll bottom";
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
    _display.flash(line_positions::TOP, "FLASH TOP");
    _display.show(line_positions::TOP, "SHOW TOP");
    _display.flash(line_positions::MIDDLE, "flash middle");
    _display.show(line_positions::MIDDLE, "show middle");
    _display.flash(line_positions::BOTTOM, "flash bottom");
    _display.show(line_positions::BOTTOM, "show bottom");
    _display.scrub();

    _display.flash(line_positions::MIDDLE, init_message);
    _display.scrubLine(line_positions::MIDDLE);

    _display.show(line_positions::TOP, "HEADLINE");
    _display.scroll(line_positions::MIDDLE, scroll_middle);
    _display.scroll(line_positions::BOTTOM, scroll_bottom);
    _display.scrubInfo();

    _display.scroll(line_positions::MIDDLE, "A long message to scroll");
    _display.scrubLine(line_positions::TOP);
    
    _display.flash(line_positions::TOP, oleh_message);
    _display.fall(line_positions::TOP, oleh_message);
    _display.scrub();

    while(_display.IsProcessingMessages()) { vTaskDelay(33); }
    vTaskDelay(15000);
}

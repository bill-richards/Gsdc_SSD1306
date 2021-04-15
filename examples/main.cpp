#include "Gsdc_SSD1306.h"  

const char* mac_address = "MAC: fe:00:c4:af:94:e3";
const char* ip_address = "IP : 192.168.4.106";
const char* helo_message = "NVM LOADED";
const char* oleh_message = "Setup Complete";
const char* init_message = "INITIALIZING";

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
    _display.flash(line_positions::TOP, init_message);
    _display.rise(line_positions::TOP, init_message);
    _display.rise(line_positions::BOTTOM, helo_message);
    _display.flash(line_positions::TOP, helo_message);
    _display.scroll(line_positions::MIDDLE, ip_address);
    _display.scroll(line_positions::BOTTOM, mac_address);
    _display.flash(line_positions::MIDDLE, oleh_message);
    _display.fall(line_positions::MIDDLE, oleh_message);
    vTaskDelay(1000);
}

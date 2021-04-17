#include <Arduino.h>
#include <SSD1306Wire.h>

#ifndef _Gsdc_Structs_h_
#define _Gsdc_Structs_h_

/* *********************************************************************************************** 
 * ENUMS
 *********************************************************************************************** */
typedef enum { NO_POSITION = -1, TOP = 0, MIDDLE, BOTTOM, } line_positions;
typedef enum 
{ 
  NO_PROPERTY,
  CENTER,
  CLEAR, 
  CLEAR_LINE, 
  SCRUB, 
  SCRUB_INFO, 
  SCRUB_LINE, 
  FALL, 
  FLASH_CENTER, 
  FLASH_LEFT, 
  FLASH_RIGHT, 
  LEFT,
  RIGHT, 
  RISE, 
  SCROLL, 
  SCROLL_RIGHT,
  SCROLL_TO_CENTER, 
  WAIT 
} display_properties;

/* *********************************************************************************************** 
 * STRUCTS
 *********************************************************************************************** */
struct Message_Info
{
  ulong period = 0;
  line_positions position = NO_POSITION;
  display_properties properties = NO_PROPERTY;
  char message[64] = { };
};

struct task_data
{
public:
  QueueHandle_t messageQueue;
  SSD1306Wire * display;
  bool running = false;

  task_data(SSD1306Wire *display_, QueueHandle_t messageQueue_)
  {
    display = display_;
    messageQueue = messageQueue_;
  }
  void set_running() { running = true; }
  void set_stopped() { running = false; }
};

#endif
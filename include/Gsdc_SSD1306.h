#ifndef Gsdc_SSD1306_h_
#define Gsdc_SSD1306_h_

#include <Arduino.h>
#include <SSD1306Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

typedef enum { TOP, MIDDLE, BOTTOM } line_positions;
typedef enum { FALL, FLASH, RISE, SCROLL, STATIC } display_properties;

struct MessageInfo
{
public:
    line_positions position;
    const char * message;
    display_properties properties;

    MessageInfo() : position(line_positions::MIDDLE), message(""), properties(display_properties::SCROLL) { }

    MessageInfo(line_positions position_, const char * message_, display_properties properties_) : position(position_), message(message_), properties(properties_) { }
};

struct task_data
{
private:
public:
  QueueHandle_t messageQueue;
  const char * message;
  SSD1306Wire * display;
  bool running = false;
  line_positions screen_position;
  display_properties properties;
  task_data() { message = nullptr; }
  task_data(SSD1306Wire *display_, QueueHandle_t messageQueue_) 
    : screen_position(line_positions::MIDDLE), properties(display_properties::SCROLL)
  {
    display = display_;
    messageQueue = messageQueue_;
  }

  void set_running() { running = true; }
  void set_stopped() { running = false; }
  void set_message(const char * message_) { if(running) return; message = message_; }
  void set_position(line_positions position) { if(running) return; screen_position = position; }
  void set_display_properties(display_properties properties_) { if(running) return; properties = properties_; }
};

class Message
{
private:
  const char * the_message;
  const int _screenWidth = 128;
  int x_position = _screenWidth;
  int y_position;
  int message_length;
  SSD1306Wire* _display;
  display_properties _displayProperties;
  bool scroll()
  {
    while(x_position != -(message_length))
    {
      _display->clear();
      _display->drawString(x_position, y_position, the_message);
      _display->display(); 
      x_position -= 1;
      vTaskDelay(15);
    }
    return true;
  }
  bool rise()
  {
    x_position = (_display->width() - message_length)/2;
    while(y_position != -19)
    {
      _display->clear();
      _display->drawString(x_position, y_position, the_message);
      _display->display(); 
      y_position -= 1;
      vTaskDelay(15);
    }
    return true;
  }
  bool show()
  {
    x_position = (_display->width() - message_length)/2;
    _display->clear();
    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    vTaskDelay(250);
    return true;
  }
  bool fall()
  {
    x_position = (_display->width() - message_length)/2;
    while(y_position != _display->height()+1)
    {
      _display->clear();
      _display->drawString(x_position, y_position, the_message);
      _display->display(); 
      y_position += 1;
      vTaskDelay(15);
    }
    return true;
  }
  bool flash()
  {
    x_position = (_display->width() - message_length)/2;

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
      if(shown)
      {
        _display->clear();
        _display->display(); 
      }
      else
      {
        _display->drawString(x_position, y_position, the_message);
        _display->display(); 
      }

      shown = !shown;
      vTaskDelay(125);
    }
    return true;
  }
public:
  Message(task_data* data) 
  {
    the_message = data->message;
    _display = data->display;
    y_position = data->screen_position * 19;
    message_length = _display->getStringWidth(the_message);
    _displayProperties = data->properties;
  }
  bool display()
  {
    if(_displayProperties == display_properties::SCROLL)
        return scroll();
    if(_displayProperties == display_properties::FLASH)
        return flash();
    if(_displayProperties == display_properties::RISE)
        return rise();
    if(_displayProperties == display_properties::FALL)
        return fall();
    if(_displayProperties == display_properties::STATIC)
        return show();

    return false;
  }
};

class Gsdc_SSD1306
{
private:
    SSD1306Wire* _display;
    task_data* _taskData;
    QueueHandle_t _messageQueue;
public:
    Gsdc_SSD1306(uint8_t i2c_address, int sda_pin, int scl_pin);
    void begin();
    void fall(line_positions starting_line, const char * message);
    void flash(line_positions position, const char * message);
    void rise(line_positions starting_line, const char * message);
    void scroll(line_positions position, const char * message);
    void show(line_positions position, const char * message);
};

#endif
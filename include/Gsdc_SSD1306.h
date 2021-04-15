#ifndef Gsdc_SSD1306_h_
#define Gsdc_SSD1306_h_

#include <Arduino.h>
#include <SSD1306Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

typedef enum { TOP, MIDDLE, BOTTOM } line_positions;
typedef enum { CLEAR, CLEAR_INFO, CLEAR_LINE, FALL, FLASH, RISE, SCROLL, STATIC } display_properties;

struct MessageInfo
{
public:
    line_positions position;
    const char * message;
    display_properties properties;

    MessageInfo() : position(line_positions::MIDDLE), message(""), properties(display_properties::SCROLL) { }

    MessageInfo(line_positions position_, const char * message_, display_properties properties_) : position(position_), message(message_), properties(properties_) { }
};

struct task_services
{
public:
  QueueHandle_t messageQueue;
  SSD1306Wire * display;
  bool running = false;

  task_services(SSD1306Wire *display_, QueueHandle_t messageQueue_)
  {
    display = display_;
    messageQueue = messageQueue_;
  }
  void set_running() { running = true; }
  void set_stopped() { running = false; }
};

struct task_data
{
public:
  const char * message;
  SSD1306Wire * display;
  line_positions screen_position;
  display_properties properties;
  task_data() : screen_position(line_positions::MIDDLE), properties(display_properties::SCROLL)
  {
    message = nullptr; 
    display = nullptr;
  }

  void set_message(const char * message_) { return; message = message_; }
  void set_position(line_positions position) { return; screen_position = position; }
  void set_display_properties(display_properties properties_) {  properties = properties_; }
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
  bool scroll();
  bool rise();
  bool show();
  bool fall();
  bool flash();
  void clearLine();
  void clearLine(int x_position, int y_position);
  bool scrub();
  bool scrubInfo();
  bool scrubLine();
public:
  Message(task_data* data);
  bool display();
};

class Gsdc_SSD1306
{
private:
    SSD1306Wire* _display;
    task_services* _taskData;
    QueueHandle_t _messageQueue;
public:
    Gsdc_SSD1306(uint8_t i2c_address, int sda_pin, int scl_pin);
    void begin();
    void fall(line_positions starting_line, const char * message);
    void flash(line_positions position, const char * message);
    void rise(line_positions starting_line, const char * message);
    void scroll(line_positions position, const char * message);
    void scrub();
    void scrubInfo();
    void scrubLine(line_positions position);
    void show(line_positions position, const char * message);

    bool IsProcessingMessages() { return _taskData->running; }
};

#endif
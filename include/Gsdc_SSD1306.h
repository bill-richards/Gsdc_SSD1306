#ifndef Gsdc_SSD1306_h_
#define Gsdc_SSD1306_h_

#include <Arduino.h>
#include <SSD1306Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define CALL_AND_WAIT_500_MILLIS(message) \
  message; \
  _display.wait(500);

typedef enum { TOP, MIDDLE, BOTTOM, NO_POSITION } line_positions;
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

struct MessageInfo
{
public:
  TickType_t period = 0;
  line_positions position;
  const char * message;
  display_properties properties;

  MessageInfo() : position(NO_POSITION), message(nullptr), properties(NO_PROPERTY) { }
  MessageInfo(TickType_t period_) : period(period_), position(NO_POSITION), message(nullptr), properties(WAIT) { }
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
  TickType_t period;
  const char * message;
  SSD1306Wire * display;
  line_positions screen_position;
  display_properties properties;
  task_data() : screen_position(NO_POSITION), properties(display_properties::NO_PROPERTY)
  {
    message = nullptr; 
    display = nullptr;
  }
};

class Message
{
private:
  const char * the_message;
  const int _screenWidth = 128;
  int x_position = _screenWidth;
  int y_position;
  int message_length;
  line_positions _linePosition;
  SSD1306Wire* _display;
  display_properties _displayProperties;
  TickType_t _waitPeriod;
  bool center();
  void clear();
  void clearLine();
  void clearLine(int x_position, int y_position);
  void clearMessage(int x, int y, int length);
  bool fall();
  bool flashCenter();
  bool flashLeft();
  bool flashRight();
  bool left();
  bool rise();
  bool right();
  bool scroll();
  bool scrollRight();
  bool scrollToCenter();
  bool scrubInfo();
  bool scrubLine();
  bool scrubScreen();
  bool wait();
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
    void center(line_positions starting_line, const char * message);
    void centerHeading(const char * message);
    void clear();
    void clearLine(line_positions position) ;
    void fall(line_positions starting_line, const char * message);
    void flashCenter(line_positions position, const char * message);
    void flashHeading(const char * message);
    void flashLeft(line_positions position, const char * message);
    void flashRight(line_positions position, const char * message);
    void heading(const char * message);
    void important(const char * message);
    void left(line_positions starting_line, const char * message);
    void leftHeading(const char * message);
    void rise(line_positions starting_line, const char * message);
    void right(line_positions starting_line, const char * message);
    void rightHeading(const char * message);
    void scroll(line_positions position, const char * message);
    void scrollRight(line_positions position, const char * message);
    void scrollLeftHeading(const char * message);
    void scrollToCenter(line_positions position, const char * message);
    void scrub();
    void scrubHeading();
    void scrubInfo();
    void scrubLine(line_positions position);
    void wait(TickType_t period);

    bool IsProcessingMessages() { return _taskData->running; }
};

#endif
#include "structs.h"

#ifndef Gsdc_SSD1306_h_
#define Gsdc_SSD1306_h_

class Gsdc_SSD1306
{
private:
    SSD1306Wire* _display;
    task_data* _taskServices;
    QueueHandle_t _messageQueue;
public:
    Gsdc_SSD1306(uint8_t i2c_address, int sda_pin, int scl_pin);
    void begin();
    void center(line_positions starting_line, String message);
    void centerHeading(String message);
    void clear();
    void clearLine(line_positions position) ;
    void fall(line_positions starting_line, String message);
    void flashCenter(line_positions position, String message);
    void flashHeading(String message);
    void flashLeft(line_positions position, String message);
    void flashRight(line_positions position, String message);
    void heading(String message);
    void important(String message);
    void left(line_positions starting_line, String message);
    void leftHeading(String message);
    void rise(line_positions starting_line, String message);
    void right(line_positions starting_line, String message);
    void rightHeading(String message);
    void scroll(line_positions position, String message);
    void scrollRight(line_positions position, String message);
    void scrollLeftHeading(String message);
    void scrollToCenter(line_positions position, String message);
    void scrub();
    void scrubHeading();
    void scrubInfo();
    void scrubLine(line_positions position);
    void wait(ulong period);

    bool IsProcessingMessages() { return _taskServices->running; }
};

/* *********************************************************************************************** 
 * MACROS
 *********************************************************************************************** */
#define CALL_AND_WAIT_100_MILLIS(message) \
  message; \
  _display.wait(100);
#define CALL_AND_WAIT_250_MILLIS(message) \
  message; \
  _display.wait(250);
#define CALL_AND_WAIT_500_MILLIS(message) \
  message; \
  _display.wait(500);


/* *********************************************************************************************** */

#endif
#include "Gsdc_SSD1306.h"
#include "MessageTask.h"


Gsdc_SSD1306::Gsdc_SSD1306(uint8_t i2c_address, int sda_pin, int scl_pin)
{
    _display = new SSD1306Wire(i2c_address, sda_pin, scl_pin);
    _messageQueue = xQueueCreate(64, sizeof(struct Message_Info));
    _taskServices = new task_data(_display, _messageQueue);
}

void Gsdc_SSD1306::begin()
{
    _display->init();
    _display->clear();
    _display->flipScreenVertically();
    _display->setColor(WHITE);
    _display->setFont(ArialMT_Plain_16);
    _display->setTextAlignment(TEXT_ALIGN_LEFT);

    xTaskCreatePinnedToCore(messageTask, "MSG_TSK", 10000, (void *)_taskServices, 2, NULL, 1);
}

void Gsdc_SSD1306::center(line_positions starting_line, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = starting_line;
    info.properties = CENTER;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::centerHeading(String message)
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = TOP;
    info.properties = CENTER;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::clear() 
{
    Message_Info info;
    info.properties = CLEAR;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::clearLine(line_positions position) 
{
    Message_Info info;
    info.position = position;
    info.properties = CLEAR_LINE;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::fall(line_positions starting_line, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = starting_line;
    info.properties = FALL;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::flashCenter(line_positions position, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = position;
    info.properties = FLASH_CENTER;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::flashHeading(String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = TOP;
    info.properties = FLASH_CENTER;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::flashLeft(line_positions position, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = position;
    info.properties = FLASH_LEFT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::flashRight(line_positions position, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = position;
    info.properties = FLASH_RIGHT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::heading(String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = TOP;
    info.properties = SCROLL_TO_CENTER;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::important(String message)
{   
    scrub();
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = MIDDLE;
    info.properties = FLASH_CENTER;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::left(line_positions starting_line, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = starting_line;
    info.properties = LEFT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::leftHeading(String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = TOP;
    info.properties = LEFT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::rise(line_positions starting_line, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = starting_line;
    info.properties = RISE;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::right(line_positions starting_line, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = starting_line;
    info.properties = RIGHT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::rightHeading(String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = TOP;
    info.properties = RIGHT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::scroll(line_positions position, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = position;
    info.properties = SCROLL;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrollLeftHeading(String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = TOP;
    info.properties = SCROLL;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrollRight(line_positions position, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = position;
    info.properties = SCROLL_RIGHT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrollToCenter(line_positions position, String message) 
{
    Message_Info info;
    strcpy(info.message, message.c_str());
    info.position = position;
    info.properties = SCROLL_TO_CENTER;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::scrub() 
{
    Message_Info info;
    info.properties = SCRUB;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubHeading() 
{
    Message_Info info;
    info.position = TOP;
    info.properties = SCRUB_LINE;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubInfo() 
{
    Message_Info info;
    info.position = MIDDLE;
    info.properties = SCRUB_INFO;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubLine(line_positions position) 
{
    Message_Info info;
    info.position = position;
    info.properties = SCRUB_LINE;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::wait(ulong period) 
{
    Message_Info info;
    info.period = period;
    info.properties = WAIT;
    xQueueSendToBack(_messageQueue, (void *)&info, portMAX_DELAY);
}

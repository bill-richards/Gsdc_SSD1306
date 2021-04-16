#include "Gsdc_SSD1306.h"

/**
 *  Message members
 */

Message::Message(task_data* data) 
{
    the_message = data->message;
    _display = data->display;
    y_position = data->screen_position * 19;
    _linePosition = data->screen_position;
    message_length = _display->getStringWidth(the_message);
    _displayProperties = data->properties;
    _waitPeriod = data->period;
}

bool Message::display()
{

    if(_displayProperties == display_properties::CENTER)            { return center();}
    if(_displayProperties == display_properties::CLEAR)             { clear(); return true;}
    if(_displayProperties == display_properties::CLEAR_LINE)        { clearLine(); return true;}
    if(_displayProperties == display_properties::FALL)              { return fall(); }
    if(_displayProperties == display_properties::FLASH_CENTER)      { return flashCenter(); }
    if(_displayProperties == display_properties::FLASH_LEFT)        { return flashLeft(); }
    if(_displayProperties == display_properties::FLASH_RIGHT)       { return flashRight(); }
    if(_displayProperties == display_properties::LEFT)              { return left(); }
    if(_displayProperties == display_properties::RISE)              { return rise(); }
    if(_displayProperties == display_properties::RIGHT)             { return right(); }
    if(_displayProperties == display_properties::SCROLL)            { return scroll(); }
    if(_displayProperties == display_properties::SCROLL_RIGHT)      { return scrollRight(); }
    if(_displayProperties == display_properties::SCROLL_TO_CENTER)  { return scrollToCenter(); }
    if(_displayProperties == display_properties::SCRUB)             { return scrubScreen(); }
    if(_displayProperties == display_properties::SCRUB_INFO)        { return scrubInfo(); }
    if(_displayProperties == display_properties::SCRUB_LINE)        { return scrubLine(); }
    if(_displayProperties == display_properties::WAIT)              { return wait(); }

    return false;
}

bool Message::scroll()
{
    int max_width = _display->getWidth();
    int final_x_position = (message_length >= max_width)
        ? -message_length
        : 0;

    while(x_position != final_x_position)
    {
        clearLine();
        _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        x_position -= 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::scrollRight()
{
    x_position = _display->getWidth();
    int final_x_position = x_position-message_length;
     
    while(x_position != final_x_position)
    {
        clearMessage(x_position, y_position, message_length);
        _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        x_position -= 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::scrollToCenter()
{
    x_position = _display->getWidth();
    int final_x_position = (_display->width() - message_length)/2;

    while(x_position != final_x_position)
    {
        clearLine();
        _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        x_position -= 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::center()
{
    x_position = (_display->width() - message_length)/2;
    clearLine(0, y_position);
    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    return true;
}

bool Message::left()
{
    x_position = 0;
    clearMessage(0, y_position, message_length);
    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    return true;
}

void Message::clearMessage(int x_position, int y_position, int length)
{
    _display->setColor(BLACK);
    for(int y_pos = y_position; y_pos <= y_position+19; y_pos++)
    {
        _display->drawHorizontalLine(x_position, y_pos, length);
    }
    _display->setColor(WHITE);
}

bool Message::wait() { vTaskDelay(_waitPeriod/portTICK_PERIOD_MS); return true; }

bool Message::flashLeft()
{
    x_position = 0;
    clearMessage(x_position, y_position, message_length);

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
        if(shown) clearMessage(x_position, y_position, message_length);
        else _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        shown = !shown;
        vTaskDelay(125);
    }
    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    return true;
}
bool Message::flashRight()
{
    x_position = _display->getWidth() - message_length;
    clearMessage(x_position, y_position, message_length);

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
        if(shown) clearMessage(x_position, y_position, message_length);
        else _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        shown = !shown;
        vTaskDelay(125);
    }
    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    return true;
}
bool Message::flashCenter()
{
    x_position = (_display->width() - message_length)/2;
    clearMessage(x_position, y_position, message_length);

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
        if(shown) clearMessage(x_position, y_position, message_length);
        else _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        shown = !shown;
        vTaskDelay(125);
    }
    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    return true;
}

bool Message::right()
{
    x_position = _display->getWidth()-message_length;
    clearMessage(x_position, y_position, message_length);

    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    vTaskDelay(250);
    return true;
}

bool Message::rise()
{
    x_position = (_display->width() - message_length)/2;
    while(y_position != -19)
    {
        clearLine(x_position, y_position+1);
        _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        y_position -= 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::fall()
{
    x_position = (_display->width() - message_length)/2;
    while(y_position != _display->height()+1)
    {
        clearLine(x_position, y_position-1);
        _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        y_position += 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::scrubScreen()
{
    int x_position = 64;
    int y_position1 = 31;
    int y_position2 = 32;
    int line_extent = 1;

    while(x_position != 0)
    {
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);
        _display->display();
        x_position -= 1;
        line_extent += 2;
        vTaskDelay(2);
    }

    while(y_position1 != 0)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1+1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2-1, line_extent);

        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1--, line_extent);
        _display->drawHorizontalLine(x_position, y_position2++, line_extent);
        _display->display();

        vTaskDelay(1);
    }

    while(y_position1 != 31)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1-1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2+1, line_extent);

        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1++, line_extent);
        _display->drawHorizontalLine(x_position, y_position2--, line_extent);
        _display->display();

        vTaskDelay(1);
    }
    _display->setColor(OLEDDISPLAY_COLOR::BLACK);
    _display->drawHorizontalLine(x_position, y_position1-1, line_extent);
    _display->drawHorizontalLine(x_position, y_position2+1, line_extent);
    _display->setColor(OLEDDISPLAY_COLOR::WHITE);

    while(x_position != 64)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);

        x_position += 1;
        line_extent -= 2;
        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);
        _display->display();
        vTaskDelay(2);
    }

    _display->clear();
    return true;
}

bool Message::scrubInfo()
{
    int x_position = 64;
    int y_root = 19;
    int y_position1 = 42;
    int y_position2 = 43;
    int line_extent = 1;

    while(x_position != 0)
    {
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);
        _display->display();
        x_position -= 1;
        line_extent += 2;
        vTaskDelay(2);
    }

    while(y_position1 != y_root)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1+1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2-1, line_extent);

        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1--, line_extent);
        _display->drawHorizontalLine(x_position, y_position2++, line_extent);
        _display->display();

        vTaskDelay(1);
    }

    while(y_position1 != 42)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1-1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2+1, line_extent);

        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1++, line_extent);
        _display->drawHorizontalLine(x_position, y_position2--, line_extent);
        _display->display();

        vTaskDelay(1);
    }
    _display->setColor(OLEDDISPLAY_COLOR::BLACK);
    _display->drawHorizontalLine(x_position, y_position1-1, line_extent);
    _display->drawHorizontalLine(x_position, y_position2+1, line_extent);
    _display->setColor(OLEDDISPLAY_COLOR::WHITE);

    while(x_position != 64)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);

        x_position += 1;
        line_extent -= 2;
        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);
        _display->display();
        vTaskDelay(2);
    }

    clearLine(x_position, y_position1);
    clearLine(x_position, y_position2);
    return true;
}

bool Message::scrubLine()
{
    const int starting_point = 9;
    int x_position = 64;
    const int y_root = y_position;
    int y_position1 = y_root+starting_point;
    int y_position2 = y_root+starting_point;
    int line_extent = 1;

    while(x_position != 0)
    {
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->display();
        x_position -= 1;
        line_extent += 2;
        vTaskDelay(2);
    }

    while(y_position1 != y_root)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1+1, line_extent);
        
        if(y_position1 != y_root+starting_point)
            _display->drawHorizontalLine(x_position, y_position2-1, line_extent);

        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1--, line_extent);        
        _display->drawHorizontalLine(x_position, y_position2++, line_extent);
            
        _display->display();

        vTaskDelay(18);
    }

    while(y_position1 != y_root+starting_point)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1-1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2+1, line_extent);

        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1++, line_extent);
        _display->drawHorizontalLine(x_position, y_position2--, line_extent);
        _display->display();

        vTaskDelay(18);
    }
    _display->setColor(OLEDDISPLAY_COLOR::BLACK);
    _display->drawHorizontalLine(x_position, y_position1-1, line_extent);
    _display->drawHorizontalLine(x_position, y_position2+1, line_extent);
    _display->setColor(OLEDDISPLAY_COLOR::WHITE);

    while(x_position != 64)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);

        x_position += 1;
        line_extent -= 2;
        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
        _display->drawHorizontalLine(x_position, y_position1, line_extent);
        _display->drawHorizontalLine(x_position, y_position2, line_extent);
        _display->display();
        vTaskDelay(2);
    }

    clearLine(x_position, y_position1);
    return true;
}

void Message::clear() { _display->clear(); }

void Message::clearLine()
{
    for(int start = y_position; start < y_position+18; start++)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(0, start, 128);
        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
    }
    _display->display();
}

void Message::clearLine(int x_position, int y_position)
{
    int draw_from = (x_position < 0) ? 0 : x_position;
    for(int start = y_position; start < y_position+19; start++)
    {
        _display->setColor(OLEDDISPLAY_COLOR::BLACK);
        _display->drawHorizontalLine(draw_from, start, 128);
        _display->setColor(OLEDDISPLAY_COLOR::WHITE);
    }
    _display->display();
}

void messageTask(void * parameter)
{
    task_services services = *((task_services *)parameter);
    Serial.println("Starting message scroller task");

    for( ;; )
    {
        MessageInfo _currentMessage;

        if(xQueueReceive(services.messageQueue, &_currentMessage, (TickType_t) 10) == pdPASS)
        {
            services.set_running();
            task_data data_object;
            data_object.display = services.display;
            data_object.screen_position = _currentMessage.position;
            data_object.message = _currentMessage.message;
            data_object.period = _currentMessage.period;

            data_object.properties = _currentMessage.properties;
            Message message(&data_object);
            while(!message.display()){ vTaskDelay(1);} 
            if(!xQueuePeek(services.messageQueue, &_currentMessage, (TickType_t) 10))
                services.set_stopped();
        }
    }
}


/**
 *  Gsdc_SSD1306 Members
 */

Gsdc_SSD1306::Gsdc_SSD1306(uint8_t i2c_address, int sda_pin, int scl_pin)
{
    _display = new SSD1306Wire(i2c_address, sda_pin, scl_pin);
    _messageQueue = xQueueCreate(20, sizeof(MessageInfo));
    _taskData = new task_services(_display, _messageQueue);
}

void Gsdc_SSD1306::begin()
{
    _display->init();
    _display->clear();
    _display->flipScreenVertically();
    _display->setColor(WHITE);
    _display->setFont(ArialMT_Plain_16);
    _display->setTextAlignment(TEXT_ALIGN_LEFT);

    xTaskCreatePinnedToCore(messageTask, "MSG_TSK", 20480, (void *)_taskData, 3, NULL, 1);
}


void Gsdc_SSD1306::center(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, CENTER);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::centerHeading(const char * message)
{
    MessageInfo info(TOP, message, CENTER);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::clear() 
{
    MessageInfo info(NO_POSITION, nullptr, CLEAR);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::clearLine(line_positions position) 
{
    MessageInfo info(position, nullptr, CLEAR_LINE);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::fall(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, FALL);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::flashCenter(line_positions position, const char * message) 
{
    MessageInfo info(position, message, FLASH_CENTER);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::flashHeading(const char * message) 
{
    MessageInfo info(TOP, message, FLASH_CENTER);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::flashLeft(line_positions position, const char * message) 
{
    MessageInfo info(position, message, FLASH_LEFT);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::flashRight(line_positions position, const char * message) 
{
    MessageInfo info(position, message, FLASH_RIGHT);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::heading(const char * message) 
{
    MessageInfo info(TOP, message, SCROLL_TO_CENTER);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::important(const char * message)
{   
    scrub();
    MessageInfo info(MIDDLE, message, FLASH_CENTER);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::left(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, LEFT);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::leftHeading(const char * message) 
{
    MessageInfo info(TOP, message, LEFT);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::rise(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, RISE);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::right(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, RIGHT);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::rightHeading(const char * message) 
{
    MessageInfo info(TOP, message, RIGHT);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::scroll(line_positions position, const char * message) 
{
    MessageInfo info(position, message, SCROLL);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrollLeftHeading(const char * message) 
{
    MessageInfo info(TOP, message, SCROLL);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrollRight(line_positions position, const char * message) 
{
    MessageInfo info(position, message, SCROLL_RIGHT);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrollToCenter(line_positions position, const char * message) 
{
    MessageInfo info(position, message, SCROLL_TO_CENTER);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::scrub() 
{
    MessageInfo info(NO_POSITION, nullptr, SCRUB);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubHeading() 
{
    MessageInfo info(TOP, nullptr, SCRUB_LINE);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubInfo() 
{
    MessageInfo info(MIDDLE, nullptr, SCRUB_INFO);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubLine(line_positions position) 
{
    MessageInfo info(position, nullptr, SCRUB_LINE);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::wait(TickType_t period) 
{
    MessageInfo info(period);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

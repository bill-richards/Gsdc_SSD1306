#include "Gsdc_SSD1306.h"

/**
 *  Message members
 */

Message::Message(task_data* data) 
{
    the_message = data->message;
    _display = data->display;
    y_position = data->screen_position * 19;
    message_length = _display->getStringWidth(the_message);
    _displayProperties = data->properties;
}

bool Message::display()
{
    if(_displayProperties == display_properties::CLEAR)
        return scrub();
    if(_displayProperties == display_properties::CLEAR_INFO)
        return scrubInfo();
    if(_displayProperties == display_properties::CLEAR_LINE)
        return scrubLine();
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
bool Message::show()
{
    x_position = (_display->width() - message_length)/2;
    clearLine();
    _display->drawString(x_position, y_position, the_message);
    _display->display(); 
    vTaskDelay(250);
    return true;
}
bool Message::flash()
{
    x_position = (_display->width() - message_length)/2;
    clearLine();

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
        if(shown) clearLine();
        else _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        shown = !shown;
        vTaskDelay(125);
    }
    _display->drawString(x_position, y_position, the_message);
    return true;
}





bool Message::rise()
{
    x_position = (_display->width() - message_length)/2;
    while(y_position != -19)
    {
        clearLine();
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
        clearLine();
        _display->drawString(x_position, y_position, the_message);
        _display->display(); 
        y_position += 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::scrub()
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
        vTaskDelay(5);
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

        vTaskDelay(5);
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

        vTaskDelay(5);
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
        vTaskDelay(5);
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
        vTaskDelay(5);
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

        vTaskDelay(5);
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

        vTaskDelay(5);
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
        vTaskDelay(5);
    }

    clearLine(x_position, y_position1);
    clearLine(x_position, y_position2);
    return true;
}

bool Message::scrubLine()
{
    int y_top = y_position;
    int y_bottom = y_top + 19;

    y_position += 10;
    x_position = 64;
    int line_extent = 1;

    while(x_position != 0)
    {
        _display->drawHorizontalLine(x_position, y_position, line_extent);
        _display->display();
        x_position -= 1;
        line_extent += 2;
        vTaskDelay(5);
    }

    while(y_position != y_top)
    {
        clearLine();
        _display->drawHorizontalLine(x_position, y_position--, line_extent);
        _display->display();
        vTaskDelay(18);
    }

    while(y_position != y_bottom)
    {
        clearLine();
        y_position += 1;
        _display->drawHorizontalLine(x_position, y_position, line_extent);
        _display->display();
        vTaskDelay(18);
    }

    while(y_position != y_top+10)
    {
        clearLine();
        y_position -= 1;
        _display->drawHorizontalLine(x_position, y_position, line_extent);
        _display->display();
        vTaskDelay(18);
    }

    while(x_position != 64)
    {
        clearLine();
        x_position += 1;
        line_extent -= 2;
        _display->drawHorizontalLine(x_position, y_position, line_extent);
        _display->display();
        vTaskDelay(5);
    }
    clearLine();
    return true;
}

void Message::clearLine()
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

            data_object.properties = _currentMessage.properties;
            Message message(&data_object);
            while(!message.display()){ vTaskDelay(1);} 
            if(!xQueuePeek(services.messageQueue, &_currentMessage, (TickType_t) 10))
                services.set_stopped();
        }
    }
}

Gsdc_SSD1306::Gsdc_SSD1306(uint8_t i2c_address, int sda_pin, int scl_pin)
{
    _display = new SSD1306Wire(i2c_address, sda_pin, scl_pin);
    _messageQueue = xQueueCreate(10, sizeof(MessageInfo));
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

void Gsdc_SSD1306::scroll(line_positions position, const char * message) 
{
    MessageInfo info(position, message, display_properties::SCROLL);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::flash(line_positions position, const char * message) 
{
    MessageInfo info(position, message, display_properties::FLASH);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

void Gsdc_SSD1306::rise(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, display_properties::RISE);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::fall(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, display_properties::FALL);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::show(line_positions starting_line, const char * message) 
{
    MessageInfo info(starting_line, message, display_properties::STATIC);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrub() 
{
    MessageInfo info(line_positions::TOP, "", display_properties::CLEAR);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubInfo() 
{
    MessageInfo info(line_positions::MIDDLE, "", display_properties::CLEAR_INFO);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}
void Gsdc_SSD1306::scrubLine(line_positions position) 
{
    MessageInfo info(position, "", display_properties::CLEAR_LINE);
    xQueueSend(_messageQueue, (void *)&info, portMAX_DELAY);
}

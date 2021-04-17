#include "Message.h"

Message::Message(Message_Info* data, SSD1306Wire* display_) 
{
    _theMessage = data->message;
    _display = display_;
    y_position = data->position * 19;
    _linePosition = data->position;
    _messageWidth = _display->getStringWidth(_theMessage);
    _displayProperties = data->properties;
    _waitPeriod = data->period;
}

bool Message::center()
{
    x_position = (_display->width() - _messageWidth)/2;
    clearLine(0, y_position);
    _display->drawString(x_position, y_position, _theMessage);
    _display->display(); 
    return true;
}

void Message::clear() { _display->clear(); _display->display(); }
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
void Message::clearMessage(int x_position, int y_position, int length)
{
    _display->setColor(BLACK);
    for(int y_pos = y_position; y_pos <= y_position+19; y_pos++)
    {
        _display->drawHorizontalLine(x_position, y_pos, length);
    }
    _display->setColor(WHITE);
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

bool Message::fall()
{
    x_position = (_display->width() - _messageWidth)/2;
    while(y_position <= _display->height())
    {
        clearLine(x_position, y_position-1);
        _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        y_position += 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::flashLeft()
{
    x_position = 0;
    clearMessage(x_position, y_position, _messageWidth);

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
        if(shown) clearMessage(x_position, y_position, _messageWidth);
        else _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        shown = !shown;
        vTaskDelay(125);
    }
    _display->drawString(x_position, y_position, _theMessage);
    _display->display(); 
    return true;
}
bool Message::flashRight()
{
    x_position = _display->getWidth() - _messageWidth;
    clearMessage(x_position, y_position, _messageWidth);

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
        if(shown) clearMessage(x_position, y_position, _messageWidth);
        else _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        shown = !shown;
        vTaskDelay(125);
    }
    _display->drawString(x_position, y_position, _theMessage);
    _display->display(); 
    return true;
}
bool Message::flashCenter()
{
    x_position = (_display->width() - _messageWidth)/2;
    clearMessage(x_position, y_position, _messageWidth);

    int start = millis();
    int end = start + 2000;
    bool shown = false;
    while(millis() <= end)
    {
        if(shown) clearMessage(x_position, y_position, _messageWidth);
        else _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        shown = !shown;
        vTaskDelay(125);
    }
    _display->drawString(x_position, y_position, _theMessage);
    _display->display(); 
    return true;
}

bool Message::left()
{
    x_position = 0;
    clearMessage(0, y_position, _messageWidth);
    _display->drawString(x_position, y_position, _theMessage);
    _display->display(); 
    return true;
}

bool Message::right()
{
    x_position = _display->getWidth()-_messageWidth;
    clearMessage(x_position, y_position, _messageWidth);

    _display->drawString(x_position, y_position, _theMessage);
    _display->display(); 
    return true;
}

bool Message::rise()
{
    x_position = (_display->width() - _messageWidth)/2;
    while(y_position != -19)
    {
        clearLine(x_position, y_position+1);
        _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        y_position -= 1;
        vTaskDelay(15);
    }
    return true;
}

bool Message::scroll()
{
    int max_width = _display->getWidth();
    int final_x_position = (_messageWidth >= max_width)
        ? -(_messageWidth+1)
        : 0;

    while(x_position != final_x_position)
    {
        clearMessage(x_position, y_position, max_width-x_position);
        _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        x_position -= 1;
        vTaskDelay(15);
    }
    return true;
}
bool Message::scrollRight()
{
    x_position = _display->getWidth();
    int final_x_position = x_position-_messageWidth;
     
    while(x_position != final_x_position)
    {
        clearMessage(x_position, y_position, _messageWidth);
        _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        x_position -= 1;
        vTaskDelay(15);
    }
    return true;
}
bool Message::scrollToCenter()
{
    x_position = _display->getWidth();
    int final_x_position = (_display->width() - _messageWidth)/2;

    while(x_position != final_x_position)
    {
        clearLine();
        _display->drawString(x_position, y_position, _theMessage);
        _display->display(); 
        x_position -= 1;
        vTaskDelay(15);
    }
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
    _display->display();
    return true;
}

bool Message::wait() 
{ 
    vTaskDelay(_waitPeriod); 
    return true; 
}
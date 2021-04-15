#include "Gsdc_SSD1306.h"

void messageTask(void * parameter)
{
    task_data* data_object = (task_data *)parameter;
    Serial.println("Starting message scroller task");
    MessageInfo _currentMessage;

    for( ;; )
    {
        if(xQueueReceive(data_object->messageQueue, &_currentMessage, (TickType_t) 10) == pdPASS)
        {
            while(data_object->running) { vTaskDelay(33); }
            data_object->set_position(_currentMessage.position);
            data_object->set_message(_currentMessage.message);
            data_object->set_display_properties(_currentMessage.properties);
            data_object->set_running();
            Message message(data_object);
            while(!message.display()){ vTaskDelay(1);} 
            data_object->set_stopped();
        }
    }
}

Gsdc_SSD1306::Gsdc_SSD1306(uint8_t i2c_address, int sda_pin, int scl_pin)
{
    this->_display = new SSD1306Wire(i2c_address, sda_pin, scl_pin);
    _messageQueue = xQueueCreate(10, sizeof(MessageInfo));
    _taskData = new task_data(_display, _messageQueue);
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
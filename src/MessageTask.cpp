#include "MessageTask.h"
#include "Message.h"

void messageTask(void * parameter)
{
    Serial.println("Starting queue monitor task");
    task_data data = *((task_data *)parameter);
    Message_Info currentMessage;
    Message_Info peekMessage;

    for( ;; )
    {
        if(xQueueReceive(data.messageQueue, &currentMessage, (TickType_t) 10) != pdPASS) continue;

        data.set_running();
        Message message(&currentMessage, data.display);
        while(!message.display()) { vTaskDelay(1); }

        if(!xQueuePeek(data.messageQueue, &peekMessage, (TickType_t) 10)) data.set_stopped();
    }
}

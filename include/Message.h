#ifndef Gsdc_Message_h_
#define Gsdc_Message_h_

#include "structs.h"

class Message
{
private:
  String _theMessage = "";
  const int _screenWidth = 128;
  int x_position = _screenWidth;
  int y_position = 0;
  int _messageWidth = 0;
  line_positions _linePosition = NO_POSITION;
  SSD1306Wire* _display = nullptr;
  display_properties _displayProperties = NO_PROPERTY;
  ulong _waitPeriod = 0;
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
  Message(Message_Info* data, SSD1306Wire* _display);
  bool display();
  String toString()
  {
    String me = "Position -> ";
    me += _linePosition;
    me += " | Property -> ";
    me += _displayProperties;
    me += " | Wait     -> ";
    me += _waitPeriod;
    if(_waitPeriod == 0)
    {
      me += " | Message  -> ";
      me += _theMessage;
    }
    return me;
  }
};

#define NO_MESSAGE ""

#endif
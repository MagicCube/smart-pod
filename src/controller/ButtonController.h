#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include <Arduino.h>

#include "../SmartPod.h"

#define BUTTON_PRESS_BETWEEN 200
#define BUTTON_REPEAT_UNTIL 500
#define BUTTON_REPEAT_DURATION 200


class ButtonController
{
  public:
    ButtonController(SmartPod *smartPod);
    void begin();
    void handle();

  private:
    SmartPod* _smartPod;

    long _middleButtonDown = 0;
    long _middleButtonUp = 0;
    bool _middleButtonLongPressed = false;
    void _middleButton_onPressed();
    void _middleButton_onLongPressed();

    long _leftButtonDown = 0;
    long _leftButtonUp = 0;
    long _leftButtonHold = 0;
    void _leftButton_onPressed();
    void _leftButton_onLongPressed();

    long _rightButtonDown = 0;
    long _rightButtonUp = 0;
    long _rightButtonHold = 0;
    void _rightButton_onPressed();
    void _rightButton_onLongPressed();
};

#endif

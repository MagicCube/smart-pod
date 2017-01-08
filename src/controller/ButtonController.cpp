#include "ButtonController.h"

#include <Console.h>

#include "../wiring.h"

ButtonController::ButtonController(SmartPod *smartPod)
{
    _smartPod = smartPod;
}

void ButtonController::begin()
{
    pinMode(MIDDLE_BUTTON_PIN, INPUT);
    pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
}

void ButtonController::handle()
{
    bool pressed;

    pressed = digitalRead(MIDDLE_BUTTON_PIN) == HIGH;
    if (_middleButtonDown == 0)
    {
        // If the button has just been pressed.
        if (pressed && millis() - _middleButtonUp > BUTTON_PRESS_BETWEEN)
        {
            _middleButtonDown = millis();
        }
    }
    else
    {
        // If the button has just been released.
        if (pressed)
        {
            // Holding
            if (!_middleButtonLongPressed &&
                millis() - _middleButtonDown > BUTTON_REPEAT_UNTIL + BUTTON_REPEAT_DURATION)
            {
                _middleButtonLongPressed = true;
                _middleButton_onLongPressed();
            }
            else if (millis() - _middleButtonDown > 1500)
            {
                //_smartPod->sleep();
                return;
            }
        }
        else
        {
            _middleButtonDown = 0;
            _middleButtonUp = millis();
            if (!_middleButtonLongPressed)
            {
                _middleButtonLongPressed = false;
                _middleButton_onPressed();
            }
            else
            {
                _middleButtonLongPressed = false;
            }
        }
    }


    pressed = digitalRead(LEFT_BUTTON_PIN) == LOW;
    if (_leftButtonDown == 0)
    {
        // If the button has just been pressed.
        if (pressed && millis() - _leftButtonUp > BUTTON_PRESS_BETWEEN)
        {
            _leftButtonDown = millis();
        }
    }
    else
    {
        // If the button has just been released.
        if (pressed)
        {
            // Holding
            if (millis() - _leftButtonDown > BUTTON_REPEAT_UNTIL &&
               (_leftButtonHold == 0 || millis() - _leftButtonHold > BUTTON_REPEAT_DURATION))
            {
                _leftButtonHold = millis();
                _leftButton_onLongPressed();
            }
        }
        else
        {
            _leftButtonDown = 0;
            _leftButtonUp = millis();
            if (_leftButtonHold == 0)
            {
                _leftButton_onPressed();
            }
            else
            {
                // Long hold
                _leftButtonHold = 0;
            }
        }
    }


    pressed = digitalRead(RIGHT_BUTTON_PIN) == LOW;
    if (_rightButtonDown == 0)
    {
        // If the button has just been pressed.
        if (pressed && millis() - _rightButtonUp > BUTTON_PRESS_BETWEEN)
        {
            _rightButtonDown = millis();
        }
    }
    else
    {
        // If the button has just been released.
        if (pressed)
        {
            // Holding
            if (millis() - _rightButtonDown > BUTTON_REPEAT_UNTIL &&
               (_rightButtonHold == 0 || millis() - _rightButtonHold > BUTTON_REPEAT_DURATION))
            {
                _rightButtonHold = millis();
                _rightButton_onLongPressed();
            }
        }
        else
        {
            _rightButtonDown = 0;
            _rightButtonUp = millis();
            if (_rightButtonHold == 0)
            {
                _rightButton_onPressed();
            }
            else
            {
                // Long hold
                _rightButtonHold = 0;
            }
        }
    }
}


void ButtonController::_middleButton_onPressed()
{
    Console::debug("[P] PLAY / PAUSE");
    _smartPod->playPause();
}
void ButtonController::_middleButton_onLongPressed()
{
    Console::debug("[M] SWITCH MODE");
    _smartPod->switchMode();
}


void ButtonController::_rightButton_onPressed()
{
    Console::debug("[>] NEXT");
    _smartPod->next();
}
void ButtonController::_rightButton_onLongPressed()
{
    Console::debug("[+] VOLUME UP");
    _smartPod->setVolumeUp();
}


void ButtonController::_leftButton_onPressed()
{
    Console::debug("[<] PREVIOUS");
    _smartPod->prev();
}
void ButtonController::_leftButton_onLongPressed()
{
    Console::debug("[-] VOLUME DOWN");
    _smartPod->setVolumeDown();
}

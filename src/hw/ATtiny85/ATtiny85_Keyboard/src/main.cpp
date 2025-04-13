#include <Arduino.h>
#include <DigiKeyboard.h>
#include <Wire.h>

#define FIX_SHIT(x) ((uint8_t)(x & 0xFF))

uint8_t funny[] =
  {FIX_SHIT(KEY_H),
   FIX_SHIT(KEY_A),
   FIX_SHIT(KEY_H),
   FIX_SHIT(KEY_A),
   FIX_SHIT(KEY_SPACE),
   FIX_SHIT(KEY_Y),
   FIX_SHIT(KEY_E),
   FIX_SHIT(KEY_S),
   FIX_SHIT(KEY_ENTER)};

uint8_t it           = 0;
bool past            = true;
bool led_state       = true;
bool is_usb          = false;
uint32_t last_update = 0;

void setup()
{
    pinMode(PB1, INPUT);
    pinMode(PB5, OUTPUT);

    uint32_t start = millis();
    while (((millis() - start) <= 1000) && (is_usb == false))
    {
        is_usb = usbInterruptIsReady();
        DigiKeyboard.delay(5);
    }
    // is_usb = true;

    if (is_usb)
    {
        DigiKeyboard.sendKeyStroke(0);
    }

    digitalWrite(PB5, HIGH);
}


void loop()
{
    if (is_usb)
    {
        if ((digitalRead(PB1) == LOW) && (past == false))
        {
            past = true;
            digitalWrite(PB5, LOW);

            DigiKeyboard.sendKeyPress(funny[it], 0);
            it++;
            if (it >= 9)
            {
                it = 0;
            }
        }
        else if ((digitalRead(PB1) == HIGH) && (past == true))
        {
            past = false;
            digitalWrite(PB5, HIGH);

            DigiKeyboard.sendKeyPress(0, 0);
        }

        DigiKeyboard.delay(1);
    }
    else
    {
        digitalWrite(PB5, !led_state);
        led_state = !led_state;

        delay(1000);
    }
}

/* -------------------------------- Includes -------------------------------- */
#include <Arduino.h>
#include <DigiKeyboard.h>
#include <Wire.h>

/* --------------------------------- Defines -------------------------------- */
#define FIX_SHIT(x) ((uint8_t)(x & 0xFF))

#define PIN_LED (PB1)
#define PIN_BTN (PB5)

/* -------------------------------- Typedefs -------------------------------- */
typedef enum
{
    KEY_STATE_IDLE = 0,
    KEY_STATE_PRESSED,
    KEY_STATE_HELD,
    KEY_STATE_RELEASED
} KeyState_t;


typedef struct
{
} Master_t;

typedef struct
{
    volatile KeyState_t key_state;
    volatile uint8_t led_brightness;
} Slave_t;

/* -------------------------------- Functions ------------------------------- */
/* Master state machine */

static void M_Init(Master_t* master_);
static void M_InitDelay(Master_t* master_);
static void M_SlaveDiscovery(Master_t* master_);
static void M_ReadSlaves(Master_t* master_);
static void M_ReadSelf(Master_t* master_);
static void M_SendKeys(Master_t* master_);
static void M_NoKeys(Master_t* master_);

/* Slave state machine */

static void S_Init(Slave_t* slave_);
static void S_Idle(Slave_t* slave_);
static void S_KeyPressed(Slave_t* slave_);
static void S_KeyHeld(Slave_t* slave_);

/* ---------------------------- Global Variables ---------------------------- */

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

uint8_t led_bright = 255;

/* ---------------------------- Arduino Functions --------------------------- */

void setup()
{
    pinMode(PIN_BTN, INPUT);
    pinMode(PIN_LED, OUTPUT);

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

    analogWrite(PIN_LED, 255);
}


void loop()
{
    if (is_usb)
    {
        if ((digitalRead(PIN_BTN) == LOW) && (past == false))
        {
            past = true;
            analogWrite(PIN_LED, 255);

            DigiKeyboard.sendKeyPress(funny[it], 0);
            it++;
            if (it >= 9)
            {
                it = 0;
            }
        }
        else if ((digitalRead(PIN_BTN) == HIGH) && (past == true))
        {
            past = false;
            analogWrite(PIN_LED, 127);

            DigiKeyboard.sendKeyPress(0, 0);
        }

        DigiKeyboard.delay(1);
    }
    else
    {
        analogWrite(PIN_LED, led_bright);
        if (led_state == false)
        {
            if (led_bright > 0)
            {
                led_bright -= 1;
            }
            else
            {
                led_bright = 1;
                led_state  = true;
            }
        }
        else
        {
            if (led_bright < 255)
            {
                led_bright += 1;
            }
            else
            {
                led_bright = 254;
                led_state  = false;
            }
        }

        delay(10);
    }
}

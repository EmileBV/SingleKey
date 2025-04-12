#include "DigiKeyboard.h"

uint8_t funny[] = { KEY_H, KEY_A, KEY_H, KEY_A, KEY_SPACE, KEY_Y, KEY_E, KEY_S, KEY_ENTER };
uint8_t it = 0;
bool past = true;
bool is_usb = false;
bool led_state = true;

uint32_t last_update = 0;

void setup() {
  // don't need to set anything up to use DigiKeyboard
  pinMode(PB1, INPUT);
  pinMode(PB5, OUTPUT);


  if (digitalRead(PB3)) {
    is_usb = true;
  }
  digitalWrite(PB5, HIGH);

  if (is_usb) {
    DigiKeyboard.sendKeyStroke(0);
  }

  last_update = millis();
}


void loop() {
  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  // DigiKeyboard.sendKeyStroke(0);

  // Type out this string letter by letter on the computer (assumes US-style
  // keyboard)
  if (is_usb) {
    if (digitalRead(PB1) == LOW && past == false) {
      past = true;
      // DigiKeyboard.println("Hello Digispark!");
      DigiKeyboard.sendKeyPress(funny[it], 0);
      // DigiKeyboard.write(funny[it]);
      it++;
      if (it >= 9) {
        it = 0;
      }
    } else if (digitalRead(PB1) == HIGH && past == true) {
      past = false;
      DigiKeyboard.sendKeyPress(0, 0);
    }
  }

  if (is_usb == false) 
  // if (is_usb == false && (millis() - last_update) > 1000) 
  {
    digitalWrite(PB5, !led_state);
    led_state = !led_state;
    // last_update = millis();
  }

  // It's better to use DigiKeyboard.delay() over the regular Arduino delay()
  // if doing keyboard stuff because it keeps talking to the computer to make
  // sure the computer knows the keyboard is alive and connected
  if (is_usb) {
    DigiKeyboard.delay(1);
  } else {
    delay(1000);
  }
}

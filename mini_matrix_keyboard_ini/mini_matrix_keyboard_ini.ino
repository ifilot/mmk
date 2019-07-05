/*
  mini_matrix_keyboard.ino - Sketch to build your own mini matrix keyboard

  Copyright (c) 2019 Ivo Filot

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <Keyboard.h>

// set time settings
static const unsigned long wait_time = 10;
static const unsigned long hold_time = 500;
static const unsigned long hold_interval = 50;

// set which pins act as columns and rows
int cols[5] = { 2,  3,  4,  5,  6};
int rows[3] = {19, 20, 21};

// define state of the keys
#define KEY_RELEASED   0
#define KEY_PRESSED     1

// build arrays to hold timer and state data for the keys
uint8_t keystate_old[13];
uint8_t keystate[13];

void setup() {
  // set columns to input (probe)
  for(int i=0; i<5; i++) {
    pinMode(cols[i], INPUT_PULLUP);
  }

  // set rows to output (pulse)
  for(int i=0; i<3; i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], HIGH);
  }

  // set initial values for the arrays
  for(int i=0; i<13; i++) {
    keystate[i] = KEY_RELEASED;
    keystate_old[i] = KEY_RELEASED;
  }

  // enable keyboard
  Keyboard.begin();
}

// set keys
int keys[] = {
  0,  1,  2,  3, 12,
  4,  5,  6,  7, -1,
  8,  9, 10, 11, -1
};

// set which character to print
char characters[] = {
'1', '2', '3', '4', '5',
'6', '7', '8', '9', 'A',
'B', 'C', 'D'
};

void loop() {
  // loop over rows
  for(int i=0; i<3; i++) {
    // pulse pin (set low)
    digitalWrite(rows[i], LOW);

      // loop over columns
      for(int j=0; j<5; j++) {

        // determine key identifier
        int keyid = keys[i*5+j];

        // skip if identifier is -1 (key does not exist)
        if(keyid == -1) {
          continue;
        }

        // determine if key is pressed
        if(digitalRead(cols[j]) == LOW) {
          keystate[keyid] = KEY_PRESSED;
        } else {
          keystate[keyid] = KEY_RELEASED;
        }

      }
    digitalWrite(rows[i], HIGH);
  }

  // determine action
  for(int i=0; i<13; i++) {
    // set press if state has changed from unpressed to pressed
    if(keystate[i] == KEY_PRESSED && keystate_old[i] == KEY_RELEASED) {
      Keyboard.press(characters[i]);
    }

    // set release if state has changed from pressed to unpressed
    if(keystate[i] == KEY_RELEASED && keystate_old[i] == KEY_PRESSED) {
      Keyboard.release(characters[i]);
    }

    // copy state to old
    keystate_old[i] = keystate[i];
  }

  delay(1);
}

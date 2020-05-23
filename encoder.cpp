#include "encoder.h"
//#include "Arduino.h"
#include <Arduino.h>

volatile char encoderPos = 0;
volatile char oldEncPos = 0; 
volatile uint8_t aFlag = 0;// let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile uint8_t bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile uint8_t reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

void PinA() {
  reading = PIND & (_BV(PORTD2) | _BV(PORTD3)); // read all eight pin values then strip away all but pinA and pinB's values
  if (reading == 0b00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == 0b00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
}

void PinB() {
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == 0b00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == 0b00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
}

void setup_encoder() {
  DDRD |= _BV(PORTD2) | _BV(PORTD3);
  PORTD |= _BV(PORTD2) | _BV(PORTD3);
  //pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  //pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0, PinA, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1, PinB, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
}

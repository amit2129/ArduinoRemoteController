#ifndef ENCODER_H
#define ENCODER_H

extern volatile char encoderPos; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
extern volatile char oldEncPos; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
#endif

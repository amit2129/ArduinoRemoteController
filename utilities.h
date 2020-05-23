#ifndef UTIL_H
#define UTIL_H
#define readEncoderButton (pind & _BV(PORTD5))
#define readSwitch (pind & _BV(PORTD4))
extern bool readDebounced(bool reading);
#endif

#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>
#include "Arduino.h"
#include "UniversalRemote.h"
#include "Pages.h"
//#include "printf.h"
//#include "U8glib.h"
#include "encoder.h"
//#include "display.h"
#include "rf.h"

#define NUM_PAGES 4


//extern U8GLIB_SH1106_128X64 u8g;

void setup_pins(){
  DDRD |= _BV(PORTD4) | _BV(PORTD5);
  PORTD |= _BV(PORTD4) | _BV(PORTD5);
  PORTC &= ~_BV(7);
}

extern Page **pages;
extern void setup_encoder();
extern void setup_pages();
extern void setup_rf();


Page ** UniversalRemote::pages = pages;
UniversalRemote universalRemote = UniversalRemote(pages);
UniversalRemote *RF::remote = &universalRemote;
RF *UniversalRemote::radio = new RF();
UniversalRemote *Page::remote = &universalRemote;


void setup() {
  Serial.begin(115200);
  Serial.println("program restart");
  //universalRemote.display_page();
  setup_encoder();
  Serial.println("after setup encoder");
  setup_pages();
  Serial.println("after setup pages");
  setup_pins();

}

void loop() {
  //Serial.println(analogRead(A7));
  if(oldEncPos != encoderPos){
    //page_select = Serial.parseInt();
    oldEncPos = encoderPos;
    Serial.println(encoderPos, DEC);
    Serial.println(((encoderPos % NUM_PAGES) + NUM_PAGES) % NUM_PAGES);
    universalRemote.set_page(((encoderPos % NUM_PAGES) + NUM_PAGES) % NUM_PAGES);
  }
  timer();
  //universalRemote.update();
}

void timer(){
  unsigned long before = millis();
  universalRemote.update();
  unsigned long after = millis();
  Serial.print("update takes: ");
  Serial.println(after - before);
}

#include "utilities.h"
#include "Arduino.h"


bool readDebounced(bool current_reading){
 //  Serial.print("current reading is: ");
 // Serial.println(current_reading);
  static bool old_reading;
  static unsigned long old_reading_time;
 // Serial.print("old reading time is: ");
//  Serial.println(old_reading_time);
  unsigned long current_reading_time;
  if (current_reading){
    current_reading_time = millis();
   // Serial.print("new reading time is: ");
   // Serial.println(current_reading_time);
    if (current_reading_time - old_reading_time < 80){
      //Serial.println("returning old reading");
      return 0;
    }
    old_reading_time = current_reading_time;
    return 1;
    
  }
  return 0;
    
}

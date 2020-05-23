#include "UniversalRemote.h"
#include "Arduino.h"
#include "globals.h"
#include "utilities.h"



UniversalRemote::UniversalRemote(Page **pages_t) {
  this->pages = pages_t;
  this->page_number = 0;
  this->in_page = false;
}

void UniversalRemote::set_page(uint8_t page_num) {
  if (in_page) return;
  if (page_number != page_num) {
    page_number = page_num;
    this->pages[page_number]->display_page();
  }
}

void UniversalRemote::update() {
  pind = PIND; // taking all 8 pin readings

  // selecting a field in a page
  if (readDebounced(!readEncoderButton && in_page && !this->pages[page_number]->selected)){
    this->pages[page_number]->enter_selection();
  }
  if (!in_page){
    if (readDebounced(!readEncoderButton))
      this->enter_page();
  }
  
  if (readSwitch && in_page)
    this->leave_page();
  if (readSwitch && this->pages[page_number]->selected)
    this->pages[page_number]->leave_selection();
  if (readDebounced(!readEncoderButton && in_page && this->pages[page_number]->selected)){
    this->pages[page_number]->leave_selection();

  }
  this->pages[page_number]->update();
}

void UniversalRemote::display_page() {
  this->pages[page_number]->display_page();
}

void UniversalRemote::enter_page() {
  if (in_page) return;
  //Serial.println("entered page");
  in_page = true;
  this->pages[page_number]->enter_page();
}

void UniversalRemote::leave_page() {
  if (!in_page) return;
  //Serial.println("leaving page");
  in_page = false;
  this->pages[page_number]->leave_page();
}

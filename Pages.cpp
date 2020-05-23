#include <stdint.h>
#include "encoder.h"
#include "Pages.h"
//#include "display.h"
#include "U8glib.h"
#include "globals.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // I2C
#define NUM_PAGES 4
#define HEADER_FRAME_HEIGHT 16
#define MAX_LINES 4
char Page::old_selection_encoder_val;
bool Page::selected = false;
char Page::old_encoder_val = 0;
bool Page::in_page = false;
int Page::state = 0;
uint8_t Page::cursor_x = 0;
uint8_t Page::cursor_y = 0;
uint8_t Page::marker_pos = 0;
char Page::line_buff[20];
Page *pages_temp[NUM_PAGES];
Page **pages = pages_temp;
Page **Page::pages = pages;
Page::Page(Page **pages, char *name){
  this->pages = pages;
  this->name = name;
}
void Page::draw() {
  u8g.drawFrame(0, 0, u8g.getWidth(), HEADER_FRAME_HEIGHT);
  this->draw_header();
  this->draw_content();
}
void Page::update() {
  if (in_page && !selected) {
    marker_pos = (((encoderPos - old_encoder_val) % line_count) + line_count) % line_count;
  }
  if (in_page && selected){
    state = (encoderPos - old_encoder_val) - marker_pos;
  }
  
  this->display_page();
}
void Page::draw_header() {
  u8g.drawStr((HEADER_FRAME_HEIGHT - strlen(name)) * 8 / 2, 3 + u8g.getFontAscent(), name);
  cursor_y = 16;
  cursor_x = 1;
}

void Page::display_page() {
  u8g.firstPage();
  do {
    pind = PIND;
    draw();
  } while ( u8g.nextPage() );
}

void Page::enter_page() {
  in_page = true;
  selected = false;
  old_encoder_val = encoderPos;
}
void Page::leave_page() {
  in_page = false;
  selected = false;
  encoderPos = old_encoder_val;
}

void Page::enter_selection(){
  selected = true;
  old_selection_encoder_val = encoderPos;
}

void Page::leave_selection(){
  selected = false;
  encoderPos = old_selection_encoder_val;
}

void Page::drawStrCursor(char *line_buff, uint8_t i) {
  cursor_y += + 2 + u8g.getFontAscent();
  if (i == marker_pos && in_page) {
    if (selected)
      strcpy(line_buff + strlen(line_buff), "<");
    else
      strcpy(line_buff + strlen(line_buff), "-");
  }
  
  u8g.drawStr(cursor_x, cursor_y, line_buff);
}

StartPage::StartPage(Page **pages): Page(pages, "StartPage") {
  this->line_count = NUM_PAGES;
}

void StartPage::update() {
  if (this->selected) {
    this->selected = false;
    this->in_page = false;
    this->remote->in_page = false;
    this->remote->set_page(marker_pos);
    super::leave_page();
    encoderPos += marker_pos;
    this->marker_pos = 0;
    return;
  }
  super::update();
}

void StartPage::draw_content() {
  byte i = 0;
  if (marker_pos >= 2)
    i = (marker_pos - 2);
  for (; i < min(NUM_PAGES, MAX_LINES + i); i++) {
    strcpy(line_buff, pages[i]->name);
    drawStrCursor(line_buff, i);
  }
}

GimbalControlPage::GimbalControlPage(Page **pages): Page(pages, "Gimbal Control") {
  this->line_count = 3;
}

void GimbalControlPage::check_rf_link(){
  this->rf_link_state = this->remote->radio->get_link_state();

}

void GimbalControlPage::update() {
  if (this->selected)
    this->axes[marker_pos] = analogRead(A7);
  this->check_rf_link();
  super::update();
}


void GimbalControlPage::draw_content() {
  strcpy(line_buff, "X Axis: ");
  itoa(axes[0], line_buff + 8, 10);
  drawStrCursor(line_buff, 0);

  strcpy(line_buff, "Y Axis: ");
  itoa(axes[1], line_buff + 8, 10);
  drawStrCursor(line_buff, 1);

  strcpy(line_buff, "Z Axis: ");
  itoa(axes[2], line_buff + 8, 10);
  drawStrCursor(line_buff, 2);
}


MQTTControlPage::MQTTControlPage(Page **pages): Page(pages, "MQTT Control") {
  line_count = 2;
}

void MQTTControlPage::update() {
  if (this->selected) {
    
  }
  super::update();
}


void MQTTControlPage::draw_content() {
  strcpy(line_buff, "TV Control-ON");
  if (marker_pos == 0)
    set_state(line_buff +11);
  super::drawStrCursor(line_buff, 0);
  //u8g.drawStr(cursor_x, cursor_y, buff);
  strcpy(line_buff, "AC Control-OFF");
  if (marker_pos == 1)
    set_state(line_buff+11);
  super::drawStrCursor(line_buff, 1);
}
void MQTTControlPage::set_state(char *buff){
  if (!selected)
    return;
  if (((state % 2) + 2) % 2 == 0)
    strcpy(buff, "OFF");
  if (((state % 2) + 2) % 2== 1)
    strcpy(buff, "ON");
}

void setup_pages(){
  u8g.setFont(u8g_font_8x13B);
  pages[0] = new StartPage(pages);
  pages[1] = new GimbalControlPage(pages);
  pages[2] = new MQTTControlPage(pages);
  pages[3] = new GimbalControlPage(pages);
}

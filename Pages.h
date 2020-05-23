#ifndef PAGES_H
#define PAGES_H
#include <stdint.h>

#include <stdio.h>
#include <Arduino.h>
#include "Arduino.h"
#include "UniversalRemote.h"
class UniversalRemote;
#define NUM_PAGES 6
#define HEADER_FRAME_HEIGHT 16
#define MAX_LINES 4

class Page {
  public:
    static char old_encoder_val;
    static char old_selection_encoder_val;
    char *name;
    static bool selected;
    static Page **pages;
    static bool in_page;
    static int state;
    uint8_t line_count;
    static uint8_t cursor_x;
    static uint8_t cursor_y;
    static uint8_t marker_pos;
    static char line_buff[20];
    static UniversalRemote *remote;

    Page(Page **pages, char *name);
    virtual ~Page(){}
    virtual void draw();
    virtual void update();
    void draw_header();
    virtual void draw_content() = 0;
    virtual void display_page();
    virtual void enter_page();
    virtual void leave_page();
    virtual void enter_selection();
    virtual void leave_selection();
    void drawStrCursor(char *line_buff, uint8_t i);
};





class StartPage: public Page {
  public:
    StartPage(Page **pages);
    void update();
    void draw_content();
  private:
    typedef Page super;
};

class GimbalControlPage: public Page {
  public:
    void update();
    GimbalControlPage(Page **pages);
    void draw_content();
    void check_rf_link();
  private:
    bool rf_link_state;
    uint16_t axes[3];
    typedef Page super;
};


class MQTTControlPage: public Page {
  public:
    void update();
    void set_state(char *buff);
    MQTTControlPage(Page **pages);
    void draw_content();
  private:
    typedef Page super;
};

#endif

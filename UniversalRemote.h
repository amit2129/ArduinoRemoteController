#ifndef UNIVERSAL_REMOTE_H
#define UNIVERSAL_REMOTE_H
#include <stdint.h>
#include <string.h>
#include "Pages.h"
#include "rf.h"


class Page;
class RF;


class UniversalRemote {
  public:
    bool in_page = false;
    uint8_t page_number = 0;
    static Page **pages;
    static RF *radio;
    
    UniversalRemote(Page **pages_t);
    void set_page(uint8_t page_num);
    void update();
    void display_page();
    void enter_page();
    void leave_page();
};
#endif

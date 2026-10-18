#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <tonc.h>

class Button
{
  public:
    Button();
    Button(OBJ_ATTR *L, OBJ_ATTR *R, int nLeft_button_width); // wide buttons
    Button(OBJ_ATTR *B);                                      // short button
    bool isWide;
    void set_location(int x, int y);
    void set_highlight(bool highlight);
    void hide();
    void show();
    int x;
    int y;

  private:
    int left_button_width;
    bool highlighted;
    OBJ_ATTR *button_L;
    OBJ_ATTR *button_R;
};

#endif
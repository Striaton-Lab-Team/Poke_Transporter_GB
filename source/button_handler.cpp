#include "button_handler.h"
#include "sprite_data.h"

Button::Button() {}

Button::Button(OBJ_ATTR *L, OBJ_ATTR *R, int nLeft_button_width)
{
    button_L = L;
    button_R = R;
    isWide = true;
    left_button_width = nLeft_button_width;
    hide();
}

Button::Button(OBJ_ATTR *B)
{
    button_L = B;
    isWide = false;
    hide();
}

void Button::set_location(int nx, int ny)
{
    x = nx;
    y = ny;
    obj_set_pos(button_L, x, y);
    if (isWide)
    {
        obj_set_pos(button_R, x + left_button_width, y);
    }
}
void Button::set_highlight(bool highlight)
{
    button_L->attr2 &= ~ATTR2_PALBANK_MASK;
    button_L->attr2 |= ATTR2_PALBANK(highlight ? BTN_LIT_PAL : BTN_PAL);

    if (isWide)
    {
        button_R->attr2 &= ~ATTR2_PALBANK_MASK;
        button_R->attr2 |= ATTR2_PALBANK(highlight ? BTN_LIT_PAL : BTN_PAL);
    }
}
void Button::hide()
{
    if (isWide)
    {
        obj_hide_multi(button_L, 2);
    }
    else
    {
        obj_hide(button_L);
    }
}
void Button::show()
{
    if (isWide)
    {
        obj_unhide_multi(button_L, 0, 2);
    }
    else
    {
        obj_unhide(button_L, 0);
    }
}
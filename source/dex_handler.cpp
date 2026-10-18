#include "dex_handler.h"

Dex::Dex() {}

Dex::Dex(OBJ_ATTR *L, OBJ_ATTR *M, OBJ_ATTR *R)
{
    dex_L = L;
    dex_M = M;
    dex_R = R;
    hide();
}
void Dex::set_location(int nx, int ny)
{
    x = nx;
    y = ny;
    obj_set_pos(dex_L, x, y);
    obj_set_pos(dex_M, x + 64, y);
    obj_set_pos(dex_R, x + 128, y);
}
void Dex::hide()
{
    obj_hide(dex_L);
    obj_hide(dex_M);
    obj_hide(dex_R);
}
void Dex::show()
{
    obj_unhide(dex_L, 0);
    obj_unhide(dex_M, 0);
    obj_unhide(dex_R, 0);
}
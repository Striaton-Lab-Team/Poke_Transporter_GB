#ifndef DEX_HANDLER_H
#define DEX_HANDLER_H

#include <tonc.h>

class Dex
{
  public:
    Dex();
    Dex(OBJ_ATTR *L, OBJ_ATTR *M, OBJ_ATTR *R);
    void set_location(int x, int y);
    void hide();
    void show();

  private:
    int x;
    int y;
    bool highlighted;
    OBJ_ATTR *dex_L;
    OBJ_ATTR *dex_M;
    OBJ_ATTR *dex_R;
};

#endif
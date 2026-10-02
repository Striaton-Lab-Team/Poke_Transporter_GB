#ifndef TEXT_ENGINE_H
#define TEXT_ENGINE_H

#include "script_obj.h"
#include <cstdarg>

#define H_MAX 240
#define V_MAX 160

#define INK_WHITE 15
#define INK_ROM_COLOR 14
#define INK_DARK_GREY 13

#define SCRIPT_DEBUG 0 
#define SCRIPT_TRANSFER 1
#define SCRIPT_EVENT 2

void init_text_engine();
int text_loop(int script);
int text_next_obj_id(script_obj current_line);
void set_text_exit();
int ptgb_write_textbox(const byte *text, bool instant, bool waitForUser, int text_section, int text_key, bool eraseMainBox);
int ptgb_write_simple(const byte *text, bool instant);
int ptgb_write(const byte *text, bool instant, int length, int box_type);
int ptgb_write_debug(const u16* charset, const char *text, bool instant);
void wait_for_user_to_continue(int right, int bottom);
void scroll_text(bool instant, TTC *tc, bool scrollUp, int left, int top, int right, int bottom);

#endif
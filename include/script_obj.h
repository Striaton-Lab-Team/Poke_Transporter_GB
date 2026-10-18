#ifndef SCRIPT_OBJ_H
#define SCRIPT_OBJ_H

#include <tonc.h>
typedef struct
{
    u16 text_entry_index;
    u16 conditional_index;
    u16 next_if_true;
    u16 next_if_false;
} script_obj_params;

class script_obj
{
  public:
    script_obj();
    script_obj(const script_obj_params &params);

    bool has_text() const;
    u8 get_text_entry_index() const;
    u16 get_true_index() const;
    u16 get_false_index() const;
    u16 get_cond_id() const;

  private:
    script_obj_params params_;
};

#endif
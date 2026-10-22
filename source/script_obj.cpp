#include "script_obj.h"

script_obj::script_obj() : params_({0}) {};

script_obj::script_obj(const script_obj_params &params) : params_(params) {}

bool script_obj::has_text() const
{
    // So the thing is: when conditional_index is set, its value will always be higher than 0
    // because the way these integer defines have been set up.
    // So that means that if conditional_index IS 0, then we must have a text entry index defined
    // instead! We can't base this check on the text_entry_index itself since unfortunately its
    // value being zero can be legit.
    return (params_.conditional_index == 0);
}

u8 script_obj::get_text_entry_index() const { return params_.text_entry_index; }

u16 script_obj::get_true_index() const { return params_.next_if_true; }

u16 script_obj::get_false_index() const { return params_.next_if_false; }

u16 script_obj::get_cond_id() const { return params_.conditional_index; }
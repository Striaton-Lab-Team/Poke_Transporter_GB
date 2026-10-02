#ifndef GLOBAL_FRAME_CONTROLLER_H
#define GLOBAL_FRAME_CONTROLLER_H

#include <cstring>

enum LinkAnimationState
{
    state_off,
    state_connection,
    state_transfer,
    state_no_animation,
};

// TODO: This is kinda a silly spot for this
// make sure outBuffer is large enough! Should be at least hex_len + 1
template <typename I>
void n2hexstr(char *outBuffer, I w, size_t hex_len = sizeof(I) << 1)
{
    static const char *digits = "0123456789ABCDEF";
    memset(outBuffer, '0', hex_len);
    outBuffer[hex_len] = '\0'; // we must make sure to terminate the string

    for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
        outBuffer[i] = digits[(w >> j) & 0x0f];
}
void global_next_frame();
int get_frame_count();
void set_menu_sprite_pal(int frame);
void run_link_cable_animation(int frame);
void link_animation_state(LinkAnimationState state);
void determine_fennel_blink();
void set_missingno(bool val);
bool get_missingno_enabled();
void set_treecko(bool val);
bool get_treecko_enabled();
u32 fnv1a_hash(unsigned char *data, size_t length);
int get_string_length(const byte *str);
int get_string_char_count(const byte *str);
void convert_int_to_ptgb_str(int val, byte str[]);
void convert_int_to_ptgb_str(int val, byte str[], int min_length);

#endif

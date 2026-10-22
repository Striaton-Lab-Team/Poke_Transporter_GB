#include <cstdarg>
#include <cstdlib>
#include <ctype.h>
#include <errno.h>

#include <sys/iosupport.h>

// Okay, let's go over the basics of this file for a moment.
// By default, a lot of code is getting pulled in by the linker from static libraries such as
// newlibc and libtonc. The problem is that everytime a single symbol/function is needed from one of
// the .o files inside one of these static libs, the entire .o file is getting pulled in, alongside
// all of ITS dependencies. This cascades pretty quickly. So this pulls in a lot of unnecessary
// code, but also (global) variables. To solve that, we can try to override these functions by
// defining a custom version of them here. But to avoid still pulling in the .o file, we have to
// define every symbol that could be referenced somehow in either our code or the static libraries
// themselves. Worst case, this means we have to redefine every single function that is defined in
// that .o file. But doing so, means we can modify the code slightly to avoid pulling in a lot of
// bloat. we don't need.

// HACK: Avoid pulling in a large handles buffer, which we don't need. Only define the minimum.

#define MAX_HANDLES 3

static __handle __stdin_handle = {0, 1, NULL};
static __handle __stdout_handle = {1, 1, NULL};
static __handle __stderr_handle = {2, 1, NULL};

static __handle *handles[MAX_HANDLES] = {&__stdin_handle, &__stdout_handle, &__stderr_handle};

extern "C"
{
    // HACK:
    // Unfortunately, libtonc references siscanf, which in turn causes a "lot" of binary bloat from
    // newlibc (in regards to locale support) to be pulled in by the linker. However, if you look at
    // what it is actually used for (in libtonc's tte_iohook.c), is just to extract 1 or 2 integers
    // from a string by specifying a custom -extremely simplified- version, we can avoid pulling in
    // the version from libc (alongside all the symbols IT references) Obviously it doesn't support
    // everything it should. Just enough to support libtonc's current iteration.
    //
    // Anyway, doing this optimizes away a lot of scanf related functions from newlib and locale
    // support among which a 13KB "categories" symbol.
    int siscanf(const char *str, const char *format, ...)
    {
        bool expectingFormatSpecifier = false;
        va_list args;
        int *resultVal;
        int ret = 0;
        va_start(args, format);

        while (*format != '\0')
        {
            if (*str == '\0')
            {
                // EOF encountered.
                return -1;
            }

            if (expectingFormatSpecifier)
            {
                if (*format == 'd')
                {
                    resultVal = va_arg(args, int *);
                    (*resultVal) = 0;
                    while (isdigit(*str))
                    {
                        (*resultVal) *= 10;
                        (*resultVal) += (*str) - '0';
                        ++str;
                    }
                    // go back to the last character of the int, because we'll forward by one again
                    // at the end of the outer loop
                    --str;
                    ++ret;
                }
                expectingFormatSpecifier = false;
            }
            else if ((*format) == '%')
            {
                expectingFormatSpecifier = true;
            }

            ++format;
            ++str;
        }

        va_end(args);
        return ret;
    }

    // HACK: stub the libsysbase_libsysbase_a-handle_manager.o functions
    // this, alongside the -Wl,--wrap linker options ensures that this object file
    // and its 4KB IWRAM "handles" buffer is not getting pulled in by the linker.
    // We don't need it and we could use the 4KB of extra IWRAM!
    __handle *__wrap___get_handle(int fd)
    {
        if (fd < 0 || fd >= MAX_HANDLES)
            return NULL;

        return handles[fd];
    }

    int __wrap__close_r(struct _reent *ptr, int fd) { return 0; }

// HACK: The functions below were defined here to avoid pulling in the verdana9 font (4 KB) into the
// rom, which we're not using. The original function in libtonc would use verdana9 as a default if
// the specified font was NULL.
#include <string.h>
#include <tonc_bios.h>
#include <tonc_tte.h>
#include <tonc_video.h>

    //! Initialize text system for 4bpp tiled, column-major surfaces.
    /*!
        \param bgnr		Background number.
        \param bgcnt	Background control flags.
        \param se0		Base offset for screen-entries.
        \param cattrs	Color attributes; one byte per attr.
        \param clrs		ink(/shadow) colors.
        \param font		Font to initialize with.
        \param proc		Glyph renderer
    */
    void tte_init_chr4c(
        int bgnr, u16 bgcnt, u16 se0, u32 cattrs, u32 clrs, const TFont *font, fnDrawg proc)
    {
        if (proc == NULL)
            proc = chr4c_drawg_default;

        tte_init_base(font, proc, chr4c_erase);

        TTC *tc = tte_get_context();
        TSurface *srf = &tc->dst;

        srf_init(srf, SRF_CHR4C, &tile_mem[BFN_GET(bgcnt, BG_CBB)][se0 & SE_ID_MASK], SCREEN_WIDTH,
            SCREEN_HEIGHT, 4, pal_bg_mem);
        schr4c_prep_map(srf, se_mem[BFN_GET(bgcnt, BG_SBB)], se0);

        tc->flags0 = bgnr;
        tc->ctrl = bgcnt;
        REG_BGCNT[bgnr] = bgcnt;

        // --- Init color attributes ---
        u32 ink, shadow, paper; // spec;
        ink = cattrs & 15;
        shadow = (cattrs >> 8) & 15;
        paper = (cattrs >> 16) & 15;
        // spec=   (cattrs>>24)&255;

        tc->cattr[TTE_INK] = ink;
        tc->cattr[TTE_SHADOW] = shadow;
        tc->cattr[TTE_PAPER] = paper;
        // tc->cattr[TTE_SPECIAL]= 0;

        srf->palData = pal_bg_bank[se0 >> 12];
        srf->palData[ink] = clrs & 0xFFFF;
        srf->palData[shadow] = clrs >> 16;
    }

    void chr4c_erase(int left, int top, int right, int bottom)
    {
        TTC *tc = tte_get_context();

        // # Check for single sequence
        schr4c_rect(&tc->dst, left, top, right, bottom, tc->cattr[TTE_PAPER]);
    }

    // HACK: We need to replace the entirety of tte_main.c in libtonc to avoid pulling in sys8font
    // (because it's used as a default in tte_init_base() if the specified font is NULL) So here's a
    // modified version of tte_main.c (only tte_init_base() is really changed though):
    //////////////////////// tte_main.c ////////////////////////

    // --------------------------------------------------------------------
    // CONSTANTS
    // --------------------------------------------------------------------

    // --------------------------------------------------------------------
    // GLOBALS
    // --------------------------------------------------------------------

    TTC __tte_main_context;
    TTC *gp_tte_context = &__tte_main_context;

    // --------------------------------------------------------------------
    // INLINES
    // --------------------------------------------------------------------

    INLINE char *eatwhite(const char *str)
    {
        while (isspace(*str))
            str++;

        return (char *)str;
    }

    // --------------------------------------------------------------------
    // OPERATIONS
    // --------------------------------------------------------------------

    //! Set the master context pointer.
    void tte_set_context(TTC *tc) { gp_tte_context = tc ? tc : &__tte_main_context; }

    //! Set color attribute of \a type to \a cattr.
    void tte_set_color_attr(eint type, u16 cattr)
    {
        TTC *tc = tte_get_context();
        tc->cattr[type] = cattr;
    }

    //! Load important color attribute data.
    void tte_set_color_attrs(const u16 cattrs[])
    {
        int ii;
        TTC *tc = tte_get_context();

        for (ii = 0; ii < 4; ii++)
            tc->cattr[ii] = cattrs[ii];
    }

    //! Set color attribute of \a type to \a cattr.
    void tte_set_color(eint type, u16 color)
    {
        TTC *tc = tte_get_context();

        if (tc->dst.palData != NULL)
            tc->dst.palData[tc->cattr[type]] = color;
        else
            tc->cattr[type] = color;
    }

    //! Load important color data.
    void tte_set_colors(const u16 colors[])
    {
        int ii;
        TTC *tc = tte_get_context();

        if (tc->dst.palData != NULL)
            for (ii = 0; ii < 4; ii++)
                tc->dst.palData[tc->cattr[ii]] = colors[ii];
        else
            for (ii = 0; ii < 4; ii++)
                tc->cattr[ii] = colors[ii];
    }

    //! Base initializer of a TTC.
    void tte_init_base(const TFont *font, fnDrawg drawProc, fnErase eraseProc)
    {
        if (tte_get_context() == NULL)
            tte_set_context(&__tte_main_context);

        TTC *tc = tte_get_context();
        memset(tc, 0, sizeof(TTC));

        tc->font = (TFont *)font;
        tc->drawgProc = drawProc;
        tc->eraseProc = eraseProc;

        // Default is SBB 0
        const TSurface srf = {(u8 *)se_mem, 32 * 2, 32, 32, 8, SRF_BMP16, 256, pal_bg_mem};
        tc->dst = srf;

        tc->cattr[TTE_INK] = 0xF1;
        tc->cattr[TTE_SHADOW] = 0xF2;
        tc->cattr[TTE_PAPER] = 0;
        tc->cattr[TTE_SPECIAL] = 0;

        tc->marginRight = SCREEN_WIDTH;
        tc->marginBottom = SCREEN_HEIGHT;
    }

    // --------------------------------------------------------------------
    // String interpretations
    // --------------------------------------------------------------------

    //! Retrieve a single multibyte utf8 character.
    uint utf8_decode_char(const char *ptr, char **endptr)
    {
        uchar *src = (uchar *)ptr;
        uint ch8, ch32;

        // Poor man's try-catch.
        do
        {
            // UTF8 formats:
            // 0aaaaaaa                            ->                   0aaaaaaa
            // 110aaaaa 10bbbbbb                   ->          00000aaa aabbbbbb
            // 1110aaaa 10bbbbbb 10cccccc          ->          aaaabbbb bbcccccc
            // 11110aaa 10bbbbbb 10cccccc 10dddddd -> 000aaabb bbbbcccc ccdddddd
            //
            // Any invalid format will be returned as a single byte.

            ch8 = *src;
            if (ch8 < 0x80) // 7b
            {
                ch32 = ch8;
            }
            else if (0xC0 <= ch8 && ch8 < 0xE0) // 11b
            {
                ch32 = (*src++ & 0x1F) << 6;
                if ((*src >> 6) != 2)
                    break;
                ch32 |= (*src++ & 0x3F) << 0;
            }
            else if (0xE0 <= ch8 && ch8 < 0xF0) // 16b
            {
                ch32 = (*src++ & 0x0F) << 12;
                if ((*src >> 6) != 2)
                    break;
                ch32 |= (*src++ & 0x3F) << 6;
                if ((*src >> 6) != 2)
                    break;
                ch32 |= (*src++ & 0x3F) << 0;
            }
            else if (0xF0 <= ch8 && ch8 < 0xF8) // 21b
            {
                ch32 = (*src++ & 0x0F) << 18;
                if ((*src >> 6) != 2)
                    break;
                ch32 |= (*src++ & 0x3F) << 12;
                if ((*src >> 6) != 2)
                    break;
                ch32 |= (*src++ & 0x3F) << 6;
                if ((*src >> 6) != 2)
                    break;
                ch32 |= (*src++ & 0x3F) << 0;
            }
            else
                break;

            // Proper UTF8 char: set endptr and return
            if (endptr)
                *endptr = (char *)src;

            return ch32;

        } while (0);

        // Not really UTF: interpret as single byte.
        src = (uchar *)ptr;
        ch32 = *src++;
        if (endptr)
            *endptr = (char *)src;

        return ch32;
    }

    //! Find the string-position after the command.
    /*!
        \param str	String to check.
        \return		The string-pointer after the current/next command.
            If there is no command-end, this moves to the end of the
            string.
    */
    char *tte_cmd_skip(const char *str)
    {
        int ch;

        while ((ch = *str) != '\0')
        {
            str++;
            if (ch == '}')
                break;
        }

        return (char *)str;
    }

    //! Move to the next command in a sequence.
    /*!
        \return	Position of EOS (\'0'), EOC ('}') or next cmd token (rest)
    */
    char *tte_cmd_next(const char *str)
    {
        int ch;

        // Find EOC, separator or NULL
        while (1)
        {
            ch = *str;
            if (ch == '\0' || ch == '}') // EOS/EOC
                return (char *)str;
            else if (ch == ';') // More commands
                break;

            str++;
        }

        // More commands: try to find next token (or EOS/EOC again)
        while (1)
        {
            ch = *++str;
            if (ch == '\0' || !isspace(ch))
                return (char *)str;
        }
    }

    //!	Text command handler.
    /*!	Takes commands formatted as "#{[cmd]:[opt];[[cmd]:[opt];...]} and
        deals with them.<br>
        <br>Command list:
        - <b>P</b>				Set cursor to margin top-left.
        - <b>Ps</b>				Save cursor position
        - <b>Pr</b>				Restore cursor position.
        - <b>P:\#x,\#y</b>		Set cursorX/Y to \e x, \e y.
        - <b>X</b>				Set cursorX to margin left.
        - <b>X:\#x</b>			Set cursorX to \e x.
        - <b>Y</b>				Set cursorY to margin top.
        - <b>Y:\#y</b>			Set cursorX to \e y.
        - <b>c[ispx]:\#val</b>	Set ink/shadow/paper/special color to \e val.
        - <b>e[slbfr]</b>		Erase screen/line/backward/forward/rect
        - <b>m:\#l,\#t,\#r,\#b</b>	Set all margins
        - <b>m[ltrb]:\#val</b>	Set margin to \e val.
        - <b>p:\#x,\#y</b>		Move cursorX/Y by \e x, \e y.
        - <b>w:\#val</b>		Wait \e val frame.
        - <b>x:\#x</b>			Move cursorX by \e x.
        - <b>y:\#y</b>			Move cursorX by \e y.

        Examples:<br>
        - <b>#{X:32}</b>		Move to \e x = 32;
        - <b>#{ci:0x7FFF}</b>	Set ink color to white.
        - <b>#{w:120;es;P}</b>	Wait 120 frames, clear screen, return to top of screen.
        \param str	Start of command. Assumes the initial "\{" is lobbed
            off already.
        \return pointer to after the parsed command.
        \note	Routine does text wrapping. Make sure margins are set.
        \note	This function involves heavy (yet necessary)
            switching. Leave your sanity at the door before viewing.
        \todo	Scrolling and variables ?
        \todo	Restructure for safety checks.
    */
    char *tte_cmd_default(const char *str)
    {
        int ch, val;
        char *curr = (char *)str, *next;

        TTC *tc = tte_get_context();

        while (1)
        {
            // --- Get cmd char and act on it ---
            // "Ew, double switches". Yes, I know. I'm not proud.
            // That behaviour may change later

            ch = *curr;
            next = curr + 1;

            switch (ch)
            {
            // --- Absolute Positions ---
            case 'X':
                tc->cursorX = curr[1] == ':' ? strtol(curr + 2, &next, 0) : tc->marginLeft;
                break;
            case 'Y':
                tc->cursorY = curr[1] == ':' ? strtol(curr + 2, &next, 0) : tc->marginTop;
                break;
            case 'P':
            {
                switch (curr[1])
                {
                case 's': // Save position
                    tc->savedX = tc->cursorX;
                    tc->savedY = tc->cursorY;
                    break;
                case 'r': // Restore position
                    tc->cursorX = tc->savedX;
                    tc->cursorY = tc->savedY;
                    break;
                case ':': // Set position
                    tc->cursorX = strtol(curr + 2, &next, 0);
                    curr = eatwhite(next);
                    if (curr[0] == ',')
                        tc->cursorY = strtol(curr + 1, &next, 0);
                    break;
                default: // Set to top-left of screen
                    tc->cursorX = tc->marginLeft;
                    tc->cursorY = tc->marginTop;
                }
            }
            break;

            // --- Relative Positions ---
            case 'x':
                tc->cursorX += strtol(curr + 2, &next, 0);
                break;
            case 'y':
                tc->cursorY += strtol(curr + 2, &next, 0);
                break;
            case 'p':
                tc->cursorX += strtol(curr + 2, &next, 0);
                curr = eatwhite(next);
                if (curr[0] == ',')
                    tc->cursorY += strtol(curr + 1, &next, 0);
                break;

            // --- Colors c[ispx]:# ---
            case 'c':
            {
                ch = curr[1]; // index character ("ispx")
                curr += 3;
                switch (ch)
                {
                case 'i':
                    tc->cattr[TTE_INK] = strtol(curr, &next, 0);
                    break;
                case 's':
                    tc->cattr[TTE_SHADOW] = strtol(curr, &next, 0);
                    break;
                case 'p':
                    tc->cattr[TTE_PAPER] = strtol(curr, &next, 0);
                    break;
                case 'x':
                    tc->cattr[TTE_SPECIAL] = strtol(curr, &next, 0);
                    break;
                }
                break;
            }

            // --- Erasing ---
            case 'e':
            {
                switch (curr[1])
                {
                case 's': // screen (within margins)
                    tte_erase_screen();
                    break;
                case 'l': // line (within margins)
                    tte_erase_line();
                    break;
                case 'f': // line up to cursorX
                    tte_erase_rect(
                        tc->cursorX, tc->cursorY, tc->marginRight, tc->cursorY + tc->font->charH);
                    break;
                case 'b': // line from cursorX
                    tte_erase_rect(
                        tc->marginLeft, tc->cursorY, tc->cursorX, tc->cursorY + tc->font->charH);
                    break;
                case 'r': // rectangle
                {
                    int rect[4];
                    curr += 3;
                    rect[0] = strtol(curr + 0, &next, 0);
                    curr = eatwhite(next);
                    if (curr[0] != ',')
                        break;

                    rect[1] = strtol(curr + 1, &next, 0);
                    curr = eatwhite(next);
                    if (curr[0] != ',')
                        break;

                    rect[2] = strtol(curr + 1, &next, 0);
                    curr = eatwhite(next);
                    if (curr[0] != ',')
                        break;

                    rect[3] = strtol(curr + 1, &next, 0);
                    tte_erase_rect(rect[0], rect[1], rect[2], rect[3]);
                    break;
                }
                    // # erase character / backspace.
                }
            }
            break;

            // --- Margins ---
            case 'm':
            {
                ch = curr[1]; // index character ("ispx")
                curr += 3;
                switch (ch)
                {
                case 'l':
                    tc->marginLeft = strtol(curr, &next, 0);
                    break;
                case 't':
                    tc->marginTop = strtol(curr, &next, 0);
                    break;
                case 'r':
                    tc->marginRight = strtol(curr, &next, 0);
                    break;
                case 'b':
                    tc->marginBottom = strtol(curr, &next, 0);
                    break;
                case 's': // To screen size
                    tc->marginLeft = 0;
                    tc->marginTop = 0;
                    tc->marginRight = SCREEN_WIDTH;
                    tc->marginBottom = SCREEN_HEIGHT;
                    break;
                case ':': // Set all margins
                    tc->marginLeft = strtol(curr - 1, &next, 0);
                    curr = eatwhite(next);
                    if (curr[0] != ',')
                        break;

                    tc->marginTop = strtol(curr + 1, &next, 0);
                    curr = eatwhite(next);
                    if (curr[0] != ',')
                        break;

                    tc->marginRight = strtol(curr + 1, &next, 0);
                    curr = eatwhite(next);
                    if (curr[0] != ',')
                        break;

                    tc->marginBottom = strtol(curr + 1, &next, 0);
                    break;
                }
            }
            break;

            // --- Font ---
            case 'f':
                val = strtol(curr + 2, &next, 0);
                if (tc->fontTable && tc->fontTable[val])
                    tc->font = (TFont *)tc->fontTable[val];
                break;

            // --- String ---
            case 's':
                val = strtol(curr + 2, &next, 0);
                if (tc->stringTable && tc->stringTable[val])
                    tte_write(tc->stringTable[val]);
                break;

            // --- Wait a few frames ---
            case 'w':
                val = strtol(curr + 2, &next, 0);
                VBlankIntrDelay(val);
                break;

            } // /main switch

            // Find EOS/EOC/token and act on it
            curr = tte_cmd_next(next);

            if (curr[0] == '\0')
                return curr;
            else if (curr[0] == '}')
                return curr + 1;
        }
    }

    //! Extended string writer, with positional and color info
    int tte_write_ex(int x0, int y0, const char *text, const u16 *cattrs)
    {
        TTC *tc = tte_get_context();
        tc->cursorX = x0;
        tc->cursorY = y0;

        if (cattrs)
            tte_set_color_attrs(cattrs);

        return tte_write(text);
    }

    // Generic TTE putc (to be tested later)
    // NOTE: the glyph inlines seem as fast as manual retrieval,
    //   even though the former get the TTC and font again as well.
    //	Yay.

    //! Plot a single character; does wrapping too.
    /*!
        \param ch	Character to plot (not glyph-id).
        \return		Character width.
        \note		Overhead: ~70 cycles.
    */
    int tte_putc(int ch)
    {
        TTC *tc = tte_get_context();
        TFont *font = tc->font;

        uint gid = tte_get_glyph_id(ch);
        int charW = tte_get_glyph_width(gid);

        if (tc->cursorX + charW > tc->marginRight)
        {
            tc->cursorY += font->charH;
            tc->cursorX = tc->marginLeft;
        }

        // Draw and update position
        tc->drawgProc(gid);
        tc->cursorX += charW;

        return charW;
    }

    //! Render a string.
    /*!
        \param text	String to parse and write.
        \return		Number of parsed characters.
    */
    int tte_write(const char *text)
    {
        if (text == NULL)
            return 0;

        uint ch, gid;
        char *str = (char *)text;
        TTC *tc = tte_get_context();
        TFont *font;

        while ((ch = *str) != '\0')
        {
            str++;
            switch (ch)
            {
            // --- Newline/carriage return ---
            case '\r':
                if (str[0] == '\n') // deal with CRLF pair
                    str++;
                // FALLTHRU
            case '\n':
                tc->cursorY += tc->font->charH;
                tc->cursorX = tc->marginLeft;
                break;
            // --- Tab ---
            case '\t':
                tc->cursorX = (tc->cursorX / TTE_TAB_WIDTH + 1) * TTE_TAB_WIDTH;
                break;

            // --- Normal char ---
            default:
                // Command sequence
                if (ch == '#' && str[0] == '{')
                {
                    str = tte_cmd_default(str + 1);
                    break;
                }
                // Escaped command: skip '\\' and print '#'
                else if (ch == '\\' && str[0] == '#')
                    ch = *str++;
                // Check for UTF8 code
                else if (ch >= 0x80)
                    ch = utf8_decode_char(str - 1, &str);

                // Get glyph index and call renderer
                font = tc->font;
                gid = ch - font->charOffset;
                if (tc->charLut)
                    gid = tc->charLut[gid];

                // Character wrap
                int charW = font->widths ? font->widths[gid] : font->charW;
                if (tc->cursorX + charW > tc->marginRight)
                {
                    tc->cursorY += font->charH;
                    tc->cursorX = tc->marginLeft;
                }

                // Draw and update position
                tc->drawgProc(gid);
                tc->cursorX += charW;
            }
        }

        // Return characters used (PONDER: is this really the right thing?)
        return str - text;
    }

    //! Erase a porttion of the screen (ignores margins)
    void tte_erase_rect(int left, int top, int right, int bottom)
    {
        TTC *tc = tte_get_context();

        if (tc->eraseProc)
            tc->eraseProc(left, top, right, bottom);
    }

    //! Erase the screen (within the margins).
    /*!
        \note Ponder: set paper color?
    */
    void tte_erase_screen()
    {
        TTC *tc = tte_get_context();

        if (tc->eraseProc)
            tc->eraseProc(tc->marginLeft, tc->marginTop, tc->marginRight, tc->marginBottom);
    }

    //! Erase the whole line (within the margins).
    /*!
        \note Ponder: set paper color?
    */
    void tte_erase_line()
    {
        TTC *tc = tte_get_context();
        int height = tc->font->charH;

        if (tc->eraseProc)
            tc->eraseProc(tc->marginLeft, tc->cursorY, tc->marginRight, tc->cursorY + height);
    }

    //! Get the size taken up by a string.
    /*!
        \param str	String to check.
        \return	width and height, packed into a POINT16.
        \note	This function \e ignores tte commands, so don't use
            on strings that use commands.
    */
    POINT16 tte_get_text_size(const char *str)
    {
        TTC *tc = tte_get_context();

        int charW, charH = tc->font->charH;

        s16 x = 0, width = 12, height = charH;
        int ch;

        while ((ch = *str++) != 0)
        {
            switch (ch)
            {
            // --- Newline/carriage return ---
            case '\r':
                if (str[0] == '\n') // deal with CRLF pair
                    str++;
                // FALLTHRU
            case '\n':
                height += charH;
                if (x > width)
                    width = x;
                x = 0;
                break;

            // --- Special char ---
            case '\\':
                // # Use cmd-functino
                // # Take care of positioning commands.
                if (str[0] == '{')
                    str = tte_cmd_skip(str);
                break;

            // --- Normal char ---
            default:
                charW = tc->font->cellW;
                if (x + charW > tc->marginRight)
                {
                    height += charH;
                    if (x > width)
                        width = x;
                    x = 0;
                }
                else
                    x += tte_get_glyph_width(tte_get_glyph_id(ch));
            }
        }

        // One more to make sure we got it >_<
        if (x > width)
            width = x;

        POINT16 pt = {width, height};
        return pt;
    }

    void tte_set_margins(int left, int top, int right, int bottom)
    {
        TTC *tc = tte_get_context();

        tc->marginLeft = left;
        tc->marginTop = top;
        tc->marginRight = right;
        tc->marginBottom = bottom;
    }

    // EOF
    ////////////////////// end tte_main.c //////////////////////
}
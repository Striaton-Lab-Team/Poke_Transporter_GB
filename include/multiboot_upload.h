#ifndef MULTIBOOT_UPLOAD_H
#define MULTIBOOT_UPLOAD_H

/**
 * @brief Shows the multiboot upload screen.
 * If the user presses A in this screen/popup,
 * the GBA will upload the whole EWRAM as a multiboot rom to another GBA over the link cable.
 */
void multiboot_upload_screen();

#endif
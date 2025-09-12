#ifndef SOKOL_BITMAP_H
#define SOKOL_BITMAP_H

#include "sokol_gfx.h"
#include "sokol_gp.h"

typedef struct sbm_desc {
  sg_image img;                   // Sokol GFX image

  uint32_t img_width_pixels;      // Image total width
  uint32_t img_height_pixels;     // Image total height
  uint32_t img_padding_x_pixels;  // Image outer padding left
  uint32_t img_padding_y_pixels;  // Image outer padding top

  uint32_t char_width_pixels;     // Character width
  uint32_t char_height_pixels;    // Character height
  uint32_t char_padding_x_pixels; // Character padding right
  uint32_t char_padding_y_pixels; // Character padding bottom

  const char *chars;              // Characters in font image
  size_t num_chars;               // Number of characters
} sbm_desc;

typedef struct sbm_font {
  sbm_desc desc;

  uint32_t img_width_chars;
  uint32_t img_height_chars;

  size_t *char_jump_tbl;
  size_t max_val;
} sbm_font;

typedef struct sbm_draw_opts {
  const char *string;             // String to draw
  size_t string_len;              // Number of characters in string

  uint32_t font_size;             // Font size

  float x;                        // X coordinate (to top left of string)
  float y;                        // Y coordinate (to top left of string)
  float gap;                      // Character gap
} sbm_draw_opts;

/**
 * Initializes a bitmap font with the given description.
 * @param self Pointer to the font object to initialize.
 * @param desc Font description containing image and character info.
 * @returns true on success, false on failure.
 */
bool sbm_init_font(sbm_font *self, sbm_desc desc);

/**
 * Frees resources associated with a bitmap font.
 * @param self Pointer to the font object to free.
 */
void sbm_free_font(sbm_font *self);

/**
 * Draws a single character using the bitmap font.
 * @param self Pointer to the font object.
 * @param c Character to draw.
 * @param r Rectangle specifying position and size.
 */
void sbm_draw_char(sbm_font *self, char c, sgp_rect r);

/**
 * Draws a string using the bitmap font and drawing options.
 * @param self Pointer to the font object.
 * @param opts Drawing options including string, position, and font size.
 */
void sbm_draw_string(sbm_font *self, sbm_draw_opts opts);

/**
 * Measures the width of a string when rendered with the bitmap font and options.
 * @param self Pointer to the font object.
 * @param opts Drawing options including string, font size, and gap.
 * @returns The width of the rendered string in pixels.
 */
float sbm_measure_string(sbm_font *self, sbm_draw_opts opts);

#endif

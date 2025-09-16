#ifndef SOKOL_BITMAP_H
#define SOKOL_BITMAP_H

#ifndef SOKOL_GP_INCLUDED
#error "Please include sokol_gp.h before sokol_bitmap.h"
#endif

// ╔══════════════════╗
// ║ TYPE DEFINITIONS ║
// ╚══════════════════╝

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

// ╔═══════════════════╗
// ║ API DOCUMENTATION ║
// ╚═══════════════════╝

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

#endif // SOKOL_BITMAP_H

// ╔════════════════╗
// ║ IMPLEMENTATION ║
// ╚════════════════╝

#ifdef SOKOL_BITMAP_IMPL
#ifndef SOKOL_BITMAP_IMPL_INCLUDED
#define SOKOL_BITMAP_IMPL_INCLUDED

#include <stdlib.h>

static size_t find_max(const char *chars, size_t num_chars) {
  if (num_chars == 0) {
    return 0;
  }

  size_t max_val = chars[0];
  for (size_t i = 0; i < num_chars; i++) {
    if (chars[i] > max_val) {
      max_val = chars[i];
    }
  }

  return max_val;
}

static bool build_char_jump_tbl(size_t *tbl, const char *chars, size_t num_chars) {
  if (!tbl || !chars) {
    return false;
  }

  for (size_t i = 0; i < num_chars; i++) {
    tbl[(size_t)chars[i]] = i;
  }

  return true;
}

bool sbm_init_font(sbm_font *self, sbm_desc desc) {
  if (!self) {
    return false;
  }

  self->desc = desc;

  self->img_width_chars = desc.img_width_pixels /
                          (desc.char_width_pixels + desc.char_padding_x_pixels);
  self->img_height_chars = desc.img_height_pixels /
                           (desc.char_height_pixels + desc.char_padding_y_pixels);

  size_t max_val = find_max(desc.chars, desc.num_chars);
  size_t *tbl = malloc((max_val + 1) * sizeof(size_t));
  if (!tbl) {
    return false;
  }

  if (!build_char_jump_tbl(tbl, desc.chars, desc.num_chars)) {
    free(tbl);
    return false;
  }

  self->max_val = max_val;
  self->char_jump_tbl = tbl;
  return true;
}

void sbm_free_font(sbm_font *self) {
  if (self && self->char_jump_tbl) {
    free(self->char_jump_tbl);
    self->char_jump_tbl = NULL;
  }
}

void sbm_draw_char(sbm_font *self, char c, sgp_rect r) {
  if (!self || (size_t)c > self->max_val) {
    return;
  }

  size_t index = self->char_jump_tbl[(size_t)c];

  uint32_t x_tiles = index % self->img_width_chars;
  uint32_t y_tiles = index / self->img_width_chars;

  uint32_t x_pixels = self->desc.img_padding_x_pixels +
                      x_tiles * (self->desc.char_width_pixels + self->desc.char_padding_x_pixels);
  uint32_t y_pixels = self->desc.img_padding_y_pixels +
                      y_tiles * (self->desc.char_height_pixels + self->desc.char_padding_y_pixels);

  sgp_set_image(0, self->desc.img);
  sgp_set_blend_mode(SGP_BLENDMODE_ADD);
  sgp_draw_textured_rect(0, r, (sgp_rect) {
    .x = x_pixels,
    .y = y_pixels,
    .w = self->desc.char_width_pixels,
    .h = self->desc.char_height_pixels,
  });
  sgp_reset_image(0);
}

void sbm_draw_string(sbm_font *self, sbm_draw_opts opts) {
  float w = opts.font_size * self->desc.char_width_pixels / self->desc.char_height_pixels;

  for (size_t i = 0; i < opts.string_len; i++) {
    sbm_draw_char(self, opts.string[i], (sgp_rect) {
      .x = opts.x + (i * (w + opts.gap)),
      .y = opts.y,
      .w = w,
      .h = opts.font_size,
    });
  }
}

float sbm_measure_string(sbm_font *self, sbm_draw_opts opts) {
  float w = opts.font_size * self->desc.char_width_pixels / self->desc.char_height_pixels;
  float length = opts.string_len * (w + opts.gap) - opts.gap;

  return length;
}

#endif // SOKOL_BITMAP_IMPL_INCLUDED
#endif // SOKOL_BITMAP_IMPL

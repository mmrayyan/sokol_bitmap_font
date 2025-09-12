#ifndef SOKOL_BITMAP_H
#define SOKOL_BITMAP_H

#include "sokol_gfx.h"
#include "sokol_gp.h"

typedef struct bitmap_desc {
  sg_image img;

  uint32_t img_width_pixels;
  uint32_t img_height_pixels;
  uint32_t img_padding_x_pixels;
  uint32_t img_padding_y_pixels;
  uint32_t char_width_pixels;
  uint32_t char_height_pixels;
  uint32_t char_padding_x_pixels;
  uint32_t char_padding_y_pixels;

  const char *chars;
  size_t num_chars;
} bitmap_desc;

typedef struct bitmap_font {
  bitmap_desc desc;

  uint32_t img_width_chars;
  uint32_t img_height_chars;

  size_t *char_jump_tbl;
  size_t max_val;
} bitmap_font;

bool bitmap_init(bitmap_font *self, bitmap_desc desc);
void bitmap_free(bitmap_font *self);

void bitmap_draw_char(bitmap_font *self, char c, sgp_rect r);
void bitmap_draw_line(bitmap_font *self, const char *s, float gap, sgp_rect r);

#endif

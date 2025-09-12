#ifndef SOKOL_BITMAP_H
#define SOKOL_BITMAP_H

#include "sokol_gfx.h"
#include "sokol_gp.h"

typedef struct sbm_desc {
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
} sbm_desc;

typedef struct sbm_font {
  sbm_desc desc;

  uint32_t img_width_chars;
  uint32_t img_height_chars;

  size_t *char_jump_tbl;
  size_t max_val;
} sbm_font;

bool sbm_init_font(sbm_font *self, sbm_desc desc);
void sbm_free_font(sbm_font *self);

void sbm_draw_char(sbm_font *self, char c, sgp_rect r);
void sbm_draw_string(sbm_font *self, const char *s, float gap, sgp_rect r);

#endif

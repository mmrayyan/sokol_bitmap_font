#include "sokol_bitmap.h"

#include <stdio.h>
#include <stdlib.h>

size_t find_max(const char *chars, size_t num_chars) {
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

bool build_char_jump_tbl(size_t *tbl, const char *chars, size_t num_chars) {
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

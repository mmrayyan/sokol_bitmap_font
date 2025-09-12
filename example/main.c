#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gp.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../sokol_bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static bitmap_font minogram_font;

static void frame(void) {
  sg_begin_pass(&(sg_pass){.swapchain = sglue_swapchain()});

  int width = sapp_width(), height = sapp_height();
  sgp_begin(width, height);
  sgp_viewport(0, 0, width, height);

  sgp_set_color(0.1f, 0.1f, 0.1f, 1.0f);
  sgp_clear();

  bitmap_draw_line(&minogram_font, "Hello,World", 10, (sgp_rect) {
    .x = 10,
    .y = 10,
    .w = 50,
    .h = 80,
  });

  sgp_flush();
  sgp_end();
  sg_end_pass();
  sg_commit();
}

static void init(void) {
  sg_setup(&(sg_desc){
    .environment = sglue_environment(),
    .logger.func = slog_func,
  });

  if (!sg_isvalid()) {
    fprintf(stderr, "Failed to create Sokol GFX context!\n");
    exit(-1);
  }

  sgp_setup(&(sgp_desc){0});

  if (!sgp_is_valid()) {
    fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
    exit(-1);
  }

  // Create and load font texture
  int width, height, channels;
  unsigned char *pixels = stbi_load("./minogram_6x10.png", &width, &height, &channels, 4); // 4 = RGBA
  if (!pixels) {
    fprintf(stderr, "Failed to load texture\n");
    exit(-1);
  }

  sg_image_desc img_desc = {
    .width = width,
    .height = height,
    .data.subimage[0][0] = {
      .ptr = pixels,
      .size = width * height * 4  // 4 bytes per pixel (RGBA)
    }
  };
  sg_image minogram_texture = sg_make_image(&img_desc);
  stbi_image_free(pixels);

  // Initialize font
  const char *minogram_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-=()[]{}<>/*:#%!?.,'\"@&$ ";
  size_t minogram_nchars = strlen(minogram_chars);
  bitmap_desc minogram_desc = {
    .img = minogram_texture,
    .img_width_pixels = width,
    .img_height_pixels = height,
    .img_padding_x_pixels = 0,
    .img_padding_y_pixels = 1,
    .char_width_pixels = 5,
    .char_height_pixels = 9,
    .char_padding_x_pixels = 1,
    .char_padding_y_pixels = 1,
    .chars = minogram_chars,
    .num_chars = minogram_nchars,
  };

  if (!bitmap_init(&minogram_font, minogram_desc)) {
    fprintf(stderr, "Failed to initialize bitmap font!\n");
    exit(-1);
  }
}

static void cleanup(void) {
  bitmap_free(&minogram_font);
  sgp_shutdown();
  sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
  return (sapp_desc){
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .window_title = "Bitmap Font Renderer",
    .logger.func = slog_func,
    .icon.sokol_default = true,
  };
}
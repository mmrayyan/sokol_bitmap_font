# Sokol Bitmap Font

Minimal bitmap font rendering for Sokol GP

## Overview

`sokol_bitmap_font` is a lightweight C library for drawing bitmap fonts in [Sokol](https://github.com/floooh/sokol) applications using the [Sokol GP](https://github.com/edubart/sokol_gp) 2D graphics API. It allows you to render text using a pre-generated bitmap font atlas, leveraging Sokol GP's batching and draw functions for efficient rendering.

## Features

- Draw individual characters or entire strings using a bitmap font atlas
- Integrates directly with Sokol GP's draw functions (`sgp_draw_textured_rect`)
- Simple API for font initialization and cleanup
- Customizable character size, padding, and atlas layout
- No dynamic allocations during rendering

## Getting Started

### 1. Add the library

Copy `sokol_bitmap.h` and `sokol_bitmap.c` into your project. Make sure you have Sokol headers (`sokol_gfx.h`, `sokol_gp.h`, etc.) available.

### 2. Prepare a bitmap font atlas

Create a PNG image containing all glyphs arranged in a grid. You will need to know the width/height of each glyph and the padding between them.

### 3. Initialize the font

```c
#include "sokol_bitmap.h"

// Load your font texture as sg_image (using stb_image or similar)
bitmap_desc desc = {
    .img = my_font_texture,
    .img_width_pixels = ...,
    .img_height_pixels = ...,
    .char_width_pixels = ...,
    .char_height_pixels = ...,
    .char_padding_x_pixels = ...,
    .char_padding_y_pixels = ...,
    .chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789...",
    .num_chars = strlen(chars),
};

bitmap_font font;
bitmap_init(&font, desc);
```

### 4. Draw text

Call `bitmap_draw_char` or `bitmap_draw_line` inside your frame rendering function:

```c
bitmap_draw_line(&font, "Hello, World!", 10, (sgp_rect){
    .x = 10, .y = 10, .w = 50, .h = 80
});
```

### 5. Cleanup

```c
bitmap_free(&font);
```

## API Reference

See [`sokol_bitmap.h`](sokol_bitmap.h) for full API documentation.

- `bool bitmap_init(bitmap_font *self, bitmap_desc desc);`
- `void bitmap_free(bitmap_font *self);`
- `void bitmap_draw_char(bitmap_font *self, char c, sgp_rect r);`
- `void bitmap_draw_line(bitmap_font *self, const char *s, float gap, sgp_rect r);`

## Example

See [`test/main.c`](test/main.c) for a complete usage example.

**Font Attribution:**  
The font used in the example is [**Minogram 6x10**](https://frostyfreeze.itch.io/pixel-bitmap-fonts-png-xml) by frostyfreeze on itch.io.

## License

This library is released under the zlib/libpng license, same as Sokol.

---

**Links**

- [Sokol GFX](https://github.com/floooh/sokol)
- [Sokol GP](https://github.com/edubart/sokol_gp)

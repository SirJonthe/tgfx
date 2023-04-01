# tgfx
## Copyright
Public domain, 2023

github.com/SirJonthe

## About
`tgfx` is a minimalist library for C++11 that can be used to display an image in a Unix terminal.

## Design
The library only aims to allow the user to let the terminal act as a kind of frame buffer to display images. It does not ship with routines for importing various image formats nor graphics rendering capabilities.

Note that this is a very small (incomplete) library that essentially will only work with a handful of terminal emulators.

Looking for something more complete? See `ncurses`.

## Usage
`tgfx` provides only a few contact points for the programmer to use. The main work-horse function is `TerminalBlit` which renders an image to the terminal window.

## Building
No special adjustments need to be made to build `tgfx` except enabling C++11 compatibility or above. Simply include the relevant headers in your code and make sure the headers and source files are available in your compiler search paths. Using `g++` as an example, building is no harder than:

```
g++ -std=c++11 code.cpp tgfx/tgfx.cpp
```

...where `code.cpp` is an example source file containing some user code as well as the entry point for the program.

## Examples
## Straight-forward blitting:
Straight-forward blitting assumes that the programmer has converted the image to be displayed into an RGB triplet of eight bits per channel. The code for displaying the image would then be:

```
#include "tgfx/tgfx.h"

struct Image
{
	uint8_t *pixels;
	uint32_t width, height;
};

int main()
{
	Image image = load_img("image.png");
	cc0::tgfx::TerminalBlit(image.pixels, image.width, image.height);
	destroy_image(image);
	return 0;
}
```

In this case we assume `load_img` loads the specified image and stores each pixel as a raw RGB triplet.

## Blitting using a different pixel format:
// also think of the alpha case

Some pixel formats may be loaded into memory differently than what `tgfx` expects. Either the programmer must reorganize the memory layout of the image, or the programmer can provide the blitting function with slightly more information in order to have it render properly.

In this case, we assume that we have loaded an image formatted ABGR (note the alpha component).

```
#include "tgfx/tgfx.h"

struct Image
{
	uint8_t *pixels;
	uint32_t width, height;
};

int main()
{
	Image image = load_img("image.png");
	cc0::tgfx::TerminalBlit(
		image.pixels, image.width, image.height,
		cc0::tgfx::PixelFormat{
			3, // The index of R.
			2, // The index of G.
			1, // The index of B.
			4, // The number of channels/bytes per pixel.
			2  // The number of characters making up a pixel in width. 2x to compensate for thin characters.
		}
	);
	destroy_image(image);
	return 0;
}
```

In this case we assume `load_img` loads the specified image and stores each pixel as a raw ABGR quadruplet.

Additional information can only help deal with pixel formats where each channel is eight bits and RGB are present in some order. When dealing with compressed formats where this might not be true see examples below.

## Blitting a region:
Sometimes the programmer wishes to only blit a small region of a larger image. As an example would be blitting a single sprite from a larger sprite sheet containing all sprites.

Blitting regions is no more difficult than providing the blitting function with the bounds for the region to be rendered.

```
#include "tgfx/tgfx.h"

struct Image
{
	uint8_t *pixels;
	uint32_t width, height;
};

int main()
{
	Image image = load_img("image.png");
	cc0::tgfx::TerminalBlit(
		image.pixels, image.width, image.height,
		cc0::tgfx::Region{
			image.width / 4,
			image.height / 4,
			image.width / 2,
			image.height / 2
		}
	);
	destroy_image(image);
	return 0;
}
```

We assume `load_img` loads the specified image in an RGB triplet format. The `Region` crops the borders of the image, effectively rendering only a quarter of the surface area of the original image.

## Scaling width:
Many terminal emulators ship with fonts that are much taller than they are wide. Unless corrected, the displayed image may look compacted along the X axis. By default the width scaling is set to 2, i.e. two characters are rendered in width for every one pixel to draw. This scaling may not work for all fonts so the ability to alter the scaling is present in all blitting functions in the `PixelFormat` parameter.

```
#include "tgfx/tgfx.h"

struct Image
{
	uint8_t *pixels;
	uint32_t width, height;
};

int main()
{
	Image image = load_img("image.png");
	cc0::tgfx::PixelFormat fmt; // Default values are set.
	fmt.width_scale = 3; // 3x characters in width per pixel will now be rendered.
	cc0::tgfx::TerminalBlit(
		image.pixels, image.width, image.height,
		fmt
	);
	destroy_image(image);
	return 0;
}
```

We assume `load_img` loads the specified image in an RGB triplet format.

## Custom pixel decoding:
In many cases, images loaded into memory do not come in a format directly readable by `tgfx` routines. In order to render such images to the terminal using examples above, the programmer would need to convert the loaded image into a raw format. However, a more performant way of rendering the image to the screen is to write a completely custom pixel decoder. That way, the pixel format could, for instance, be some compressed format or reduced bit depth that can be decompressed on the fly.

Since there is no end to compressed formats, we will not write an actually useful decoder here. Instead, `pixels` will still point to an RGB triplet, packed inside an ARGB quadruplet (this could be decoded more easily using the methods described above, but for the purposes of illustration we use a custom decoder):

```
#include "tgfx/tgfx.h"

struct Image
{
	void *pixels; // ARGB
	uint32_t width, height;
};

cc0::Pixel decode(const void *pixels, uint32_t width, uint32_t height, cc0::Coord coord)
{
	const uint8_t *argb = (const uint8_t*)pixels + (width * coord.y + coord.x) * 4;
	return Pixels{ argb[1], argb[2], argb[3] };
}

int main()
{
	Image image = load_img("image.png");
	cc0::tgfx::PixelFormat fmt; // Default values are set.
	fmt.width_scale = 3; // 3x characters in width per pixel will now be rendered.
	cc0::tgfx::TerminalBlit(
		image.pixels, image.width, image.height,
		decode
	);
	destroy_image(image);
	return 0;
}
```

The decode function take only a limited number of inputs, so additional formation needed to properly decode the pixels must come from other sources than as input parameters to the decoder function.

For more complex examples, `pixels` can be a non-RGB triplet, compressed format, and that `decode` can decode the pixels into an RGB triplet given the coordinate of the pixel that we want to decode.

## Demo:
The libary ships with a small example that the programmer can trigger showing an animation. The code to trigger the example is as follows:

```
#include <thread>
#include "tgfx/tgfx.h"

int main(int, char**)
{
	while (true) {
		cc0::tgfx::RunExample();
	}
	return 0;
}
```

Note that the application will run forever. Under Unix, press Ctrl+C to terminate the application.

## Future work
Images should be showed where the cursor is when launching the application. This can be difficult, however, as it would seem only a handful of terminals support saving and restoring the cursor position.

Ensure reasonable compatibility with the terminal emulators that ship with Windows and macOS.

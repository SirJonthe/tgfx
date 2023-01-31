/// @file
/// @author github.com/SirJonthe
/// @date 2023
/// @copyright Public domain.
/// @license BSD-3-Clause

// THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef TGFX_H_INCLUDED__
#define TGFX_H_INCLUDED__

#include <cstdint>

namespace tgfx
{

/// Describes the format of the incoming pixels.
struct PixelFormat
{
	uint32_t r_index         = 0; // The index of the red color component.
	uint32_t g_index         = 1; // The index of the green color component.
	uint32_t b_index         = 2; // The index of the blue color component.
	uint32_t bytes_per_pixel = 3; // The number of bytes per pixel.
	uint32_t width_scale     = 2; // The number of characters in width that make up a pixel. Used to scale pixels properly in terminals that use tall characters. 
};

/// Data used to render only a portion of the incoming pixels. 
struct Region
{
	uint32_t x_offset; // The X offset in the image.
	uint32_t y_offset; // The Y offset in the image.
	uint32_t width;    // The width of the frame inside the image.
	uint32_t height;   // The height of the frame inside the image.
};

/// An RGB triplet.
struct Pixel
{
	uint8_t r, g, b;
};

/// A 2D coordinate.
struct Coord
{
	uint32_t x, y;
};

/// Blits an image with specified width, height, and format, and displays it in the terminal window. 
/// @param pixels A pointer to the pixels of the image.
/// @param width The width, in pixels, of the image.
/// @param height The height, in pixels, of the image.
/// @param region The region of the image to draw.
/// @param format The format of the pixels in the image.
void TerminalBlit(const uint8_t *pixels, uint32_t width, uint32_t height, Region region, PixelFormat format = PixelFormat());

/// Blits an image with specified width, height, and format, and displays it in the terminal window. 
/// @param pixels A pointer to the pixels of the image.
/// @param width The width, in pixels, of the image.
/// @param height The height, in pixels, of the image.
/// @param format The format of the pixels in the image.
void TerminalBlit(const uint8_t *pixels, uint32_t width, uint32_t height, PixelFormat format = PixelFormat());

/// Blits an image with specified width, and height, and displays it in the terminal window. 
/// @param pixels A pointer to the pixels of the image.
/// @param width The width, in pixels, of the image.
/// @param height The height, in pixels, of the image.
/// @param region The region of the image to draw.
/// @param decoder A decoder function used to interpret the incoming pixel format into an RGB triplet. The decoder takes the incoming pixel pointer and the currently rendered pixel coordinate, and returns the RGB triplet.
/// @param width_scale The number of characters in width that make up a pixel. Used to scale pixels properly in terminals that use tall characters.
void TerminalBlit(const void *pixels, uint32_t width, uint32_t height, Region region, Pixel (*decoder)(const void*, Coord), uint32_t width_scale = 2);

/// Blits an image with specified width, and height, and displays it in the terminal window. 
/// @param pixels A pointer to the pixels of the image.
/// @param width The width, in pixels, of the image.
/// @param height The height, in pixels, of the image.
/// @param decoder A decoder function used to interpret the incoming pixel format into an RGB triplet. The decoder takes the incoming pixel pointer and the currently rendered pixel coordinate, and returns the RGB triplet.
/// @param width_scale The number of characters in width that make up a pixel. Used to scale pixels properly in terminals that use tall characters.
void TerminalBlit(const void *pixels, uint32_t width, uint32_t height, Pixel (*decoder)(const void*, Coord), uint32_t width_scale = 2);

/// A small example showing an animated character in the terminal window. 
void RunExample( void );

}

#endif

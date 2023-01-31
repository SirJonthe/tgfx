#include <iostream>
#include <thread>
#include "tgfx.h"

uint32_t EncodeChannel(char *color_code, uint32_t n, uint8_t c)
{
	for (uint32_t i = 0; i < 3; ++i) {
		color_code[n--] = char(c % 10) + '0';
		c /= 10;
	}
	return n - 1;
}
void EncodeColor(char *color_code, uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t n = 17;
	n = EncodeChannel(color_code, n, b);
	n = EncodeChannel(color_code, n, g);
	EncodeChannel(color_code, n, r);
}

void ResetCursor( void )
{
	std::cout << "\e[u\e[s\n";
}

uint32_t EncodeCoord(char *xy_code, uint32_t n, uint32_t c)
{
	for (uint32_t i = 0; i < 4; ++i) {
		xy_code[n--] = char(c % 10) + '0';
		c /= 10;
	}
	return n - 1;
}

void EncodeXY(char *xy_code, uint32_t x, uint32_t y)
{
	uint32_t n = 10;
	n = EncodeCoord(xy_code, n, x);
	EncodeCoord(xy_code, n, y);
}

void Refresh( void )
{
	std::cout.flush();
}

void tgfx::TerminalBlit(const uint8_t *pixels, uint32_t width, uint32_t height, Region region, tgfx::PixelFormat format)
{
	ResetCursor();
	char color_code[] = "\e[48;2;000;000;000m ";
	for (uint32_t y = region.y_offset; y < region.y_offset + region.height; ++y) {
		for (uint32_t x = region.x_offset * format.width_scale; x < (region.x_offset + region.width) * format.width_scale; ++x) {
			const uint8_t *p = pixels + ((x / format.width_scale) + y * width) * format.bytes_per_pixel;
			EncodeColor(color_code, p[format.r_index], p[format.g_index], p[format.b_index]);
			std::cout << color_code;
		}
		std::cout << std::endl;
	}
	Refresh();
}

void tgfx::TerminalBlit(const uint8_t *pixels, uint32_t width, uint32_t height, tgfx::PixelFormat format)
{
	tgfx::TerminalBlit(pixels, width, height, Region{ 0, 0, width, height }, format);
}

void tgfx::TerminalBlit(const void *pixels, uint32_t width, uint32_t height, Region region, Pixel (*decoder)(const void*, tgfx::Coord), uint32_t width_scale)
{
	tgfx::Pixel rgb;
	ResetCursor();
	char color_code[] = "\e[48;2;000;000;000m ";
	for (uint32_t y = region.y_offset; y < region.y_offset + region.height; ++y) {
		for (uint32_t x = region.x_offset * width_scale; x < (region.x_offset + region.width) * width_scale; ++x) {
			rgb = decoder(pixels, Coord{ x / width_scale, y });
			EncodeColor(color_code, rgb.r, rgb.g, rgb.b);
			std::cout << color_code;
		}
		std::cout << std::endl;
	}
	Refresh();
}

void tgfx::TerminalBlit(const void *pixels, uint32_t width, uint32_t height, Pixel (*decoder)(const void*, tgfx::Coord), uint32_t width_scale)
{
	tgfx::TerminalBlit(pixels, width, height, Region{ 0, 0, width, height }, decoder, width_scale);
}

const uint8_t flirp_rgb[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x34,0x60,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xfc,0xfc,0xfc,0xfc,0xfc,0xfc,0x20,0x34,0x60,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x1c,0x18,0x30,0xb8,0xfc,0xfc,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0xb8,0xfc,0xfc,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x20,0x34,0x60,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x20,0x34,0x60,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x60,0xc0,0xc8,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x60,0xc0,0xc8,0x4c,0x90,0xac,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x4c,0x90,0xac,0x4c,0x90,0xac,0x1c,0x18,0x30,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x18,0x30,0x1c,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
const uint32_t flirp_width  = 128;
const uint32_t flirp_height =  16;
const uint32_t flirp_frames =   8;

void tgfx::RunExample( void )
{
	const auto start = std::chrono::high_resolution_clock::now();
	const uint32_t frame_width = flirp_width / flirp_frames;
	const uint32_t frame = uint32_t((start.time_since_epoch().count() * 15) / 1000000000) % flirp_frames;
	tgfx::TerminalBlit(flirp_rgb, flirp_width, flirp_height, Region{ frame * frame_width, 0, frame_width, flirp_height });
	const auto end = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::milliseconds(67) - (end - start));
}

#include <stdio.h>
#include "tgawrite.h"

void writeTGAFile(const char* const fileName, const unsigned width, const unsigned height, RGBColor* const data) {
	FILE* file = fopen(fileName, "w");

	// Tga file format: https://de.wikipedia.org/wiki/Targa_Image_File

	putc(0, file);
	putc(0, file);
	putc(2, file);
	putc(0, file);
	putc(0, file);
	putc(0, file);
	putc(0, file);
	putc(0, file);
	putc(0, file);
	putc(0, file);
	putc(0, file);
	putc(0, file);
	putc(width & 0xFF, file);
	putc((width & 0xFF00) >> 8, file);
	putc(height & 0xFF, file);
	putc((height & 0xFF00) >> 8, file);
	putc(24, file);
	putc(0, file);

	unsigned x;
	unsigned y;

	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			RGBColor pixel = data[y*width + x];

			// Tga is BGR-ordered (internal order used in modern graphics cards)

			putc(pixel.b, file);
			putc(pixel.g, file);
			putc(pixel.r, file);
		}
	}

	fclose(file);
}

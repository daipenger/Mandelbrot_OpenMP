#ifndef TGAWRITE_HEADER
#define TGAWRITE_HEADER

struct RGBColor {
unsigned char r;
unsigned char g;
unsigned char b;
};

typedef struct RGBColor RGBColor;

void writeTGAFile(const char* const fileName, const unsigned width, const unsigned height, RGBColor* const data);
#endif

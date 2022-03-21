// _OPENMP is defined if the compiler gets the -fopenmp flag
#ifdef _OPENMP
#include <omp.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "ComplexNumber.h"
#include "tgawrite.h"

// Brightness is hardcoded to be 1
void HSBtoRGB(RGBColor* buffer, double hue, double saturation) {
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;

	double h = (hue - floor(hue)) * 6.0f;
	double f = h - floor(h);
	double p = (1.0f - saturation);
	double q = (1.0f - saturation * f);
	double t = (1.0f - (saturation * (1.0f - f)));

	switch ((int) h) {
            case 0:
                g = (unsigned char) (t * 255.0f + 0.5f);
                b = (unsigned char) (p * 255.0f + 0.5f);
                break;
            case 1:
                r = (unsigned char) (q * 255.0f + 0.5f);
                b = (unsigned char) (p * 255.0f + 0.5f);
                break;
            case 2:
                r = (unsigned char) (p * 255.0f + 0.5f);
                b = (unsigned char) (t * 255.0f + 0.5f);
                break;
            case 3:
                r = (unsigned char) (p * 255.0f + 0.5f);
                g = (unsigned char) (q * 255.0f + 0.5f);
                break;
            case 4:
                r = (unsigned char) (t * 255.0f + 0.5f);
                g = (unsigned char) (p * 255.0f + 0.5f);
                break;
            case 5:
                g = (unsigned char) (p * 255.0f + 0.5f);
                b = (unsigned char) (q * 255.0f + 0.5f);
                break;
    }

    buffer->r = r;
    buffer->g = g;
    buffer->b = b;
}

RGBColor* createMandelbrotSet(const double xmin, const double xmax, const double ymin, const double ymax, const unsigned iterations,
	const unsigned width, const unsigned height) {
	RGBColor* finalField = (RGBColor*) malloc(sizeof(RGBColor) * width * height);
	if (finalField == NULL) return NULL;
        unsigned y;

	#ifdef _OPENMP
	double startTime;
	double endTime;
	startTime = omp_get_wtime();

	#pragma omp parallel for schedule(static, 10)
	#endif
	for (y = 0; y < height; ++y) {
		RGBColor* line = &finalField[y * width];
		double im = ((double) y / (double) height) * (ymax - ymin) + ymin;
		unsigned x;

		for (x = 0; x < width; ++x) {
			double re = ((double) x / (double) width) * (xmax - xmin) + xmin;
			ComplexNumber seed = { re, im };
			ComplexNumber current = { 0, 0 };

			unsigned i;
			for (i = 0; i < iterations; ++i) {
				squareComplexNumber(&current);
				addComplexNumber(&current, &seed);
				if (lengthSquared(&current) > 4) break;
			}

			if (i == iterations) {
				RGBColor* pixel = line + x;
				pixel->r = 0;
				pixel->g = 0;
				pixel->b = 0;
			} else {
				double smoothColor = (double) i + 2.0 - 1.442695 * log(log(lengthSquared(&current)));
				smoothColor /= (double) iterations;
				HSBtoRGB(line + x, 0.95 + 10.0 * smoothColor, 0.6);
			}
		}

	}

	#ifdef _OPENMP
	endTime = omp_get_wtime();
	printf("OpenMP region took %1.3fs\n", (endTime - startTime));
	#endif

	return finalField;
}

unsigned getStringLength(char* string) {
	unsigned x = 0;
	while(string[x]) ++x;
	return x;
}

int main(int argc, char** argv) {
	double xmin, xmax, ymin, ymax;
	unsigned iterations, width, height;

	if (argc == 7) {
		// xmid,ymid,radius,#iterations,width,height
		double xmid = atof(argv[1]);
		double ymid = atof(argv[2]);
		double radius = atof(argv[3]);
		iterations = atoi(argv[4]);
		width = atoi(argv[5]);
		height = atoi(argv[6]);
		double aspectratio = (double) width / (double) height;
		if (aspectratio > 1) {
			xmin = xmid - radius * aspectratio;
			xmax = xmid + radius * aspectratio;
			ymin = ymid - radius;
			ymax = ymid + radius;
		} else {
			xmin = xmid - radius;
			xmax = xmid + radius;
			ymin = ymid - radius / aspectratio;
			ymax = ymid + radius / aspectratio;
		}
	} else if (argc == 8) {
		// xmin,xmax,ymin,ymax,#iterations,width,height
        	xmin = atof(argv[1]);
        	xmax = atof(argv[2]);
        	ymin = atof(argv[3]);
        	ymax = atof(argv[4]);
        	iterations = atoi(argv[5]);
        	width = atoi(argv[6]);
	        height = atoi(argv[7]);
	} else {
                puts("Possible command line parameters:");
                puts("xmin, xmax, ymin, ymax, #Iterations, width, height");
                puts("xmiddle, ymiddle, radius, #Iterations, width, height (radius is scaled up on the axis with the larger portion of the aspect ratio)");
                return 0;
	}

	if (iterations < 1) {
		puts("Iterations must at least be 1");
		return 0;
	}
	if (width < 1) {
		puts("Width must at least be 1");
		return 0;
	}
	if (height < 1) {
		puts("Height must at least be 1");
		return 0;
	}

	unsigned fileNameLength = 100;
	char fileName[fileNameLength];
	snprintf(fileName, fileNameLength, "x=[%g, %g] y=[%g, %g] iter=%d.tga", xmin, xmax, ymin, ymax, iterations);

	#ifdef _OPENMP
	printf("Creating mandelbrot set using OpenMP on %d threads\n", omp_get_max_threads());
	#else
	printf("Creating mandelbrot set without OpenMP\n");
	#endif

	RGBColor* set = createMandelbrotSet(xmin, xmax, ymin, ymax, iterations, width, height);
	if (set == NULL) {
		printf("Not enough heap space for mandelbrot\n");
		return 1;
	}
	writeTGAFile(fileName, width, height, set);
	free(set);

	return 0;
}

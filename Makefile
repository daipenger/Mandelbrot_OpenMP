all: par parAVX2

seq: ComplexNumber.h tgawrite.h Main.c ComplexNumber.c tgawrite.c
	gcc tgawrite.c ComplexNumber.c Main.c -Wall -s -Ofast -o mandelbrotSeq -lm

par: ComplexNumber.h tgawrite.h Main.c ComplexNumber.c tgawrite.c
	gcc tgawrite.c ComplexNumber.c Main.c -Wall -s -Ofast -o mandelbrotPar -lm -fopenmp

seqAVX2: ComplexNumber.h tgawrite.h Main.c ComplexNumber.c tgawrite.c
	gcc tgawrite.c ComplexNumber.c Main.c -Wall -s -Ofast -march=core-avx2 -o mandelbrotSeqAVX2 -lm

parAVX2: ComplexNumber.h tgawrite.h Main.c ComplexNumber.c tgawrite.c
	gcc tgawrite.c ComplexNumber.c Main.c -Wall -s -Ofast -march=core-avx2 -o mandelbrotParAVX2 -lm -fopenmp

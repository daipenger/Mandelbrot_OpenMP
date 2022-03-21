#include "ComplexNumber.h"

void squareComplexNumber(ComplexNumber* number) {
    double x = number->re;
    double y = number->im;
    number->re = x * x - y * y;
    number->im = x * y * 2;
}

void addComplexNumber(ComplexNumber* target, ComplexNumber* source) {
    target->re += source->re;
    target->im += source->im;
}

double lengthSquared(ComplexNumber* number) {
    double x = number->re;
    double y = number->im;
    return x * x + y * y;
}
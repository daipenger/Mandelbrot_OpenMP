#ifndef COMPLEXNUMBER_HEADER
#define COMPLEXNUMBER_HEADER

struct ComplexNumber {
	double re;
	double im;
};

typedef struct ComplexNumber ComplexNumber;

void squareComplexNumber(ComplexNumber* number);
void addComplexNumber(ComplexNumber* target, ComplexNumber* source);
double lengthSquared(ComplexNumber* number);

#endif
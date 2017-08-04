#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DIM 500
#define ARRAY_SIZE 16777216

typedef struct {
	int r;
	int g;
	int b;
} pixel;

typedef struct {
	pxl color;
	int counter;
} pxlCounter;

bool pxlComparison(pxl pxl1, pxl pxl2);

bool checkColors(pxl _pxl, counter *array, int c);

void istoStamp(counter *array, int c);


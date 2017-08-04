#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pixel.h"

bool pxlComparison(pxl pxl1, pxl pxl2) {
	if(pxl1.r == pxl2.r && pxl1.g == pxl2.g && pxl1.b == pxl2.b)
		return true;
	else return false;
}

bool checkColors(pxl _pxl, counter *array, int c) {
	for(int i = 0; i <= c; i++) {
		if(pxlComparison(_pxl, array[i].color)) {
			array[i].counter += 1;
			return true;
		}
	}
	
	return false;
}

void istoStamp(counter *array, int c) {
	for(int i = 0; i <= c; i++) {
		printf("%03d %03d %03d: ", array[i].color.r, array[i].color.g, array[i].color.b);
		for(int j = 0; j < array[i].counter; j++) {
			printf("*");
		}
		printf("\n");
	}
}



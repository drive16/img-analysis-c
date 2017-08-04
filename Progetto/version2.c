#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DIM 500
#define ARRAY_SIZE 16777216
#define FILENAME "immagini/sample182x345.bmp"

typedef struct pixel pxl;
typedef struct pxlCounter counter;

struct pixel {
	int r;
	int g;
	int b;
};

struct pxlCounter {
	pxl color;
	int counter;
};

pxl matrix[DIM][DIM];
counter colors[ARRAY_SIZE];

int imgL(char *filename) {
	FILE *fp;
	int n, int_var, l;
	fp = fopen(filename, "r");
	fseek(fp, 18, 0);
	n = fread(&int_var, sizeof(int), 1, fp);
	l = int_var;
	fclose(fp);
	return l;
}

int imgH(char *filename) {
	FILE *fp;
	int n, int_var, h;
	fp = fopen(filename, "r");
	fseek(fp, 22, 0);
	n = fread(&int_var, sizeof(int), 1, fp);
	h = int_var;
	fclose(fp);
	return h;
}

int nBit(char *filename) {
	FILE *fp;
	short short_var;
	int n, nBit;
	fp = fopen(filename, "r");
	fseek(fp, 28, 0);
	n = fread(&short_var, sizeof(short), 1, fp);
	nBit = short_var;
	fclose(fp);
	return nBit;
}

int getOffset(char *filename) {
	FILE *fp;
	int n, int_var, offset;
	fp = fopen(filename, "r");
	fseek(fp, 10, 0);
	n = fread(&int_var, sizeof(int), 1, fp);
	offset = int_var;
	fclose(fp);
	return offset;
}

//lunghezza della riga in byte
int rowL(char *filename) {
	int w = imgL(filename);
	int bit = nBit(filename);
	int l = ((w * bit) + (32 - (w * bit) % 32)) / 8;
	return l;
}

//totale byte necessari per memorizzare un array di pixel per un'immagine a n bit
int pixelArraySize(char *filename) {
	int l = rowL(filename);
	int h = imgH(filename);
	int pxlArraySize = l * h;
	return pxlArraySize;
}

int getPadding(char *filename) {
	int rowSize = rowL(filename);
	int bit = nBit(filename);
	int l = imgL(filename);
	int nByte = l * bit / 8;
	if(nByte % 4 == 0) return 0;
	else {
		int nPadding = rowSize % (nByte);
		return nPadding;
	}
}

void readRow(char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	unsigned char BitMap[3];
	int h = imgH(filename);
	int l = imgL(filename);

	fseek(fp, getOffset(filename), 0);
	
	for(int j = 0; j < l; j++) {
		for(int i = 0; i < h; i++) {
			fread(BitMap, sizeof(char), 3, fp);
			printf("%03d %03d %03d\n", BitMap[0], BitMap[1], BitMap[2]);
			}
			fread(BitMap, sizeof(char), getPadding(filename), fp);
	}

	fclose(fp);
}

bool checkSignature(char *filename) {
	FILE *fp;
	char signature[2];
	fp = fopen(filename, "r");
	
	if(fp == NULL) {
		printf("File non trovato!\n");
		return false;
	}
	
	fseek(fp, 0, 0);
	fread(signature, sizeof(char), 2, fp);
	
	if(signature[0] == 'B' && signature[1] == 'M') return true;
	else {
		printf("Codifica file errata!\n");
		return false;
	}
	fclose(fp);
}

bool checkBit(char *filename) {
	if(nBit(filename) == 24) return true;
	else{
		printf("L'immagine non è a 24-bit");
		return false;
	}
}

bool checkDimension(char *filename) {
	if(imgH(filename) <= DIM && imgL(filename) <= DIM) return true;
	else {
		printf("L'immagine non rispetta le dimensioni!\n");
		return false;
	}
}

bool checkImage(char *filename) {
	if(checkSignature(filename) && checkBit(filename) && checkDimension(filename)) return true;
	else return false;
}

void createMatrix(char *filename, pxl matrix[DIM][DIM]) {
	if(checkImage(filename)) {
		FILE *fp;
		fp = fopen(filename, "r");
		unsigned char BitMap[3];
		int h = imgH(filename);
		int l = imgL(filename);
	
		fseek(fp, getOffset(filename), 0);
		for(int j = 0; j < h; j++) {
			for(int i = 0; i < l; i++) {
				fread(BitMap, sizeof(char), 3, fp);
				pxl tmp;
				tmp.b = BitMap[0];
				tmp.g = BitMap[1];
				tmp.r = BitMap[2];
				matrix[j][i] = tmp;
			}
			
			fread(BitMap, sizeof(char), getPadding(filename), fp);
		}

		fclose(fp);
	}
}

bool pxlComparison(pxl pxl1, pxl pxl2) {
	if(pxl1.r == pxl2.r && pxl1.g == pxl2.g && pxl1.b == pxl2.b)
		return true;
	else return false;
}

//funzione che ritorna true se un pixel è uguale a quello contenuto in un array e ne aumenta il relativo contatore
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

void matrixCounter(char *filename) {
	//pxl matrix[DIM][DIM];
	int h = imgH(filename);
	int l = imgL(filename);
	int c = 0;
	
	if(!checkImage(filename)) printf("\nCambiare immagine, per favore\n");
		else {
		createMatrix(filename, matrix);
	
		colors[c].color = matrix[0][0];
	
		for(int j = 0; j < h; j++) {
			for(int i = 0; i < l; i++) {
				if(!(checkColors(matrix[j][i], colors, c))) {
					c++;
					colors[c].color = matrix[j][i];
					colors[c].counter += 1;
				}
			}
		}
	
	for(int i = 0; i <= c; i++) {
		printf("%03d %03d %03d: ", colors[i].color.r, colors[i].color.g, colors[i].color.b);
		printf("%d\n", colors[i].counter);
	}
		printf("\nColori trovati: %d\n", c+1);
	}
	//istoStamp(colors, c);
}

void bitmapCreator(char *filename) {
	FILE *fp;
	int w, n, int_var, nBit, rowL, nByte, l, b;
	short short_var;
	unsigned char BitMap[64000L];
	
	fp = fopen(filename, "r");

	fseek(fp, 18, 0);
	n = fread(&int_var,sizeof(int), 1, fp);
	w = int_var;

	fseek(fp, 28, 0);
    	n = fread(&short_var, sizeof(short), 1, fp);
	nBit = short_var;
	
	rowL = ((w * nBit) + (32 - (w * nBit) % 32)) / 8;
	
	fseek(fp, getOffset(filename), SEEK_SET);
	nByte = nBit / 8;
    	l = fread(BitMap, sizeof(unsigned char), rowL, fp);
	
    	while (l == rowL) {
        	for (n = 0; n < rowL; n++) {
                	if (b == 0) printf("[");
                	printf("%02X", BitMap[n]);
                	if (b == (nByte - 1)) printf("]");
                	b = (b + 1) % nByte;
       	 	}	
        printf("\n");
        l = fread(BitMap, sizeof(unsigned char), rowL, fp);
    	}

    	while (!feof(fp));
    	fclose(fp);
}


int main() {
	//printf("length: %d heigth: %d nBit: %d\n", imgL(FILENAME), imgH(FILENAME), nBit(FILENAME));
	//readRow(FILENAME);
	//readMatrix(FILENAME);
	//printf("padding: %d\n", getPadding(FILENAME));
	//printf("%d\n", pixelArraySize(FILENAME));
	matrixCounter(FILENAME);
	//printf("%d\n", getOffset(FILENAME));
	return 0;
}

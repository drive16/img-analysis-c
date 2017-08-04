#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_HEADER 54
#define FILENAME "immagini/kappa.bmp"

int iOffSet, i_w, i_h, headers_size, nBit;

void bin8(int n, char bina[]) {
     	int i;
     	for (i=0 ; i < 8 ; i++)
     	{
        	bina[7-i]=(n % 2)+'0';
         	n = (n - (n % 2) ) /2;
     	}
}

void DumpFile(char *Nf)
{
    	unsigned char BitMap[64000L];
    	char dig[3], bina[8];
    	unsigned char d;
    	int LungRow, nByte;
    	int n=0, b=0,l;
    	// Dimensione riga: multiplo di 4 byte
    	LungRow=( (i_w*nBit)+(32-(i_w*nBit)%32) )/8;
		FILE *fpo;
    	if ( (fpo=fopen(Nf,"rb"))==NULL )
    	{
         	printf("File non trovato!!");
         	return;
    	}
    	fseek(fpo,iOffSet,0);
    	printf("---------------------------------------------\n");
    	printf("SEZIONE --> Bitmap data dell'immagine\n");
    	printf("---------------------------------------------\n");
    	nByte=nBit/8;
    	if (nByte<1) nByte=1;
    	l=fread(BitMap, sizeof(unsigned char), LungRow, fpo);
    	while (l == LungRow) {
        	for ( n=0 ; n < LungRow ; n++) {
            		if ( (nBit==24) || (nBit==8) ) {
                		if (b==0) printf("[");
                		printf("%02X",BitMap[n]);
                		if (b==(nByte-1)) printf("]");
                		b=(b+1) % nByte;
            		}
            		else if (nBit==4) {
              			sprintf(dig, "%02X", BitMap[n]);
       				printf("[%c] [%c] ",dig[0],dig[1]);
            		}
            		else if (nBit==1) {
                		l=(int)BitMap[n];
                		bin8(l,bina);
                		printf("[%s]",bina);
            		}
       	 	}	
        printf("\n");
        l=fread(BitMap, sizeof(unsigned char), LungRow, fpo);
    	}

    	while (!feof(fpo));
   	printf("---------------------------------------------\n");
    	fclose(fpo);
}

void infoBMP(char *filename) {

	FILE *fp;
	int n, j;
	unsigned char Palette[1024];
	
	if(!(fp = fopen(filename, "r"))) {
		printf("\nErrore durante l'apertura del file!\n");
		return;
	}

	fp = fopen(filename, "r");

	int int_var;
	short short_var;
	float float_var;
	char magic[2];
	
	printf("---------------------------------------------\n");
	printf("SEZIONE --> Titolo del File\n");
	printf("---------------------------------------------\n");

	fseek(fp, 0, 0);
	n = fread(magic, 2, 1, fp);
	printf(" - Firma		: %c%c\n", magic[0], magic[1]);

	fseek(fp, 2, 0);
	fread(&int_var, 1, sizeof(int), fp);
	printf(" - Dimensione file	: %d\n", int_var);

	fseek(fp,10,0);
    	fread(&int_var, 1, sizeof(int), fp);
    	printf(" - Offset               : %d\n",int_var);
    	iOffSet=int_var;

	printf("---------------------------------------------\n");
	printf("SEZIONE --> Titolo dell'Immagine\n");
	printf("---------------------------------------------\n");

	fseek(fp, 14, 0);
	n = fread(&int_var, sizeof(int), 1, fp);
	printf(" - Dimensione Header Immagine : %d\n", int_var);
	headers_size=int_var+14;

    	n = fread(&int_var,sizeof(int), 1, fp);
    	printf(" - Larghezza                  : %d\n",int_var);
    	i_w=int_var;

    	n = fread(&int_var,sizeof(int), 1, fp);
    	printf(" - Altezza                    : %d\n",int_var);
    	i_h=int_var;

	fseek(fp, 28, 0);
    	n = fread(&short_var, sizeof(short), 1, fp);
    	printf(" - nr. bit per colore         : %hd\n",short_var);

    	nBit=short_var;
    	n = fread(&int_var,sizeof(int), 1, fp);

   	if (int_var==0)
       		printf(" - tipo compressione          : Nessuna\n");
   	else if (int_var == 1)
       		printf(" - tipo compressione          : RLE 8 Bit\n");
    	else if (int_var == 2)
       		printf(" - tipo compressione          : RLE 4 Bit\n");
    	else if (int_var == 3)
       		printf(" - tipo compressione          : Bitfields\n");
    	else
       		printf(" - tipo compressione          : %d\n",int_var);
    	fread(&int_var, 1, sizeof(int), fp);
    	printf(" - bytes immagine senza header: %d\n",int_var);
    	printf("---------------------------------------------\n");
    	printf("SEZIONE --> Palette dell'immagine\n");
    	printf("---------------------------------------------\n");
    	if (headers_size == iOffSet)
       		printf(" - Palette                     : Non presente\n");
    	else {
        	fseek(fp, headers_size, 0);
      		fread(Palette, sizeof(unsigned char), iOffSet-headers_size, fp);
        	for (n=0, j=0 ; n < ( (iOffSet-headers_size)/4 ); n++)
        		{
           			printf("%02X^col. :%02lX",n, (long unsigned int) Palette[j++]);
           			printf("%02lX", (long unsigned int) Palette[j++]);
           			printf("%02lX | ", (long unsigned int) Palette[j++]);
          			if ( ( (n+1) % 4)==0) printf("\n");
           			j++;
        		}	
    		}
    	printf("\n");
    	fclose(fp);
}

void readColors(char *filename) {
	FILE *fp;
	int w, n, int_var, nBit, rowL, nByte, l, b;
	short short_var;
	unsigned char BitMap[64000L];
	
	fp = fopen(filename, "r");

	fseek(fp, 18, 0);
	n = fread(&int_var,sizeof(int), 1, fp);
	//printf(" - Larghezza                  : %d\n", int_var);
	w = int_var;

	fseek(fp, 28, 0);
    	n = fread(&short_var, sizeof(short), 1, fp);
	//printf(" - nr. bit per colore         : %hd\n", short_var);
	nBit = short_var;
	
	rowL = ((w * nBit) + (32 - (w * nBit) % 32)) / 8;
	
	fseek(fp, DIM_HEADER, SEEK_SET);
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
	infoBMP(FILENAME);
	return 0;
}

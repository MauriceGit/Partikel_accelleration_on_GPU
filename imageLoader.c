
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
/* ---- Eigene Header einbinden ---- */
#include "imageLoader.h"

int error(FILE * f, char * name, char * errorMessage) {
	printf("%s: %s.\n", errorMessage, name);
	fclose(f);
	return 0;
}

/* Loads image for texture creation.*/
int imageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 /* size of the image in bytes.*/
    unsigned long i;                    /* standard counter.*/
    unsigned short int planes;          /* number of planes in image (must be 1) */
    unsigned short int bpp;             /* number of bits per pixel (must be 24)*/
    char temp;                          /* used to convert bgr to rgb color.*/

	image->sizeX = 0;
	image->sizeY = 0;

    /* make sure the file is there*/
    if ((file = fopen(filename, "rb")) == NULL)
		return error(file, "File Not Found", filename);
    
    /* seek through the bmp header, up to the width/height:*/
    fseek(file, 18, SEEK_CUR);

    /* read the width*/
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) 
		return error(file, "Error reading width from", filename);
		
    printf("Width of %s: %lu\n", filename, image->sizeX);
    
    /* read the height */
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) 
		return error(file, "Error reading height from", filename); 
		
    printf("Height of %s: %lu\n", filename, image->sizeY);
    
    /* calculate the size (assuming 24 bits or 3 bytes per pixel).*/
    size = image->sizeX * image->sizeY * 3;

    /* read the planes*/
    if ((fread(&planes, 2, 1, file)) != 1) 
		return error(file, "Error reading planes from", filename);
		
    if (planes != 1)
		return error(file, "Planes from file is not 1", filename);

    /* read the bpp */
    if ((i = fread(&bpp, 2, 1, file)) != 1) 
		return error(file, "Error reading bpp from", filename);
		
    if (bpp != 24) 
		return error(file, "Bpp from file is not 24\n", filename);
	
    /* seek past the rest of the bitmap header.*/
    fseek(file, 24, SEEK_CUR);

    /* read the data. */
    image->data = malloc(size);
    if (image->data == NULL) 
		return error(file, "Error allocating memory for color-corrected image data", filename);
		
    if ((i = fread(image->data, size, 1, file)) != 1) 
		return error(file, "Error reading image data from", filename);

    for (i=0;i<size;i+=3) 
	{ 
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
    }

	fclose(file);

    return 1;
}

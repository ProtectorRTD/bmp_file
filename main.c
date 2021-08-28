#include "bmp.h"
#include "transformations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <bmp.c>
#include <transformations.c>
int main()
{
     FILE * input1 = fopen("adventurer.bmp", "rb");
     struct bmp_image* image = read_bmp(input1);
     fclose(input1);
     char *rgb = r;
     struct bmp_image* image2 = flip_horizontally(image);
     struct bmp_image* image3 = flip_vertically(image);
     struct bmp_image* image4 = rotate_right(image);
     struct bmp_image* image5 = rotate_left(image);
     struct bmp_image* image6 = crop(image, 0, 0, 40, 40);
     struct bmp_image* image7 = scale(image,0.66);
     struct bmp_image* image8 = extract(image,rgb);

     FILE *input2 = fopen("test.bmp", "wb");
     write_bmp(input2, image8);
     fclose(input2);
     free_bmp_image(image);
     free_bmp_image(image2);
     free_bmp_image(image3);
     free_bmp_image(image4);
     free_bmp_image(image5);
     free_bmp_image(image6);
     free_bmp_image(image7);
     free_bmp_image(image8);



}

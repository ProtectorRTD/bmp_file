#include "bmp.h"
#include "transformations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
bool check_stream (FILE* stream) // check stream
{
    if(stream == NULL) return false;
    return true;
}
bool check_image (const struct bmp_image* image)
{
    if(image == NULL) return false;
    return true;
}
struct bmp_header* read_bmp_header(FILE* stream) //https://www.youtube.com/watch?v=Y8EBRn4Bf70, https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/
{
    bool answer = check_stream(stream);
    if(answer == false) return NULL;
    fseek (stream, 0, SEEK_SET);
    struct bmp_header* check = malloc(sizeof(struct bmp_header));
    if((fread (check, sizeof (struct bmp_header), 1, stream) != 1 ) || check->type != 0x4D42 ) 
    {
		free(check);
		return NULL;
	}
    if(check -> width <= 0|| check -> height <= 0) 
    {
        free(check);
        return NULL;
    }
    return check;
}
struct pixel* read_data(FILE* stream,  const struct bmp_header* header) // https://ru.wikipedia.org/wiki/BMP#%D0%9F%D1%80%D0%B8%D0%BC%D0%B5%D1%80_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D1%8B_%D0%BD%D0%B0_C, https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/
{
    bool answer = check_stream(stream);
    if(answer == false || header == NULL) return NULL;
    
    struct pixel* check  = malloc(sizeof(struct pixel)*(header->height*header->width));
    fseek(stream, sizeof(struct bmp_header), 0);
    for(uint32_t i = 0; i < header->height; i++)
    {
        for(uint32_t j = 0; j < header -> width; j++)
        {
            fread (&check[i*header->width+j], sizeof (struct pixel), 1, stream);
        }
         fseek (stream,header->width%4, SEEK_CUR);
    }
    //FILE * pFile; check how write pixel in txt file
    //pFile = fopen("data.txt","w");
                //fprintf(pFile,"%d %d   %d %d %d\n", i, j,check[i*header->width+j].red, check[i*header->width+j].green, check[i*header->width+j].blue);		
                //fprintf(pFile,"\n");		
                //fclose(pFile);
    return check;
}
struct bmp_image* read_bmp(FILE* stream) // podpravit code
{
    if(stream == NULL) return NULL;
    struct bmp_image* bmp = calloc(1,sizeof(struct bmp_image));
    bmp -> header = read_bmp_header(stream);
	if(bmp -> header==NULL) 
    {
		fprintf(stderr,"Error: This is not a BMP file.\n");
		free(bmp);
		return NULL;
	}
	bmp->data = read_data(stream, bmp->header);
	if(bmp->data==NULL)
    {
		fprintf(stderr,"Error: Corrupted BMP file.\n");
		free(bmp->header);
		free(bmp);
		return NULL;	
	}
    return bmp;
}		
bool write_bmp(FILE* stream, const struct bmp_image* image) //https://cboard.cprogramming.com/cplusplus-programming/3073-writing-bitmap-files.html optim https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c/
{
    bool answer = check_stream(stream);
    if(answer == false) return false;
    answer = check_image(image);    
    if(answer == false) return false;

    uint32_t height = image->header->height, width = image->header->width;
    fwrite(image->header, sizeof(struct bmp_header), 1, stream);
    const unsigned char padding[3] = {'\0', '\0', '\0'};  
    for (int y = 0; y < height; y++){
    for(uint32_t z = 0;z<width;z++){
             fwrite(&image->data[(y*width)+z],sizeof(struct pixel),1,stream);
        }
        fwrite(padding, sizeof(unsigned char), width%4,stream);
    }
    return true;
}
void free_bmp_image(struct bmp_image* image)
{
    if(image != NULL)
    {
        free(image->header);
        free(image->data);
        free(image);
    }
}

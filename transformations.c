#include "bmp.h"
#include "transformations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

bool null_image(const struct bmp_image* image)
{
    if(image == NULL) return false;
    if(image->header->height < 0) return false;
    if(image->header->width < 0) return false;
    if(image -> header == NULL) return false;
    if(image -> data == NULL) return false;
    return true;
}
struct bmp_image* duplicate (const struct bmp_image* image)
{  
    bool check = null_image(image);
    if(check == false) return NULL;     
    struct bmp_image * bmp = calloc(1,sizeof(struct bmp_image));
	bmp -> header = calloc(1,sizeof(struct bmp_header));
	*bmp->header = *image->header;
	bmp->data =  (struct pixel*)calloc(bmp -> header ->width *bmp -> header -> height, sizeof(struct pixel));
    for(uint32_t i = 0; i < bmp->header->height*bmp->header->width; i++)
    {
        bmp->data[i] = image->data[i];
    }
	return bmp;	
}
struct bmp_image* ver(const struct bmp_image* image)
{
       bool check = null_image(image);
       if(check == false) return NULL;
       struct bmp_image* result = duplicate(image);
       for(int i = 0; i < image->header->height; i++)
        {
            for(int j = 0; j < image->header->width; j++)
            {
                result->data[i*result->header->width+j] = image->data[(image->header->height-1-i)*image->header->width+j];
            }
        }
    return result;
}
struct bmp_image* flip_horizontally(const struct bmp_image* image) //+done
{
    bool check = null_image(image);
    if(check == false) return NULL;
    struct bmp_image *result = duplicate(image);
    struct pixel tmp;
    for(int i = 0; i < image->header->height; i++)
    {
        for(int j = 0; j < image->header->width/2; j++) 
        {
            tmp = result->data[i*image->header->width + j];
            result->data[i*image->header->width + j] = result->data[i*image->header->width + image->header->width-j-1];
            result->data[i*image->header->width + image->header->width-j-1] = tmp;
        }
    }
    return result;
} 
struct bmp_image* flip_vertically(const struct bmp_image* image) // done https://www.andrew.cmu.edu/course/15-123-kesden/spring08/applications/labs/lab4/src/bmplib.c // fix //https://coderoad.ru/19481733/%D0%9A%D0%B0%D0%BA-%D0%BF%D0%BE%D0%B2%D0%B5%D1%80%D0%BD%D1%83%D1%82%D1%8C-bmp-%D0%B8%D0%B7%D0%BE%D0%B1%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D0%B5-%D1%81-%D1%88%D0%B0%D0%B3%D0%BE%D0%BC-90-%D0%B3%D1%80%D0%B0%D0%B4%D1%83%D1%81%D0%BE%D0%B2-%D0%B2-C
{
    bool check = null_image(image);
    if(check == false) return NULL;
    struct bmp_image* new_bmp = ver(image);
    return new_bmp;
}
struct bmp_image* rotate_right(const struct bmp_image* image) //https://coderoad.ru/40621200/C-C-%D0%BF%D0%BE%D0%B2%D0%BE%D1%80%D0%BE%D1%82-%D0%B8%D0%B7%D0%BE%D0%B1%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F-BMP
{
       bool check = null_image(image);
       if(check == false) return NULL;   
       struct bmp_image* step = rotate_left(image);
       struct bmp_image* step_2 = rotate_left(step);             
       free_bmp_image(step);
       struct bmp_image* step_3 = rotate_left(step_2);
       free_bmp_image(step_2);
       return step_3;
}
struct bmp_image* rotate_left(const struct bmp_image* image) //https://coderoad.ru/40621200/C-C-%D0%BF%D0%BE%D0%B2%D0%BE%D1%80%D0%BE%D1%82-%D0%B8%D0%B7%D0%BE%D0%B1%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F-BMP
{
   bool check = null_image(image);
   if(check == false) return NULL;

    struct bmp_image* new_bmp = calloc(1,sizeof(struct bmp_image));
     new_bmp->header= calloc(1, sizeof(struct bmp_header));
    *new_bmp->header=*image->header;
     new_bmp->header->width = image->header->height;
     new_bmp->header->height = image->header->width;

       new_bmp->data= calloc(new_bmp->header->height*new_bmp->header->width, sizeof(struct pixel));
   for(int i = 0; i < image->header->height; i++)
    {
        for(int j = 0; j < image->header->width; j++)
        {
            new_bmp->data[(j*new_bmp->header->width)+(image->header->height-i-1)] = image->data[(i*image->header->width)+j];
        }
    }
    uint32_t padding = (4 - (new_bmp->header->width * sizeof(struct pixel)) % 4) % 4;
    new_bmp -> header->image_size = new_bmp->header->height*(padding+new_bmp->header->width*sizeof(struct pixel));
    new_bmp -> header -> size = new_bmp -> header ->image_size + new_bmp -> header ->offset;
    return new_bmp;
}
struct bmp_image* crop(const struct bmp_image* image, const uint32_t start_y, const uint32_t start_x, const uint32_t height, const uint32_t width)
{
     bool check = null_image(image);
    if(check == false) return NULL; 
    if(start_x < 0 || start_x > image->header->width || start_y < 0 || start_y > image->header->height) return NULL;
    if(height <= 0 || start_y + height > image -> header -> height) return NULL;
    if(width <= 0 || start_x + width > image -> header -> width) return NULL;

    int old_w = image->header->width;
    //struct bmp_image* new_bmp = duplicate(ver(image));
    struct bmp_image* step = ver(image);
    struct bmp_image* new_bmp = duplicate(step);
    free_bmp_image(step);
    new_bmp->header->width = (uint32_t)width;
    new_bmp->header->height = (uint32_t)height;
    int count = 0;
    for(int i = start_y, y = 0; i < start_y+height; i++, y++)
    {
        for(int j = start_x , x = 0; j < start_x+width; j++, x++)
        {
            new_bmp->data[count++] = new_bmp->data[(i*old_w+j)];
        }
    }
    uint32_t padding = (4 - (new_bmp->header->width * sizeof(struct pixel)) % 4) % 4;
    new_bmp->header->image_size = new_bmp->header->height*(padding+new_bmp->header->width*sizeof(struct pixel)); //!!!
    new_bmp -> header -> size = new_bmp -> header->image_size+new_bmp->header->offset;
    struct bmp_image *a = ver(new_bmp);
    free_bmp_image(new_bmp);
    return a;
}
struct bmp_image* scale(const struct bmp_image* image, float factor) // http://www.davdata.nl/math/bmresize.html
{
        if(factor < 0 ) return NULL;
        bool check = null_image(image);
        if(check == false) return NULL;
    struct bmp_image* new_bmp = calloc(1,sizeof(struct bmp_image));
     new_bmp->header= calloc(1, sizeof(struct bmp_header));
    *new_bmp->header=*image->header;
     new_bmp->header->width = image->header->height;
     new_bmp->header->height = image->header->width;
        
        new_bmp->header->width = (uint32_t)round((float)image->header->width*factor);
        new_bmp->header->height = (uint32_t)round((float)image->header->height*factor);
        uint32_t padding = (4 - (new_bmp->header->width * sizeof(struct pixel)) % 4) % 4;
        new_bmp->header->image_size = new_bmp->header->height*(padding+new_bmp->header->width*sizeof(struct pixel));
        new_bmp -> header -> size = new_bmp -> header ->image_size + new_bmp -> header ->offset;
       new_bmp->data=  calloc(1,sizeof(struct pixel)*new_bmp->header->height*new_bmp->header->width);
        int counter = 0;
        for(int i = 0; i < new_bmp->header->height; i++)
        {
            for(int j = 0; j < new_bmp->header->width; j++)
            {
              int step = (int)(j*image->header->width)/new_bmp->header->width;
              int second_step = (int)(i * image->header->height)/new_bmp->header->height;
              new_bmp->data[counter++]=image->data[(second_step*image->header->width)+step];
            }
        }
        return new_bmp;
}
struct bmp_image* extract(const struct bmp_image* image, const char* colors_to_keep)
{
    bool check = null_image(image);
    if(check == false || colors_to_keep == NULL) return NULL;
    bool green = false;
    bool blue = false;
    bool red = false;
    struct bmp_image* new_bmp = calloc(1,sizeof(struct bmp_image));
     new_bmp->header= calloc(1, sizeof(struct bmp_header));
    *new_bmp->header=*image->header;
     new_bmp->data= calloc(new_bmp->header->height*new_bmp->header->width, sizeof(struct pixel));
    
    for(int i = 0; i < strlen(colors_to_keep); i++)
    {
        if(colors_to_keep[i] == 'b') blue = true;
        if(colors_to_keep[i]  == 'g') green = true;
        if(colors_to_keep[i]  == 'r') red = true;
    }
    if(green == false && blue == false && red == false)
    {
        free_bmp_image(new_bmp);
        return NULL;
    }
    for(uint32_t i = 0; i < image->header->height; i++)
    {
        for(uint32_t j = 0; j < image->header->width; j++)
        {
            new_bmp->data[(i*image->header->width)+j] = image->data[(i*image->header->width)+j];
            if(red == false)
            {
                 new_bmp->data[(i*image->header->width)+j].red = 0;
            }
            if(green == false)
            {
                new_bmp->data[(i*image->header->width)+j].green=0;
            }
            if(blue == false)
            {
                new_bmp->data[(i*image->header->width)+j].blue=0;
            }
        }
    }
    return new_bmp;
}

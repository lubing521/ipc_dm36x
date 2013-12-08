#include "alg_aewb_priv.h"
#include "TI_aewb.h"

//#define AEWB_PRINTS

#define TFC_AEW_WIN_HZ_CNT	        32	//12
#define TFC_AEW_WIN_VT_CNT	        12	//16

unsigned char TFC_WEIGHTING_TOP[TFC_AEW_WIN_HZ_CNT * TFC_AEW_WIN_VT_CNT];
unsigned char TFC_WEIGHTING_BOTTOM[TFC_AEW_WIN_HZ_CNT * TFC_AEW_WIN_VT_CNT];
unsigned char TFC_WEIGHTING_LEFT[TFC_AEW_WIN_HZ_CNT * TFC_AEW_WIN_VT_CNT];
unsigned char TFC_WEIGHTING_RIGHT[TFC_AEW_WIN_HZ_CNT * TFC_AEW_WIN_VT_CNT];

unsigned char TFC_WEIGHTING_MATRIX[TFC_AEW_WIN_HZ_CNT * TFC_AEW_WIN_VT_CNT];
unsigned char TFC_WEIGHTING_SPOT[TFC_AEW_WIN_HZ_CNT * TFC_AEW_WIN_VT_CNT];
unsigned char TFC_WEIGHTING_CENTER[TFC_AEW_WIN_HZ_CNT * TFC_AEW_WIN_VT_CNT];

static void generate_windows(int width1, int height1, 
                                    int h_start2, int v_start2, 
                                    int width2, int height2, 
                                    int weight, unsigned char *win_coeffs)
{
    int total_pixels_1, total_pixels_2;
    int pixel_weight1, pixel_weight2;
    int max_weight;
    int shift;
    int i, j;
    float total_weight;

    /* calculate weight for each window */
    total_pixels_2 = width2 * height2;
    total_pixels_1 = width1 * height1 - total_pixels_2;

    pixel_weight1 = (100 - weight) * 65536 / total_pixels_1;
    pixel_weight2 = weight * 65536 / total_pixels_2;

#ifdef AEWB_PRINTS
    OSA_printf("\nweights for each window: %d, %d\n", pixel_weight1, pixel_weight2);
#endif

    /* normalize weights */
    max_weight = pixel_weight1 > pixel_weight2 ? pixel_weight1 : pixel_weight2;
    shift = 0;
    
    while(max_weight >= 255)
    {
        shift ++;
        max_weight = (max_weight + 1) >> 1;
    }
    pixel_weight1 = (pixel_weight1 + (1<<(shift-1))) >> shift;
    pixel_weight2 = (pixel_weight2 + (1<<(shift-1))) >> shift;

    if(pixel_weight2 > pixel_weight1 )
    {
        pixel_weight2 = (pixel_weight1 * total_pixels_1  * weight) / (total_pixels_2 * (100 - weight));
    } 
    else 
    {
        pixel_weight1 = (pixel_weight2 * total_pixels_2  * (100 - weight)) / (total_pixels_1 * weight);
    }
    total_weight = pixel_weight1 * total_pixels_1 + pixel_weight2 * total_pixels_2;

#ifdef AEWB_PRINTS
    OSA_printf("weight = %f, %f\n", pixel_weight1 * total_pixels_1 / total_weight,
                                    pixel_weight2 * total_pixels_2 / total_weight);
    OSA_printf("weights for each window after shifting: %d, %d, %d\n", pixel_weight1, pixel_weight2, shift);
#endif

    for(i = 0; i < height1; i ++)
    {
        for(j = 0; j < width1; j ++) 
        {
            if(i >= v_start2 && i < (v_start2 + height2)
              && j >= h_start2 && j < (h_start2 + width2))
            {
                win_coeffs[i * width1 + j] = pixel_weight2;
            }
            else 
            {
                win_coeffs[i * width1 + j] = pixel_weight1;
            }
        }
    }
}

void printf_tables(unsigned char *tables, int width, int height)
{
    int i, j;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {   
            OSA_printf("%02X,", *(tables + i * width + j));
        }
        OSA_printf("\n");
    }
    OSA_printf("-------------------------------------------------------\n");
}

void TFC_2A_init_tables(int width, int height)
{
#ifdef AEWB_PRINTS
    OSA_printf("TFC 2A weight matrix: width = %d, height = %d\n", width, height);
#endif
    generate_windows(width, height, 0,         0,        width,   height/2, 99, TFC_WEIGHTING_TOP);
    generate_windows(width, height, 0,         0,        width,   height/2,  1, TFC_WEIGHTING_BOTTOM);
    generate_windows(width, height, 0,         0,        width/2,   height, 99, TFC_WEIGHTING_LEFT);
    generate_windows(width, height, 0,         0,        width/2,   height,  1, TFC_WEIGHTING_RIGHT);

    generate_windows(width, height, width*3/8, height/3, width/4, height/4, 80, TFC_WEIGHTING_SPOT);
    generate_windows(width, height, width/4,   height/4, width/2, height/2, 50, TFC_WEIGHTING_CENTER);
    generate_windows(width, height, 0,         0,        width,   height/2, 50, TFC_WEIGHTING_MATRIX);

#if 0
    OSA_printf("TFC_WEIGHTING_TOP\n");    printf_tables(TFC_WEIGHTING_TOP,    width, height);
    OSA_printf("TFC_WEIGHTING_BOTTOM\n"); printf_tables(TFC_WEIGHTING_BOTTOM, width, height);
    OSA_printf("TFC_WEIGHTING_LEFT\n");   printf_tables(TFC_WEIGHTING_LEFT,   width, height);
    OSA_printf("TFC_WEIGHTING_RIGHT\n");  printf_tables(TFC_WEIGHTING_RIGHT,  width, height);

    OSA_printf("TFC_WEIGHTING_SPOT\n");   printf_tables(TFC_WEIGHTING_SPOT,   width, height);
    OSA_printf("TFC_WEIGHTING_CENTER\n"); printf_tables(TFC_WEIGHTING_CENTER, width, height);
    OSA_printf("TFC_WEIGHTING_MATRIX\n"); printf_tables(TFC_WEIGHTING_MATRIX, width, height);
#endif
}


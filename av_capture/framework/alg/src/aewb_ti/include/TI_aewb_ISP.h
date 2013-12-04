#ifndef AEWB_ISP_TI_
#define AEWB_ISP_TI_

#ifdef __cplusplus
extern "C" {
#endif

#include "drv_ipipe.h"

static int RGB2RGB_stablize(int curr_RGB2RGBIndex, int reset)
{
#define AWB_AVG_BUF_LENGTH 12
    static int history_index[AWB_AVG_BUF_LENGTH];
    static int awb_count = 1;
    static int prev_RGB2RGBIndex = 0;

    int i;
    int max;
    int max_index = 0;
    int history_length;
    int index_histogram[NUM_RGB2RGB_MATRIXES];


    if (reset == 1)
    {
        awb_count = 1;
        prev_RGB2RGBIndex = 0;
    }

    if (awb_count < AWB_AVG_BUF_LENGTH)
    {
        history_length = awb_count;
        awb_count++;
    }
    else
    {
        history_length = AWB_AVG_BUF_LENGTH;
    }

    if (history_length == AWB_AVG_BUF_LENGTH)
    {
        for (i = 0; i < history_length - 1; i++)
        {
            history_index[i] = history_index[i + 1];
        }
    }
    history_index[history_length - 1] = curr_RGB2RGBIndex;

    for (i = 0; i < NUM_RGB2RGB_MATRIXES; i++)
    {
        index_histogram[i] = 0;
    }
    for (i = 0; i < history_length; i++)
    {
        index_histogram[history_index[i]]++;
    }

    max = 0;
    for (i = 0; i < NUM_RGB2RGB_MATRIXES; i++)
    {
        if (index_histogram[i] > max)
        {
            max = index_histogram[i];
            max_index = i;
        }
    }

    if (history_length < AWB_AVG_BUF_LENGTH)
    {
        curr_RGB2RGBIndex = max_index;
    }
    else
    {
        if (max * 10 >= (AWB_AVG_BUF_LENGTH * 8))
        {
            curr_RGB2RGBIndex = max_index;
        }
        else
        {
            curr_RGB2RGBIndex = prev_RGB2RGBIndex;
        }
    }
    prev_RGB2RGBIndex = curr_RGB2RGBIndex;

    return (curr_RGB2RGBIndex);
}

int switch_rgb2rgb_matrixes(int colorTemp, struct rgb2rgb_index m[], int reset)
{
    int i, diff, next_diff;
    static int prev_i = -1;

    for (i = 0; i < NUM_RGB2RGB_MATRIXES - 1 && m[i+1].color_temp > 0; i++)
    {
        diff = colorTemp - m[i].color_temp;
        next_diff = m[i + 1].color_temp - colorTemp;
        if ((next_diff >= 0 && diff >= 0) || diff < 0)
        {
            if (prev_i == i) diff -= 200;
            if (prev_i == i+1) next_diff -= 200;
            if (next_diff < diff)
            {
                i++;
            }
            break;
        }
    }
    i = RGB2RGB_stablize(i, reset);

    if (prev_i != i || reset)
    {
        ALG_aewbSetRgb2Rgb(&m[i].rgb2rgbparam);
        ALG_aewbSetRgb2Rgb2(&m[i].rgb2rgb2param);
        prev_i = i;
    }

    return i;
}

#define AC_MAX_DC 80
#define AC_MAX_GN 80
static void auto_contrast(int reset)
{
    static int cnt = 0, scnt = 0;
    static unsigned int chist[256];
    static int dc=0, gain=0;
    CSL_IpipeRgb2YuvConfig rgb2yuv = {
        .matrix = {
            { 0x004d, 0x0096, 0x001d },
            { 0x0fd5, 0x0fab, 0x0080 },
            { 0x0080, 0x0f95, 0x0feb },
        },
        .offset = {
            0x00, 0x80, 0x80
        },

        .cLpfEnable = FALSE,
        .cPos = CSL_IPIPE_YUV_CHROMA_POS_LUM,
    };
    int w, h;
    DRV_ipipeGetFrameSize(&w,&h);
    int np = w*h/4;
    if (reset)
    {
#ifdef _AEWB_DEBUG_PRINT_
        printf("Gang: reset auto_contrast\n");
#endif
		cnt = scnt = dc = gain = 0;
        DRV_ipipeSetRgb2Yuv(&rgb2yuv);
        return;
    }

    int i;
    if (cnt == 0)
    {
        scnt = 0;
        for (i=0;i<256;i++) chist[i]=0;
    }
    if (cnt%2 == 1)
    {
        unsigned int hist[256];
        unsigned char status;
        DRV_ipipeCpyHistData(hist, &status);

        int sum=0;
        for (i=0;i<256;i++) sum += hist[i];

        if (sum == np)
        {
            scnt++;
            for (i=0;i<256;i++) chist[i] += hist[i];
        }
        //printf("sum=%d, np=%d, scnt=%d\n", sum, np, scnt);
    }
    cnt++;
    if (cnt == 2*10)
    {
        if (scnt > 6)
        {
            int low = 0;
            for (i=0; i<8; i++) low += chist[i];
            low /= scnt;
            if (low*50 < np && dc<AC_MAX_DC) dc++;
            if (low*30 > np && dc>0) dc--;

            int high = 0;
            for (i=255; i>=240; i--) high += chist[i];
            high /= scnt;
            if (high*50 < np && gain<AC_MAX_GN) gain++;
            if (high*30 > np && gain>0) gain--;

            int sum = 0;
            for (i=0; i<256; i++) sum += chist[i];
            sum /= scnt;

#ifdef _AEWB_DEBUG_PRINT_
            printf("Gang: low=%d %d%% high=%d %d%% sum=%d, np=%d\n", low, low*100/np, high, high*100/np, sum, np);
            printf("Gang: dc=%d, gain=%d, scnt=%d\n", dc, gain, scnt);
#endif
            //apply dc and gain
            rgb2yuv.matrix[0][0] = 0x004d*(100+gain)/100;
            rgb2yuv.matrix[0][1] = 0x0096*(100+gain)/100;
            rgb2yuv.matrix[0][2] = 0x001d*(100+gain)/100;
            rgb2yuv.offset[0] = -dc;
            DRV_ipipeSetRgb2Yuv(&rgb2yuv);
        }
        cnt = 0;
    }
}

#ifdef __cplusplus
}
#endif

#endif

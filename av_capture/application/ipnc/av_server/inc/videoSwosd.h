#include <alg_ti_swosd.h>

#ifndef _VIDEO_SWOSD_H_
#define _VIDEO_SWOSD_H_

#define SWOSD_WINDOW_ONE        0       	// Logo Window
#define SWOSD_WINDOW_TWO       	1       	// User String "EVALUATION" Window
#define SWOSD_WINDOW_THREE   	2       	// Resolution Window
#define SWOSD_WINDOW_FOUR      	3       	// Codec Type Window
#define SWOSD_WINDOW_FIVE       4       	// Frame Rate Window
#define SWOSD_WINDOW_SIX        5       	// Bit Rate window
#define SWOSD_WINDOW_SEVEN     	6      		// Camera Name Window
#define SWOSD_WINDOW_EIGHT     	7      		// Date Info Window
#define SWOSD_WINDOW_NINE       8      		// Time Info Window
#define SWOSD_WINDOW_TEN        9      		// Auto Focus Value
#define SWOSD_WINDOW_ELEVEN     10      	// Eval Version Pop-up

/*ANR _ FR */
#define SWOSD_WINDOW_FR_ST     11      // revisit
#define SWOSD_WINDOW_FR1       SWOSD_WINDOW_FR_ST+0
#define SWOSD_WINDOW_FR2       SWOSD_WINDOW_FR_ST+1
#define SWOSD_WINDOW_FR3       SWOSD_WINDOW_FR_ST+2
#define SWOSD_WINDOW_FR4       SWOSD_WINDOW_FR_ST+3

#define SWOSD_USER_TRANSPARANCY_ON

#define SWOSD_WINDOW_FR_MAX   		4      	// revisit - Change the value in AvserverConfig.h SWOSD_WINDOW_FR_MAX
#define SWOSD_FR_USRSTR_LEN     	24      // revisit
#define SWOSD_FR_USRSTR_WIDTH    	384  	// 24 * 16 (SWOSD_FR_USRSTR_LEN * SWOSD_CHARWIDTH_770_20) 240
#define SWOSD_FR_USRSTR_HEIGHT   	28   	// 28  (SWOSD_CHARHEIGHT_770_20)  18

typedef struct _FR_USER_INFO
{
	int stX;
	int stY;
	char usrStr[SWOSD_FR_USRSTR_LEN];
	int usrID;
} FR_USER_INFO;

#define SWOSD_TIICON_160    	"/mnt/mmc/TI_Logo_160_64.yyuv420"
#define SWOSD_TIICON_80      	"/mnt/mmc/TI_Logo_80_32.yyuv420"

#define SWOSD_STRINGPATTERN		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz:; '/\\+-_()&$#.0123456789"

#define OSD_YES      	1
#define OSD_NO      	-1
#define STRING_OSD      1
#define IMAGE_OSD       2

#define SUCCESS       	0

#define SWOSD_MAX_STRING_LENGTH  			24

#define SWOSD_FRAMERATE_STR_LEN         	17
#define SWOSD_BITRATE_STR_LEN              	18
#define SWOSD_QUALITY_STR_LEN              	12
#define SWOSD_AUTOFOCUS_STR_LEN            	19
#define SWOSD_DATE_STR_LEN                  15
#define SWOSD_TIME_STR_LEN                  15
#define SWOSD_MAX_DATE_TIME_LENGHT     		15
#define SWOSD_EVAL_STR_LEN                  12

#define SWOSD_FRAMERATE_PREFIX              12
#define SWOSD_BITRATE_PREFIX                9
#define SWOSD_BITRATE_SUFFIX                4
#define SWOSD_QUALITY_PREFIX                9
#define SWOSD_QUALITY_SUFFIX                0
#define SWOSD_AUTOFOCUS_PREFIX              12
#define SWOSD_DATE_PREFIX                   5
#define SWOSD_TIME_PREFIX                   5

#define VIDEO_RESOL_WIDTH_LARGE           	1000
#define VIDEO_RESOL_WIDTH_MEDIUM        	750
#define VIDEO_RESOL_WIDTH_SMALL           	400

#define SWOSD_CHARWIDTHTILOGO_160_64		160
#define SWOSD_CHARHEIGHTTILOGO_160_64		64
#define SWOSD_CHARWIDTHTILOGO_80_32	 	    80
#define SWOSD_CHARHEIGHTTILOGO_80_32       	32

#define SWOSD_OSDWHITETRANSVAL	 	    0x80FF
#define SWOSD_OSDWHITETRANSRANGE 	    0x48
#define SWOSD_OSDBLACKTRANSVAL	 	    0x00
#define SWOSD_OSDBLACKTRANSRANGE 	    0x48

#define SWOSD_CHARWIDTH_1540_40         20
#define SWOSD_CHARHEIGHT_1540_40        38
#define SWOSD_STRINGWIDTH_1540_40     	1540
#define SWOSD_STRINGHEIGHT_1540_40    	38

#define SWOSD_CHARWIDTH_1232_30         16
#define SWOSD_CHARHEIGHT_1232_30        28
#define SWOSD_STRINGWIDTH_1232_30     	1232
#define SWOSD_STRINGHEIGHT_1232_30     	28

#define SWOSD_CHARWIDTH_1078_28        	14
#define SWOSD_CHARHEIGHT_1078_28       	26
#define SWOSD_STRINGWIDTH_1078_28     	1078
#define SWOSD_STRINGHEIGHT_1078_28     	26

#define SWOSD_CHARWIDTH_770_20         	10
#define SWOSD_CHARHEIGHT_770_20         18
#define SWOSD_STRINGWIDTH_770_20       	770
#define SWOSD_STRINGHEIGHT_770_20      	18

#define SWOSD_CHARWIDTH_616_18          8
#define SWOSD_CHARHEIGHT_616_18         16
#define SWOSD_STRINGWIDTH_616_18        616
#define SWOSD_STRINGHEIGHT_616_18       16

#define SWOSD_CHARWIDTH_462_12          6
#define SWOSD_CHARHEIGHT_462_12         12
#define SWOSD_STRINGWIDTH_462_12        462
#define SWOSD_STRINGHEIGHT_462_12       12

typedef struct _DATE_TIME_INFO
{
	int year;
	int month;
	int mday;
	int hours;
	int min;
	int sec;

} DATE_TIME_INFO;

int VIDEO_swosdTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState );

int VIDEO_swosdTskRun(int streamId, OSA_BufInfo *pBufInfo);

#endif

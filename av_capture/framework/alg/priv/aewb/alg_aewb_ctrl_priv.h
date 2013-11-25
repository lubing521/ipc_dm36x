#ifndef _ALG_AEWB_CTRL_PRIV_H_
#define _ALG_AEWB_CTRL_PRIV_H_

typedef	struct _CONTROL3A{
	short	IMAGE_SHARPNESS;
	short   IMAGE_CONTRAST;
	short   IMAGE_BRIGHTNESS;
	short   IMAGE_SATURATION;
	short	IMAGE_BACKLIGHT;
	short   INDOUTDOOR;
	short   VIDEO_MODE;    		/*default  = 0    NTSC */
	short   PAUSE_AWWB;			/*default  = 0	*/
	short   SKIP_BINNING_MODE; 	/*default  = 0    binning mode */
	short	AUTO_IRIS;
	short	DAY_NIGHT;
}CONTROL3AS;

#endif

/*=======================================================================
 *
 *            Texas Instruments Internal Reference Software
 *
 *                           DSPS R&D Center
 *                     Video and Image Processing
 *
 *         Copyright (c) 2009 Texas Instruments, Incorporated.
 *                        All Rights Reserved.
 *
 *
 *          FOR TI INTERNAL USE ONLY. NOT TO BE REDISTRIBUTED.
 *
 *                    TI INTERNAL - TI PROPRIETARY
 *
 *
 *  Contact: Wei Hong (weihong@ti.com)
 *
 *=======================================================================
 *
 *  File: FacetrackerStruct.h
 *
 *=======================================================================
 *
 *
 =======================================================================*/


#ifndef _Facetracker_STRUCT_INCLUDED_

#define _Facetracker_STRUCT_INCLUDED_

#define MAX_FACE_NUMBER		(35)

//Facetracker structure: Contains all Facetracker parameters.
typedef struct {

// Input: Point to current frame
  Uint8    *frmBuffer_1;
// Input: Point to previous frame
  Uint8    *frmBuffer_2; 
// Input: Frame width in pixels
  Uint16   inpImgSizeV;
// Input: Frame height in pixels
  Uint16   inpImgSizeH; 

// Input: Max number of frames during which a face can be tracked
// without any positive detection by the face detector
// This number is usually depends the frame rate
// For 30 fps, setting it to 90 means the tracker will 
// be active for up to 3 seconds if no face is detected
  Uint16   maxNumOfTrackingFrames;
// Input: Amount of expansion applied to the bounding boxes
// in the left, right, top, bottom sides .
// The resulting boxes are called ROI boxes.
// Unit step is 1/8 of bouding box size
// For expandRoiLeft and expandRoiRight, the unit is 1/8 of the face detector bounding box's width.
// For instance, if expandRoiLeft= 2, it means the amount of expansion to the left is 2/8 of the face detector box's width
// For expandRoiTop and expandRoiBottom, the unit is 1/8 of the face detector bounding box's height.
// For instance, if expandRoiTop= 3, it means the amount of expansion to the left is 3/8 of the face detector box's height
  Uint16   expandRoiLeft;
  Uint16   expandRoiRight;
  Uint16   expandRoiTop;
  Uint16   expandRoiBottom;
// Input and output: As input, it is number of faces detected by hardware in current frame
// As output, it is number of hardware detected faces + missing faces recovered by TI_track_face_run().
  Uint16   face_number; 
// Input: position of faces' bounding boxes detected in current frame
// Output: position of faces' bounding boxes returned by TI_face_track_run()
// face_pos[.][0]= Y coordinate of upper left corner of bounding box
// face_pos[.][1]= X coordinate of upper left corner of bounding box
// face_pos[.][2]= Y coordinate of lower right corner of bounding box
// face_pos[.][3]= X coordinate of lower right corner of bounding box
  Uint16   face_pos[MAX_FACE_NUMBER][4];
// Output: position of roi boxes, which are expanded bounding boxes
// face_pos[.][0]= Y coordinate of upper left corner of ROI box
// face_pos[.][1]= X coordinate of upper left corner of ROI box
// face_pos[.][2]= Y coordinate of lower right corner of ROI box
// face_pos[.][3]= X coordinate of lower right corner of ROI box  
  Uint16   roi_pos[MAX_FACE_NUMBER][4];
// Output: motion vector of each face
// mv[.][0]: y coordinate of motion vector
// mv[.][1]: x coordinate of motion vector
  Int16    mv[MAX_FACE_NUMBER][2]; //VC

// Internal: number of faces detected and tracked in previous frame
  Uint16   face_number_previous;
  Uint16   face_pos_previous[MAX_FACE_NUMBER][4];
  Int16    mv_previous[MAX_FACE_NUMBER][2]; //VC
    
  Uint16   face_pos_lastdetect[MAX_FACE_NUMBER][4];

// Output: for each face in current frame, number of times the hardware face detector missed and tracker had to be activated
  Uint16   tracking_counter[MAX_FACE_NUMBER];
// Output: for each face in previous frame, number of times the hardware face detector missed and tracker had to be activated
  Uint16   tracking_counter_previous[MAX_FACE_NUMBER];
// Input: number of pixels that extends the bounding box to make up the search region
  Uint8    searchRange; // VC

} Facetrackerstruct;

void TI_track_face_run(Facetrackerstruct* facetracker);

#endif

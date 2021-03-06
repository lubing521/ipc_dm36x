#include <alg_ti_aewb_priv.h>

awb_calc_data_t awb_calc_data = {
   
// Sensor specific calibration data

   8,			// num_of_ref_1
   3,			// num_of_ref_2
   4,			// num_of_gray

   // color_temp_1[num_of_ref_1]
   {5000,4500,4000,3760,3520,3280,3040,2800},

   // wbReferenceCb[num_of_ref_1][num_of_gray]
   {
       {-42,-41,-41,-36},
       {-52,-50,-49,-44},
       {-62,-60,-58,-53},
       {-58,-56,-55,-50},
       {-63,-62,-60,-55},
       {-69,-67,-66,-61},
       {-74,-73,-71,-66},
       {-79,-78,-76,-71}
   },

   // wbReferenceCr[num_of_ref_1][num_of_gray]
   {
       {-29,-28,-27,-26},
       {-28,-27,-26,-25},
       {-26,-26,-25,-24},
       {-16,-16,-15,-15},
       {-6,-6,-5,-5},
       {4,4,4,4},
       {14,14,14,14},
       {24,24,24,23}
   },

   // ref_gray_R_1[num_of_ref_1][num_of_gray]
   {
       {199,132,75,38},
       {190,126,73,36},
       {180,120,70,34},
       {186,125,73,36},
       {192,129,76,38},
       {198,134,80,39},
       {204,138,83,41},
       {210,143,86,43}
   },

   // ref_gray_G_1[num_of_ref_1][num_of_gray]
   {
       {268,177,100,50},
       {255,169,97,48},
       {242,161,93,45},
       {232,155,90,44},
       {222,149,87,43},
       {212,143,84,41},
       {202,137,81,40},
       {192,131,78,39}
   },

   // ref_gray_B_1[num_of_ref_1][num_of_gray]
   {
       {167,112,64,33},
       {144,97,56,29},
       {120,81,48,25},
       {113,76,45,24},
       {106,72,43,23},
       {98,67,40,21},
       {91,63,38,20},
       {84,58,35,19}
   },

   // ref_index_2[num_of_ref_2]
   {0,2,7},

   // color_temp_2[num_of_ref_2]
   {5000,4000,2800},

   // img_ref[NUM_OF_REF_2*1120]
   {
       //5000
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,5,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,11,
       	5,5,5,8,
       	5,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,3,3,
       	5,5,8,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,11,0,0,
       	5,0,5,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,11,
       	16,0,3,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,5,0,
       	5,13,3,0,
       	0,5,3,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	5,35,11,0,
       	3,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,51,5,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,3,
       	5,11,8,5,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,5,8,
       	5,8,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	11,5,3,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,3,
       	0,3,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,8,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	3,3,0,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,3,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	

       //4000
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,8,
       	3,3,6,6,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,11,
       	8,6,11,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,17,0,3,
       	14,6,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,3,3,
       	6,3,0,6,
       	17,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,25,23,6,
       	11,11,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	3,34,17,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,14,
       	3,8,11,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,3,
       	6,17,6,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	6,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,3,8,
       	6,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,3,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	11,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	

       //2800
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	3,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,8,6,
       	17,11,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,8,23,14,
       	3,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,20,
       	8,0,23,17,
       	6,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,6,0,8,
       	56,17,8,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,3,11,
       	31,14,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,6,6,17,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,3,3,6,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,11,0,8,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,3,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	0,0,0,0,
       	

   },

   // referencesCb_2[num_of_ref_2][num_of_gray]
   {
       {-42,-41,-41,-36},
       {-62,-60,-58,-53},
       {-79,-78,-76,-71}
   },

   // referencesCr_2[num_of_ref_2][num_of_gray]
   {
       {-29,-28,-27,-26},
       {-26,-26,-25,-24},
       {24,24,24,23}
   },

   // ref_gray_R_2[num_of_ref_2][num_of_gray]
   {
       {199,132,75,38},
       {180,120,70,34},
       {210,143,86,43}
   },

   // ref_gray_G_2[num_of_ref_2][num_of_gray]
   {
       {268,177,100,50},
       {242,161,93,45},
       {192,131,78,39}
   },

   // ref_gray_B_2[num_of_ref_2][num_of_gray]
   {
       {167,112,64,33},
       {120,81,48,25},
       {84,58,35,19}
   },

   
// AWB tuning parameters

   50,     // radius

   5,     // luma_awb_min
   250,     // luma_awb_max

   3500,     // low_color_temp_thresh
   0,     // apply_rgb_adjust
   110,     // R_adjust(%)
   90,     // B_adjust(%)

   999999,     // SB_1
   1000000,     // SB_2
   0,     // SB_low_bound

   5000,     // default_T_Hh
   5000,     // default_T_MH
   4000,     // default_T_ML
   2800,     // default_T_L

   0,		// default_T_Hh_index
   0,		// default_T_MH_index
   2,		// default_T_ML_index
   7,		// default_T_L_index

   1,     // best_gray_index_default

};



#ifndef _DRV_IMGS_OV2715_2MP_H_
#define _DRV_IMGS_OV2715_2MP_H_

#include <drv_imgs.h>
#include <drv_i2c.h>

#define IMGS_ROW_START_MIN  (0)
#define IMGS_COL_START_MIN  (0)

#define IMGS_H_PAD          (8)
#define IMGS_V_PAD          (6)

//#define IMGS_W_MAX     (1920)
//#define IMGS_H_MAX     (1080)
#define IMGS_W_MAX     (1280)
#define IMGS_H_MAX     (800)


#define IMGS_MHZ        (1000000L)

#define IMGS_PLL_REF_CLK     (24*IMGS_MHZ)

//#define IMGS_CHIP_ID        (0x2710)
#define IMGS_CHIP_ID        (0xa630)

/***************** OV10630's registers starts *****************/
#define OV10630_REG_0103		0x0103
#define OV10630_REG_301b 		0x301b 
#define OV10630_REG_301c 		0x301c 
#define OV10630_REG_301a 		0x301a 
#define OV10630_REG_300c 		0x300c 
#define OV10630_REG_3021 		0x3021 
#define OV10630_REG_3011 		0x3011 
#define OV10630_REG_6900 		0x6900 //where is it ?
#define OV10630_REG_6901 		0x6901 //where is it ?
#define OV10630_REG_3033 		0x3033 
#define OV10630_REG_3503 		0x3503 
#define OV10630_REG_302d 		0x302d 
#define OV10630_REG_3025 		0x3025 
#define OV10630_REG_3003 		0x3003 
#define OV10630_REG_3004 		0x3004 
#define OV10630_REG_3005 		0x3005 
#define OV10630_REG_3006 		0x3006 
#define OV10630_REG_3600 		0x3600 
#define OV10630_REG_3601 		0x3601 
#define OV10630_REG_3612 		0x3612 
#define OV10630_REG_3611 		0x3611 
#define OV10630_REG_3633 		0x3633 
#define OV10630_REG_3602 		0x3602 
#define OV10630_REG_3603 		0x3603 
#define OV10630_REG_3630 		0x3630 
#define OV10630_REG_3631 		0x3631 
#define OV10630_REG_3714 		0x3714 
#define OV10630_REG_371d 		0x371d 
#define OV10630_REG_4300 		0x4300 
#define OV10630_REG_3007 		0x3007 
#define OV10630_REG_3024 		0x3024 
#define OV10630_REG_3020 		0x3020 
#define OV10630_REG_3702 		0x3702 
#define OV10630_REG_3703 		0x3703 
#define OV10630_REG_3704 		0x3704 
#define OV10630_REG_3709 		0x3709 
#define OV10630_REG_370d 		0x370d 
#define OV10630_REG_3712 		0x3712 
#define OV10630_REG_3713 		0x3713 
#define OV10630_REG_3715 		0x3715 
#define OV10630_REG_381d 		0x381d 
#define OV10630_REG_381c 		0x381c 
#define OV10630_REG_3824 		0x3824 
#define OV10630_REG_3815 		0x3815 
#define OV10630_REG_3804 		0x3804 
#define OV10630_REG_3805 		0x3805 
#define OV10630_REG_3800 		0x3800 
#define OV10630_REG_3801 		0x3801 
#define OV10630_REG_3806 		0x3806 
#define OV10630_REG_3807 		0x3807 
#define OV10630_REG_3802 		0x3802 
#define OV10630_REG_3803 		0x3803 
#define OV10630_REG_3808 		0x3808 
#define OV10630_REG_3809 		0x3809 
#define OV10630_REG_380a 		0x380a 
#define OV10630_REG_380b 		0x380b 
#define OV10630_REG_380c 		0x380c 
#define OV10630_REG_380d 		0x380d 
#define OV10630_REG_380e 		0x380e 
#define OV10630_REG_380f 		0x380f 
#define OV10630_REG_3811 		0x3811 
#define OV10630_REG_381f 		0x381f 
#define OV10630_REG_3621 		0x3621 
#define OV10630_REG_5005 		0x5005 
#define OV10630_REG_56d5 		0x56d5 
#define OV10630_REG_56d6 		0x56d6 
#define OV10630_REG_56d7 		0x56d7 
#define OV10630_REG_56d8 		0x56d8 
#define OV10630_REG_56d9 		0x56d9 
#define OV10630_REG_56da 		0x56da 
#define OV10630_REG_56db 		0x56db 
#define OV10630_REG_56dc 		0x56dc 
#define OV10630_REG_56e8 		0x56e8 
#define OV10630_REG_56e9 		0x56e9 
#define OV10630_REG_56ea 		0x56ea 
#define OV10630_REG_56eb 		0x56eb 
#define OV10630_REG_5100 		0x5100 
#define OV10630_REG_5101 		0x5101 
#define OV10630_REG_5102 		0x5102 
#define OV10630_REG_5103 		0x5103 
#define OV10630_REG_5104 		0x5104 
#define OV10630_REG_5105 		0x5105 
#define OV10630_REG_5106 		0x5106 
#define OV10630_REG_5107 		0x5107 
#define OV10630_REG_5108 		0x5108 
#define OV10630_REG_5109 		0x5109 
#define OV10630_REG_510a 		0x510a 
#define OV10630_REG_510b 		0x510b 
#define OV10630_REG_510c 		0x510c 
#define OV10630_REG_510d 		0x510d 
#define OV10630_REG_510e 		0x510e 
#define OV10630_REG_510f 		0x510f 
#define OV10630_REG_5110 		0x5110 
#define OV10630_REG_5111 		0x5111 
#define OV10630_REG_5112 		0x5112 
#define OV10630_REG_5113 		0x5113 
#define OV10630_REG_5114 		0x5114 
#define OV10630_REG_5115 		0x5115 
#define OV10630_REG_5116 		0x5116 
#define OV10630_REG_5117 		0x5117 
#define OV10630_REG_5118 		0x5118 
#define OV10630_REG_5119 		0x5119 
#define OV10630_REG_511a 		0x511a 
#define OV10630_REG_511b 		0x511b 
#define OV10630_REG_511c 		0x511c 
#define OV10630_REG_511d 		0x511d 
#define OV10630_REG_511e 		0x511e 
#define OV10630_REG_511f 		0x511f 
#define OV10630_REG_56d0 		0x56d0 
#define OV10630_REG_5006 		0x5006 
#define OV10630_REG_5608 		0x5608 
#define OV10630_REG_52d7 		0x52d7 
#define OV10630_REG_528d 		0x528d 
#define OV10630_REG_5293 		0x5293 
#define OV10630_REG_52d3 		0x52d3 
#define OV10630_REG_5288 		0x5288 
#define OV10630_REG_5289 		0x5289 
#define OV10630_REG_52c8 		0x52c8 
#define OV10630_REG_52c9 		0x52c9 
#define OV10630_REG_52cd 		0x52cd 
#define OV10630_REG_5381 		0x5381 
#define OV10630_REG_5382 		0x5382 
#define OV10630_REG_5589 		0x5589 
#define OV10630_REG_558a 		0x558a 
#define OV10630_REG_558b 		0x558b 
#define OV10630_REG_558c 		0x558c 
#define OV10630_REG_558d 		0x558d 
#define OV10630_REG_558e 		0x558e 
#define OV10630_REG_558f 		0x558f 
#define OV10630_REG_5590 		0x5590 
#define OV10630_REG_5591 		0x5591 
#define OV10630_REG_5592 		0x5592 
#define OV10630_REG_55a2 		0x55a2 
#define OV10630_REG_55a3 		0x55a3 
#define OV10630_REG_55a4 		0x55a4 
#define OV10630_REG_55a5 		0x55a5 
#define OV10630_REG_55a6 		0x55a6 
#define OV10630_REG_55a7 		0x55a7 
#define OV10630_REG_55a8 		0x55a8 
#define OV10630_REG_55a9 		0x55a9 
#define OV10630_REG_55aa 		0x55aa 
#define OV10630_REG_55ab 		0x55ab 
#define OV10630_REG_5581 		0x5581 
#define OV10630_REG_5300 		0x5300 
#define OV10630_REG_5301 		0x5301 
#define OV10630_REG_5302 		0x5302 
#define OV10630_REG_5303 		0x5303 
#define OV10630_REG_5304 		0x5304 
#define OV10630_REG_5305 		0x5305 
#define OV10630_REG_5306 		0x5306 
#define OV10630_REG_5307 		0x5307 
#define OV10630_REG_5308 		0x5308 
#define OV10630_REG_5309 		0x5309 
#define OV10630_REG_530a 		0x530a 
#define OV10630_REG_530b 		0x530b 
#define OV10630_REG_530c 		0x530c 
#define OV10630_REG_530d 		0x530d 
#define OV10630_REG_530e 		0x530e 
#define OV10630_REG_530f 		0x530f 
#define OV10630_REG_5310 		0x5310 
#define OV10630_REG_5311 		0x5311 
#define OV10630_REG_5312 		0x5312 
#define OV10630_REG_5313 		0x5313 
#define OV10630_REG_5314 		0x5314 
#define OV10630_REG_5315 		0x5315 
#define OV10630_REG_5316 		0x5316 
#define OV10630_REG_5317 		0x5317 
#define OV10630_REG_5318 		0x5318 
#define OV10630_REG_5319 		0x5319 
#define OV10630_REG_531a 		0x531a 
#define OV10630_REG_531b 		0x531b 
#define OV10630_REG_531c 		0x531c 
#define OV10630_REG_531d 		0x531d 
#define OV10630_REG_531e 		0x531e 
#define OV10630_REG_531f 		0x531f 
#define OV10630_REG_5320 		0x5320 
#define OV10630_REG_5321 		0x5321 
#define OV10630_REG_5322 		0x5322 
#define OV10630_REG_5323 		0x5323 
#define OV10630_REG_5324 		0x5324 
#define OV10630_REG_5325 		0x5325 
#define OV10630_REG_5326 		0x5326 
#define OV10630_REG_5327 		0x5327 
#define OV10630_REG_5609 		0x5609 
#define OV10630_REG_560a 		0x560a 
#define OV10630_REG_560b 		0x560b 
#define OV10630_REG_560c 		0x560c 
#define OV10630_REG_560d 		0x560d 
#define OV10630_REG_560e 		0x560e 
#define OV10630_REG_560f 		0x560f 
#define OV10630_REG_5610 		0x5610 
#define OV10630_REG_5611 		0x5611 
#define OV10630_REG_5612 		0x5612 
#define OV10630_REG_5613 		0x5613 
#define OV10630_REG_5614 		0x5614 
#define OV10630_REG_5615 		0x5615 
#define OV10630_REG_5616 		0x5616 
#define OV10630_REG_5617 		0x5617 
#define OV10630_REG_5618 		0x5618 
#define OV10630_REG_563b 		0x563b 
#define OV10630_REG_563c 		0x563c 
#define OV10630_REG_563d 		0x563d 
#define OV10630_REG_563e 		0x563e 
#define OV10630_REG_563f 		0x563f 
#define OV10630_REG_5640 		0x5640 
#define OV10630_REG_5641 		0x5641 
#define OV10630_REG_5642 		0x5642 
#define OV10630_REG_5643 		0x5643 
#define OV10630_REG_5644 		0x5644 
#define OV10630_REG_5645 		0x5645 
#define OV10630_REG_5646 		0x5646 
#define OV10630_REG_5647 		0x5647 
#define OV10630_REG_5651 		0x5651 
#define OV10630_REG_5652 		0x5652 
#define OV10630_REG_521a 		0x521a 
#define OV10630_REG_521b 		0x521b 
#define OV10630_REG_521c 		0x521c 
#define OV10630_REG_521d 		0x521d 
#define OV10630_REG_521e 		0x521e 
#define OV10630_REG_521f 		0x521f 
#define OV10630_REG_5220 		0x5220 
#define OV10630_REG_5223 		0x5223 
#define OV10630_REG_5225 		0x5225 
#define OV10630_REG_5227 		0x5227 
#define OV10630_REG_5229 		0x5229 
#define OV10630_REG_522b 		0x522b 
#define OV10630_REG_522d 		0x522d 
#define OV10630_REG_522f 		0x522f 
#define OV10630_REG_5241 		0x5241 
#define OV10630_REG_5242 		0x5242 
#define OV10630_REG_5243 		0x5243 
#define OV10630_REG_5244 		0x5244 
#define OV10630_REG_5245 		0x5245 
#define OV10630_REG_5246 		0x5246 
#define OV10630_REG_5247 		0x5247 
#define OV10630_REG_5248 		0x5248 
#define OV10630_REG_524a 		0x524a 
#define OV10630_REG_524c 		0x524c 
#define OV10630_REG_524e 		0x524e 
#define OV10630_REG_5250 		0x5250 
#define OV10630_REG_5252 		0x5252 
#define OV10630_REG_5254 		0x5254 
#define OV10630_REG_5256 		0x5256 
#define OV10630_REG_4605 		0x4605 
#define OV10630_REG_4606 		0x4606 
#define OV10630_REG_4607 		0x4607 
#define OV10630_REG_460a 		0x460a 
#define OV10630_REG_460b 		0x460b 
#define OV10630_REG_460c 		0x460c 
#define OV10630_REG_4620 		0x4620 
#define OV10630_REG_4700 		0x4700 
#define OV10630_REG_4701 		0x4701 
#define OV10630_REG_4702 		0x4702 
#define OV10630_REG_4708 		0x4708 
#define OV10630_REG_4004 		0x4004 
#define OV10630_REG_4005 		0x4005 
#define OV10630_REG_4001 		0x4001 
#define OV10630_REG_4050 		0x4050 
#define OV10630_REG_4051 		0x4051 
#define OV10630_REG_4057 		0x4057 
#define OV10630_REG_405a 		0x405a 
#define OV10630_REG_4202 		0x4202 
#define OV10630_REG_3023 		0x3023 
#define OV10630_REG_0100 		0x0100 
#define OV10630_REG_6f0e 		0x6f0e 
#define OV10630_REG_6f0f 		0x6f0f 
#define OV10630_REG_460e 		0x460e 
#define OV10630_REG_460f 		0x460f 
#define OV10630_REG_4610 		0x4610 
#define OV10630_REG_4611 		0x4611 
#define OV10630_REG_4612 		0x4612 
#define OV10630_REG_4613 		0x4613 
#define OV10630_REG_4605 		0x4605 
#define OV10630_REG_4608 		0x4608 
#define OV10630_REG_4609 		0x4609 
#define OV10630_REG_6804 		0x6804 
#define OV10630_REG_6805 		0x6805 
#define OV10630_REG_6806 		0x6806 
#define OV10630_REG_5120 		0x5120 
#define OV10630_REG_3510 		0x3510 
#define OV10630_REG_3504 		0x3504 
#define OV10630_REG_6800 		0x6800 
#define OV10630_REG_6f0d 		0x6f0d 
#define OV10630_REG_5000 		0x5000 
#define OV10630_REG_5001 		0x5001 
#define OV10630_REG_5002 		0x5002 
#define OV10630_REG_503d 		0x503d 
#define OV10630_REG_c450 		0xc450 
#define OV10630_REG_c452 		0xc452 
#define OV10630_REG_c453 		0xc453 
#define OV10630_REG_c454 		0xc454 
#define OV10630_REG_c455 		0xc455 
#define OV10630_REG_c456 		0xc456 
#define OV10630_REG_c457 		0xc457 
#define OV10630_REG_c458 		0xc458 
#define OV10630_REG_c459 		0xc459 
#define OV10630_REG_c45b 		0xc45b 
#define OV10630_REG_c45c 		0xc45c 
#define OV10630_REG_c45d 		0xc45d 
#define OV10630_REG_c45e 		0xc45e 
#define OV10630_REG_c45f 		0xc45f 
#define OV10630_REG_c460 		0xc460 
#define OV10630_REG_c461 		0xc461 
#define OV10630_REG_c462 		0xc462 
#define OV10630_REG_c464 		0xc464 
#define OV10630_REG_c465 		0xc465 
#define OV10630_REG_c466 		0xc466 
#define OV10630_REG_c467 		0xc467 
#define OV10630_REG_c468 		0xc468 
#define OV10630_REG_c469 		0xc469 
#define OV10630_REG_c46a 		0xc46a 
#define OV10630_REG_c46b 		0xc46b 
#define OV10630_REG_c46c 		0xc46c 
#define OV10630_REG_c46d 		0xc46d 
#define OV10630_REG_c46e 		0xc46e 
#define OV10630_REG_c46f 		0xc46f 
#define OV10630_REG_c47c 		0xc47c 
#define OV10630_REG_c47d 		0xc47d 
#define OV10630_REG_c47e 		0xc47e 
#define OV10630_REG_c47f 		0xc47f 
#define OV10630_REG_c480 		0xc480 
#define OV10630_REG_c481 		0xc481 
#define OV10630_REG_c482 		0xc482 
#define OV10630_REG_c483 		0xc483 
#define OV10630_REG_c484 		0xc484 
#define OV10630_REG_c485 		0xc485 
#define OV10630_REG_c486 		0xc486 
#define OV10630_REG_c487 		0xc487 
#define OV10630_REG_c488 		0xc488 
#define OV10630_REG_c489 		0xc489 
#define OV10630_REG_c48a 		0xc48a 
#define OV10630_REG_c48b 		0xc48b 
#define OV10630_REG_c48c 		0xc48c 
#define OV10630_REG_c48d 		0xc48d 
#define OV10630_REG_c48e 		0xc48e 
#define OV10630_REG_c48f 		0xc48f 
#define OV10630_REG_c490 		0xc490 
#define OV10630_REG_c492 		0xc492 
#define OV10630_REG_c493 		0xc493 
#define OV10630_REG_c498 		0xc498 
#define OV10630_REG_c499 		0xc499 
#define OV10630_REG_c49a 		0xc49a 
#define OV10630_REG_c49b 		0xc49b 
#define OV10630_REG_c49c 		0xc49c 
#define OV10630_REG_c49d 		0xc49d 
#define OV10630_REG_c49e 		0xc49e 
#define OV10630_REG_c49f 		0xc49f 
#define OV10630_REG_c4a0 		0xc4a0 
#define OV10630_REG_c4a1 		0xc4a1 
#define OV10630_REG_c4a2 		0xc4a2 
#define OV10630_REG_c4a3 		0xc4a3 
#define OV10630_REG_c4a4 		0xc4a4 
#define OV10630_REG_c4a5 		0xc4a5 
#define OV10630_REG_c4a6 		0xc4a6 
#define OV10630_REG_c4a7 		0xc4a7 
#define OV10630_REG_c4a8 		0xc4a8 
#define OV10630_REG_c4a9 		0xc4a9 
#define OV10630_REG_c4aa 		0xc4aa 
#define OV10630_REG_c4ab 		0xc4ab 
#define OV10630_REG_c4ac 		0xc4ac 
#define OV10630_REG_c4ad 		0xc4ad 
#define OV10630_REG_c4b4 		0xc4b4 
#define OV10630_REG_c4b5 		0xc4b5 
#define OV10630_REG_c4b6 		0xc4b6 
#define OV10630_REG_c4b7 		0xc4b7 
#define OV10630_REG_c4b8 		0xc4b8 
#define OV10630_REG_c4b9 		0xc4b9 
#define OV10630_REG_c4ba 		0xc4ba 
#define OV10630_REG_c4bb 		0xc4bb 
#define OV10630_REG_c4be 		0xc4be 
#define OV10630_REG_c4bf 		0xc4bf 
#define OV10630_REG_c4c8 		0xc4c8 
#define OV10630_REG_c4c9 		0xc4c9 
#define OV10630_REG_c4ca 		0xc4ca 
#define OV10630_REG_c4cb 		0xc4cb 
#define OV10630_REG_c4cc 		0xc4cc 
#define OV10630_REG_c4cd 		0xc4cd 
#define OV10630_REG_c4ce 		0xc4ce 
#define OV10630_REG_c4cf 		0xc4cf 
#define OV10630_REG_c4d0 		0xc4d0 
#define OV10630_REG_c4d1 		0xc4d1 
#define OV10630_REG_c4e0 		0xc4e0 
#define OV10630_REG_c4e1 		0xc4e1 
#define OV10630_REG_c4e2 		0xc4e2 
#define OV10630_REG_c4e4 		0xc4e4 
#define OV10630_REG_c4e5 		0xc4e5 
#define OV10630_REG_c4e6 		0xc4e6 
#define OV10630_REG_c4e7 		0xc4e7 
#define OV10630_REG_c4e8 		0xc4e8 
#define OV10630_REG_c4e9 		0xc4e9 
#define OV10630_REG_c4ea 		0xc4ea 
#define OV10630_REG_c4eb 		0xc4eb 
#define OV10630_REG_c4ec 		0xc4ec 
#define OV10630_REG_c4ed 		0xc4ed 
#define OV10630_REG_c4ee 		0xc4ee 
#define OV10630_REG_c4ef 		0xc4ef 
#define OV10630_REG_c4f0 		0xc4f0 
#define OV10630_REG_c4f1 		0xc4f1 
#define OV10630_REG_c4f2 		0xc4f2 
#define OV10630_REG_c4f3 		0xc4f3 
#define OV10630_REG_c4f4 		0xc4f4 
#define OV10630_REG_c4f5 		0xc4f5 
#define OV10630_REG_c4f6 		0xc4f6 
#define OV10630_REG_c4f7 		0xc4f7 
#define OV10630_REG_c4f8 		0xc4f8 
#define OV10630_REG_c4f9 		0xc4f9 
#define OV10630_REG_c4fa 		0xc4fa 
#define OV10630_REG_c4fb 		0xc4fb 
#define OV10630_REG_c4fc 		0xc4fc 
#define OV10630_REG_c4fd 		0xc4fd 
#define OV10630_REG_c4fe 		0xc4fe 
#define OV10630_REG_c4ff 		0xc4ff 
#define OV10630_REG_c500 		0xc500 
#define OV10630_REG_c501 		0xc501 
#define OV10630_REG_c502 		0xc502 
#define OV10630_REG_c503 		0xc503 
#define OV10630_REG_c504 		0xc504 
#define OV10630_REG_c505 		0xc505 
#define OV10630_REG_c506 		0xc506 
#define OV10630_REG_c507 		0xc507 
#define OV10630_REG_c508 		0xc508 
#define OV10630_REG_c509 		0xc509 
#define OV10630_REG_c50a 		0xc50a 
#define OV10630_REG_c50b 		0xc50b 
#define OV10630_REG_c50c 		0xc50c 
#define OV10630_REG_c50d 		0xc50d 
#define OV10630_REG_c50e 		0xc50e 
#define OV10630_REG_c50f 		0xc50f 
#define OV10630_REG_c510 		0xc510 
#define OV10630_REG_c511 		0xc511 
#define OV10630_REG_c512 		0xc512 
#define OV10630_REG_c513 		0xc513 
#define OV10630_REG_c514 		0xc514 
#define OV10630_REG_c515 		0xc515 
#define OV10630_REG_c518 		0xc518 
#define OV10630_REG_c519 		0xc519 
#define OV10630_REG_c51a 		0xc51a 
#define OV10630_REG_c51b 		0xc51b 
#define OV10630_REG_c2e0 		0xc2e0 
#define OV10630_REG_c2e1 		0xc2e1 
#define OV10630_REG_c2e2 		0xc2e2 
#define OV10630_REG_c2e3 		0xc2e3 
#define OV10630_REG_c2e4 		0xc2e4 
#define OV10630_REG_c2e5 		0xc2e5 
#define OV10630_REG_c2e9 		0xc2e9 
#define OV10630_REG_c2ea 		0xc2ea 
#define OV10630_REG_c2eb 		0xc2eb 
#define OV10630_REG_c2ed 		0xc2ed 
#define OV10630_REG_c2ee 		0xc2ee 
#define OV10630_REG_c2ef 		0xc2ef 
#define OV10630_REG_c308 		0xc308 
#define OV10630_REG_c309 		0xc309 
#define OV10630_REG_c30a 		0xc30a 
#define OV10630_REG_c30c 		0xc30c 
#define OV10630_REG_c30d 		0xc30d 
#define OV10630_REG_c30e 		0xc30e 
#define OV10630_REG_c30f 		0xc30f 
#define OV10630_REG_c310 		0xc310 
#define OV10630_REG_c311 		0xc311 
#define OV10630_REG_c312 		0xc312 
#define OV10630_REG_c313 		0xc313 
#define OV10630_REG_c314 		0xc314 
#define OV10630_REG_c315 		0xc315 
#define OV10630_REG_c316 		0xc316 
#define OV10630_REG_c317 		0xc317 
#define OV10630_REG_c318 		0xc318 
#define OV10630_REG_c319 		0xc319 
#define OV10630_REG_c31a 		0xc31a 
#define OV10630_REG_c31b 		0xc31b 
#define OV10630_REG_c31c 		0xc31c 
#define OV10630_REG_c31d 		0xc31d 
#define OV10630_REG_c31e 		0xc31e 
#define OV10630_REG_c31f 		0xc31f 
#define OV10630_REG_c320 		0xc320 
#define OV10630_REG_c321 		0xc321 
#define OV10630_REG_c322 		0xc322 
#define OV10630_REG_c323 		0xc323 
#define OV10630_REG_c324 		0xc324 
#define OV10630_REG_c325 		0xc325 
#define OV10630_REG_c326 		0xc326 
#define OV10630_REG_c327 		0xc327 
#define OV10630_REG_c328 		0xc328 
#define OV10630_REG_c329 		0xc329 
#define OV10630_REG_c32a 		0xc32a 
#define OV10630_REG_c32b 		0xc32b 
#define OV10630_REG_c32c 		0xc32c 
#define OV10630_REG_c32d 		0xc32d 
#define OV10630_REG_c32e 		0xc32e 
#define OV10630_REG_c32f 		0xc32f 
#define OV10630_REG_c330 		0xc330 
#define OV10630_REG_c331 		0xc331 
#define OV10630_REG_c332 		0xc332 
#define OV10630_REG_c333 		0xc333 
#define OV10630_REG_c334 		0xc334 
#define OV10630_REG_c335 		0xc335 
#define OV10630_REG_c336 		0xc336 
#define OV10630_REG_c337 		0xc337 
#define OV10630_REG_c338 		0xc338 
#define OV10630_REG_c339 		0xc339 
#define OV10630_REG_c33a 		0xc33a 
#define OV10630_REG_c33b 		0xc33b 
#define OV10630_REG_c33c 		0xc33c 
#define OV10630_REG_c33d 		0xc33d 
#define OV10630_REG_c33e 		0xc33e 
#define OV10630_REG_c33f 		0xc33f 
#define OV10630_REG_c340 		0xc340 
#define OV10630_REG_c341 		0xc341 
#define OV10630_REG_c342 		0xc342 
#define OV10630_REG_c343 		0xc343 
#define OV10630_REG_c344 		0xc344 
#define OV10630_REG_c345 		0xc345 
#define OV10630_REG_c346 		0xc346 
#define OV10630_REG_c347 		0xc347 
#define OV10630_REG_c348 		0xc348 
#define OV10630_REG_c349 		0xc349 
#define OV10630_REG_6700 		0x6700 
#define OV10630_REG_6701 		0x6701 
#define OV10630_REG_6702 		0x6702 
#define OV10630_REG_6703 		0x6703 
#define OV10630_REG_6704 		0x6704 
#define OV10630_REG_6705 		0x6705 
#define OV10630_REG_6706 		0x6706 
#define OV10630_REG_6708 		0x6708 
#define OV10630_REG_3822 		0x3822 
#define OV10630_REG_6f06 		0x6f06 
#define OV10630_REG_6f07 		0x6f07 
#define OV10630_REG_6f0a 		0x6f0a 
#define OV10630_REG_6f0b 		0x6f0b 
#define OV10630_REG_6f00 		0x6f00 
#define OV10630_REG_3042 		0x3042 
#define OV10630_REG_301b  		0x301b  
#define OV10630_REG_301c 		0x301c 
#define OV10630_REG_301a 		0x301a 


/*SC control registers */
#define SYSTEM_CONTROL00 		0x3008 
#define CHIP_VERSION_H 			0x300A 
#define CHIP_VERSION_L 			0x300B 
#define MIPI_CTRL00 				0x300E 
#define PLL_CTRL00 					0x300F 
#define PLL_CTRL01 					0x3010 
#define PLL_CTRL02 					0x3011 
#define PLL_PREDIVIDER 			0x3012 
#define PAD_OUTPUT_ENABLE00	0x3016 
#define PAD_OUTPUT_ENABLE01	0x3017 
#define PAD_OUTPUT_ENABLE02	0x3018 
#define PAD_OUTPUT_VALUE00	0x3019 
#define PAD_OUTPUT_VALUE01	0x301A 
#define PAD_OUTPUT_VALUE02	0x301B 
#define PAD_OUTPUT_SELECT00	0x301C 
#define PAD_OUTPUT_SELECT01	0x301D 
#define PAD_OUTPUT_SELECT02	0x301E 
#define PAD_OUTPUT_DRIVE_C	0x302C 
#define PAD_INPUT_VALUE00 	0x3040 
#define PAD_INPUT_VALUE01 	0x3041 
#define PAD_INPUT_VALUE02 	0x3042 

/* SCCB control registers */
#define SCCB_ID									0x3100 
#define PLL_CLOCK_SELECT 				0x3103 
#define SCCB_PAD_CLOCK_DIVIDER 	0x3104

/* group sharing registers */
#define GROUP_ADDR0 				0x3200 
#define GROUP_ADDR1 				0x3201 
#define GROUP_ADDR2 				0x3202 
#define GROUP_ADDR3 				0x3203 
#define GROUP_ACCESS				0x3212 

/* analog registers */
#define ANA_ARRAY						0x3621        
#define SENSOR_REG0D				0x370D 


/* unknow register*/
#define SENSOR_RESV1				0x3706
#define SENSOR_RESV2				0x3712
#define SENSOR_RESV3				0x3630
#define SENSOR_RESV4				0x3604
#define SENSOR_RESV5				0x3603
#define SENSOR_RESV6				0x3631
#define SENSOR_RESV7				0x3600
#define SENSOR_RESV8				0x3620
#define SENSOR_RESV9				0x3623
#define SENSOR_RESV10				0x3702
#define SENSOR_RESV11				0x3703
#define SENSOR_RESV12				0x3704
#define SENSOR_RESV13				0x3713
#define SENSOR_RESV14				0x3714
#define SENSOR_RESV15				0x3710
#define SENSOR_RESV16				0x3605
#define SENSOR_RESV17				0x3606
#define SENSOR_RESV18				0x302d
#define SENSOR_RESV19				0x370b
#define SENSOR_RESV20				0x4301
#define SENSOR_RESV21				0x4303
#define SENSOR_RESV22				0x3A1A
#define SENSOR_RESV23				0x5688
#define SENSOR_RESV24				0x3716
#define SENSOR_RESV25				0x382E
#define SENSOR_RESV26				0x381A









/* timing control registers */
#define TIMING_CONTROL_HS_HIGHBYTE   	0x3800         
#define TIMING_CONTROL_HS_LOWBYTE    	0x3801         
#define TIMING_CONTROL_VS_HIGHBYTE   	0x3802         
#define TIMING_CONTROL_VS_LOWBYTE   	0x3803         
#define TIMING_CONTROL_HW_HIGHBYTE   	0x3804         
#define TIMING_CONTROL_HW_LOWBYTE    	0x3805         
#define TIMING_CONTROL_VH_HIGHBYTE   	0x3806         
#define TIMING_CONTROL_VH_LOWBYTE   	0x3807         
#define TIMING_CONTROL_DVP_HSIZE_HIGH 0x3808         
#define TIMING_CONTROL_DVP_HSIZE_LOW 	0x3809		   
#define TIMING_CONTROL_DVP_VSIZE_HIGH 0x380A		   
#define TIMING_CONTROL_DVP_VSIZE_LOW 	0x380B		   
#define TIMING_CONTROL_HTS_HIGHBYTE  	0x380C          
#define TIMING_CONTROL_HTS_LOWBYTE   	0x380D          
#define TIMING_CONTROL_VTS_HIGHBYTE  	0x380E          
#define TIMING_CONTROL_VTS_LOWBYTE   	0x380F         
#define TIMING_CONTROL_HV_OFFSET 			0x3810         
#define TIMING_CONTROL18 							0x3818 

/* AEC/AGC registers */
#define AEC_PK_EXPO1			0x3500 
#define AEC_PK_EXPO2			0x3501 
#define AEC_PK_EXPO3			0x3502 
#define AEC_PK_MANUAL			0x3503 
#define AEC_AGC_ADJ_H			0x350A 
#define AEC_AGC_ADJ_L 		0x350B 
#define AEC_PK_VTS_H			0x350C 
#define AEC_PK_VTS_L			0x350D 
#define AEC_CONTROL0 			0x3A00 
#define AEC_CONTROL1 			0x3A01 
#define AEC_MAX_EXPO_60_1	0x3A02  
#define AEC_MAX_EXPO_60_2	0x3A03  
#define AEC_MAX_EXPO_60_3	0x3A04 
#define AEC_B50_STEP_H		0x3A08 
#define AEC_B50_STEP_L 		0x3A09 
#define AEC_B60_STEP_H 		0x3A0A 
#define AEC_B60_STEP_L 		0x3A0B 
#define AEC_CONTROLD 			0x3A0D 
#define AEC_CONTROLE 			0x3A0E 
#define AEC_CONTROLF 			0x3A0F 
#define AEC_CONTROL10			0x3A10 
#define AEC_CONTROL11 		0x3A11 
#define AEC_CONTROL12 		0x3A12 
#define AEC_CONTROL13 		0x3A13 
#define AEC_MAX_EXPO50_1	0x3A14 
#define AEC_MAX_EXPO50_2 	0x3A15 
#define AEC_MAX_EXPO50_3 	0x3A16 
#define AEC_CONTROL17			0x3A17 
#define AEC_G_CEIL_H 			0x3A18 
#define AEC_G_CEIL_L 			0x3A19 
#define AEC_CONTROL1B 		0x3A1B 
#define AEC_LED_ADD_ROW_H	0x3A1C 
#define AEC_LED_ADD_ROW_L 0x3A1D 
#define AEC_CONTROL1E			0x3A1E  
#define AEC_CONTROL1F			0x3A1F  
#define AEC_CONTROL20			0x3A20

/* BLC control */
#define BLC_CONTROL_00				0x4000         
#define BLC_CONTROL_01				0x4001         
#define BLC_CONTROL_02				0x4002         
#define BLACK_LEVEL_TARGET_H	0x4006		   
#define BLACK_LEVEL_TARGET_L	0x4007		   
#define BLC_FRAME_CONTROL			0x401D


/*  DVP registers */
#define DVP_CTRL00	 		0x4700          
#define DVP_CTRL01 			0x4704          
#define DVP_CTRL02 			0x4708          
#define DVP_CTRL03 			0x4709 
#define POLARITY_CTRL00	0x4740


/*  ISP control registers  */  
#define ISP_CONTROL0 		0x5000        
#define ISP_CONTROL2 		0x5002        
#define ISP_CONTROL5 		0x5005        
#define ISP_CONTROL31		0x501F        
#define ISP_TEST				0x503D        
#define ISP_SENSOR_BIAS 0x503E		  
#define ISP_SENSOR_GAIN 0x503F		  

/*  AWB control registers  */
#define AWB_GAIN_PK_RED_H					0x3400		
#define AWB_GAIN_PK_RED_L					0x3401		
#define AWB_GAIN_PK_GREEN_H				0x3402		
#define AWB_GAIN_PK_GREEN_L				0x3403		
#define AWB_GAIN_PK_BLUE_H				0x3404		
#define AWB_GAIN_PK_BLUE_L				0x3405		
#define AWB_GAIN_PK_AWB 					0x3406		
#define AWB_CONTROL_00   					0x5180      
#define AWB_CONTROL_01   					0x5181      
#define AWB_CONTROL_02   					0x5182      
#define STABLE_RANGE 							0x5183		
#define RED_GAIN_LIMIT						0x518C      
#define GREEN_GAIN_LIMIT					0x518D      
#define BLUE_GAIN_LIMIT						0x518E      
#define AWB_FRAME_COUNTER					0x518F		
#define RED_BEFORE_GAIN_AVERAGE		0x5196		
#define GREEN_BEFORE_GAIN_AVERAGE	0x5197		
#define BLUE_BEFORE_GAIN_AVERAGE	0x5198		
#define RED_AFTER_GAIN_AVERAGE_H	0x5199		
#define RED_AFTER_GAIN_AVERAGE_L	0x519A		
#define GREEN_AFTER_GAIN_AVERAGE_H	0x519B		
#define GREEN_AFTER_GAIN_AVERAGE_L	0x519C		
#define BLUE_AFTER_GAIN_AVERAGE_H		0x519D		
#define BLUE_AFTER_GAIN_AVERAGE_L		0x519E	


/* AVG registers  */
#define AVG_START_POSITION_AT_HORIZONTAL_H 	0x5680		
#define AVG_START_POSITION_AT_HORIZONTAL_L 	0x5681		
#define AVG_END_POSITION_AT_HORIZONTAL_H 		0x5682		
#define AVG_END_POSITION_AT_HORIZONTAL_L 		0x5683		
#define AVG_START_POSITION_AT_VERTICAL_H		0x5684		
#define AVG_START_POSITION_AT_VERTICAL_L		0x5685		
#define AVG_END_POSITION_AT_VERTICAL_H 			0x5686      
#define AVG_END_POSITION_AT_VERTICAL_L     	0x5687       
#define AVG_R10 														0x5690   


#define IMGS_I2C_ADDR       (0x6e)

#ifdef BOARD_AP_IPNC
//#define IMGS_I2C_ADDR       (0x6C)
#endif

#ifdef BOARD_TI_EVM
//#define IMGS_I2C_ADDR       (0x6C)
#endif

#define IMGS_RESET_GPIO     (31)

//Added by Richard: adding registers to update awb & color matrix
#define OV10630_REG_5580 	0x5580
#define OV10630_REG_5582 	0x5582
#define OV10630_REG_5583 	0x5583
#define OV10630_REG_5584 	0x5584
#define OV10630_REG_5585 	0x5585
#define OV10630_REG_5586 	0x5586
#define OV10630_REG_5587    0x5587
#define OV10630_REG_5588	0x5588
#define OV10630_REG_5593	0x5593
#define OV10630_REG_5594	0x5594
#define OV10630_REG_5595	0x5595
#define OV10630_REG_5596	0x5596
#define OV10630_REG_5597	0x5597
#define OV10630_REG_559e	0x559e
#define OV10630_REG_559f	0x559f
#define OV10630_REG_55a0	0x55a0
#define OV10630_REG_55a1	0x55a1
#define OV10630_REG_55ac	0x55ac
#define OV10630_REG_55ad	0x55ad
#define OV10630_REG_55ae	0x55ae


typedef struct {

  int fps;        // Hz
  float t_frame;  // ns
  float t_row;    // ns
  float t_pclk;   // ns
  int W;
  int H;
  int SW;
  int SD;
  int HB;
  int VB;
  int HBmin;
  int VBmin;
  int f_pclk;     // Hz

  float shutterOverhead;

  int col_size;
  int row_size;
  int col_skip;
  int row_skip;
  int col_bin;
  int row_bin;
  int col_start;
  int row_start;

  int pll_M;
  int pll_N;
  int pll_DIV;

  int div_base;

} DRV_ImgsFrameTime;

typedef struct {

  DRV_I2cHndl i2cHndl;

  DRV_ImgsFrameTime curFrameTime;
  DRV_ImgsModeConfig curModeConfig;

} DRV_ImgsObj;


extern DRV_ImgsObj gDRV_imgsObj;

extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_736x480_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1280x736_2_VS;

// for TNF+SNF cases
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_768x512_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_0_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_1_VS;
extern DRV_ImgsLdcConfig    gDRV_imgsLdcConfig_1312x768_2_VS;

extern DRV_ImgsIsifConfig   gDRV_imgsIsifConfig_Common;

extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Vnfdemo;
extern DRV_ImgsIpipeConfig  gDRV_imgsIpipeConfig_Common;

extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Appro;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_TI;
extern DRV_ImgsH3aConfig    gDRV_imgsH3aConfig_Common;

int DRV_imgsReset();

int DRV_imgsCalcFrameTime(Uint32 fps, Uint32 dataW, Uint32 dataH, Bool binEnable, Bool flipH, Bool flipV);
int DRV_imgsCalcSW(int exposureTimeInUsecs);
int DRV_imgsCalcAgain(int aGain);
int DRV_imgsCalcFrameRate(int fps);
int DRV_imgsGetEshutter(Uint32 *eshutterInUsec);
int DRV_imgsGetAgain(int *again);

int DRV_imgsCheckId();
int DRV_imgsSetRegs();

int DRV_imgsAPChipInit(int *pGioC1, int *pGioD1);
int DRV_SetAfPosition(unsigned short position);


#endif


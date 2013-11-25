
#include <drv_imgs.h>

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_736x480_0 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 12,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 736/2, 
      .lensCenterY         = 480/2,  
      .kvh                 = 175,  
      .kvl                 = 175,  
      .khr                 = 175, 
      .khl                 = 175, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_736x480_0.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_736x480_1 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 9,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 288/2, 
      .lensCenterY         = 192/2,  
      .kvh                 = 145,  
      .kvl                 = 145,  
      .khr                 = 145, 
      .khl                 = 145, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_736x480_1.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_736x480_0_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 12,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 864/2, 
      .lensCenterY         = 576/2,  
      .kvh                 = 128,  
      .kvl                 = 128,  
      .khr                 = 128, 
      .khl                 = 128, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_736x480_0_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_736x480_1_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 10,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 352/2, 
      .lensCenterY         = 256/2,  
      .kvh                 = 205,  
      .kvl                 = 205,  
      .khr                 = 205, 
      .khl                 = 205, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_736x480_1_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1280x736_0 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 14,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 1280/2, 
      .lensCenterY         = 736/2,  
      .kvh                 = 215,  
      .kvl                 = 215,  
      .khr                 = 215, 
      .khl                 = 215, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1280x736_0.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1280x736_1 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 10,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 320/2, 
      .lensCenterY         = 192/2,  
      .kvh                 = 220,  
      .kvl                 = 220,  
      .khr                 = 220, 
      .khl                 = 220, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1280x736_1.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1280x736_2 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 12,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 640/2, 
      .lensCenterY         = 352/2,  
      .kvh                 = 210,  
      .kvl                 = 210,  
      .khr                 = 210, 
      .khl                 = 210, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1280x736_2.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1280x736_0_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 9,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 1536/2, 
      .lensCenterY         = 896/2,  
      .kvh                 = 133,  
      .kvl                 = 133,  
      .khr                 = 133, 
      .khl                 = 133, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1280x736_0_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1280x736_1_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 9,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 384/2, 
      .lensCenterY         = 256/2,  
      .kvh                 = 133,  
      .kvl                 = 133,  
      .khr                 = 133, 
      .khl                 = 133, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1280x736_1_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1280x736_2_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 9,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 768/2, 
      .lensCenterY         = 448/2,  
      .kvh                 = 133,  
      .kvl                 = 133,  
      .khr                 = 133, 
      .khl                 = 133, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1280x736_2_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_768x512_0 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 12,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 768/2, 
      .lensCenterY         = 512/2,  
      .kvh                 = 163,  
      .kvl                 = 163,  
      .khr                 = 163, 
      .khl                 = 163, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_768x512_0.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_768x512_1 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 10,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 320/2, 
      .lensCenterY         = 224/2,  
      .kvh                 = 242,  
      .kvl                 = 242,  
      .khr                 = 242, 
      .khl                 = 242, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_768x512_1.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_768x512_0_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 13,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 928/2, 
      .lensCenterY         = 640/2,  
      .kvh                 = 228,  
      .kvl                 = 228,  
      .khr                 = 228, 
      .khl                 = 228, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_768x512_0_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_768x512_1_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 10,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 384/2, 
      .lensCenterY         = 288/2,  
      .kvh                 = 176,  
      .kvl                 = 176,  
      .khr                 = 176, 
      .khl                 = 176, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_768x512_1_VS.txt"
    },
  },
};


DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1312x768_0 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 14,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 1312/2, 
      .lensCenterY         = 768/2,  
      .kvh                 = 215,  
      .kvl                 = 215,  
      .khr                 = 215, 
      .khl                 = 215, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1312x768_0.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1312x768_1 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 10,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 352/2, 
      .lensCenterY         = 244/2,  
      .kvh                 = 220,  
      .kvl                 = 220,  
      .khr                 = 220, 
      .khl                 = 220, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1312x768_1.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1312x768_2 = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 12,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 672/2, 
      .lensCenterY         = 384/2,  
      .kvh                 = 210,  
      .kvl                 = 210,  
      .khr                 = 210, 
      .khl                 = 210, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1312x768_2.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1312x768_0_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 9,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 1600/2, 
      .lensCenterY         = 928/2,  
      .kvh                 = 133,  
      .kvl                 = 133,  
      .khr                 = 133, 
      .khl                 = 133, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1312x768_0_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1312x768_1_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 9,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 448/2, 
      .lensCenterY         = 288/2,  
      .kvh                 = 133,  
      .kvl                 = 133,  
      .khr                 = 133, 
      .khl                 = 133, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1312x768_1_VS.txt"
    },
  },
};

DRV_ImgsLdcConfig gDRV_imgsLdcConfig_1312x768_2_VS = {

  .ldcParams = {
  
    .ldcSetup = {
      .enable              = FALSE,
      .ldMapEnable         = TRUE,
      .inputType           = CSL_LDC_INPUT_MODE_YUV422_LD,
      .bayerType           = CSL_LDC_BAYER_FORMAT_UNPACKED_12_BIT, 
      .bayerStartPhase     = CSL_LDC_BAYER_START_PHASE_B, 
      .interpolationTypeY  = CSL_LDC_Y_INTERPOLATION_TYPE_BICUBIC, 
      .backMapRightShift   = 9,
      .backMapThreshold    = 0,
      .table               = NULL, 
      .lensCenterX         = 832/2, 
      .lensCenterY         = 480/2,  
      .kvh                 = 133,  
      .kvl                 = 133,  
      .khr                 = 133, 
      .khl                 = 133, 
      .affineTranswrap = {
          4096, 
          0,
          0,
          0,
          4096,
          0,
      },
      .frameConfig         = NULL,    
    },
  
   .ldcTable = {
      #include "ldc_table_1312x768_2_VS.txt"
    },
  },
};



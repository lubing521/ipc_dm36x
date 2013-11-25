#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/codec_engine_2_24/examples;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/dm365_codecs_01_00_06/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/codec_engine_2_24/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/xdais_6_24/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/framework_components_2_25_00_04/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/edma3_lld_1_06_00_01/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/linuxutils_2_24_03/packages
override XDCROOT = /home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/xdctools_3_15_01_59
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/codec_engine_2_24/examples;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/dm365_codecs_01_00_06/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/codec_engine_2_24/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/xdais_6_24/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/framework_components_2_25_00_04/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/edma3_lld_1_06_00_01/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/linuxutils_2_24_03/packages;/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/dvsdk_2_10_01_18/xdctools_3_15_01_59/packages;..
HOSTOS = Linux
endif


include $(BASE_DIR)/COMMON_HEADER.MK

LIBS=$(LIB_DIR)/moduletest.a $(LIB_DIR)/alg.a $(LIB_DIR)/drv.a $(LIB_DIR)/alg.a $(LIB_DIR)/image_tune.a $(LIB_DIR)/osa.a $(LIB_DIR)/csl.a $(CMEM_LIB)
LIBS+=$(CODEC_LIBS)
LIBS+=$(DM365MM_LIB)

include $(BASE_DIR)/COMMON_FOOTER.MK

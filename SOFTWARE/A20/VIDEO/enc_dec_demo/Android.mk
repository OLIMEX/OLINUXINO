LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

CEDARX_PATH:=/home/yaosen/exdroid_a20/android/frameworks/av/media/CedarX-Projects/CedarX/

##A31 A20 use suxi mem 
CEDARX_USE_SUNXI_MEM_ALLOCATOR:=N
CEDARX_CHIP_VERSION:=F23

LOCAL_SRC_FILES := \
			main.c \
			capture/capture.c \
			decode/decode_api.c \
			render/render.c \

			
LOCAL_C_INCLUDES := $(KERNEL_HEADERS) \
			$(LOCAL_PATH) \
			$(LOCAL_PATH)/capture \
			$(LOCAL_PATH)/decode \
			$(LOCAL_PATH)/render \
			$(CEDARX_PATH)/include \
			$(CEDARX_PATH)/include/include_platform/CHIP_$(CEDARX_CHIP_VERSION) \
			$(CEDARX_PATH)/include/include_platform/CHIP_$(CEDARX_CHIP_VERSION)/disp \
			$(CEDARX_PATH)/include/include_cedarv \
			$(CEDARX_PATH)/include/include_vencoder \
			$(CEDARX_PATH)/include/include_camera

LOCAL_LDFLAGS += -L$(LOCAL_PATH)

LOCAL_SHARED_LIBRARIES := libcutils libstdc++ libc \
			libutils \
			liblog \

LOCAL_LDLIBS := -ldl 
LOCAL_CFLAGS += -Wfatal-errors -fno-short-enums -D__OS_ANDROID
LOCAL_ARM_MODE := arm	

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := enc_test

LOCAL_SHARED_LIBRARIES := \
						  libcedarv_adapter \
						  libcedarv_base \
						  libcedarxosal \
						  libve \
						  libcedarv
						  
ifeq ($(CEDARX_USE_SUNXI_MEM_ALLOCATOR),Y)
LOCAL_SHARED_LIBRARIES += \
						 libsunxi_alloc \
						 libaw_h264enc
						 
LOCAL_CFLAGS += -DUSE_SUNXI_MEM_ALLOCATOR						 
endif						    

include $(BUILD_EXECUTABLE)

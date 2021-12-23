LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := RainCityGamingTECH

LOCAL_CFLAGS := -Wno-error=format-security -fpermissive -w -s -Werror -Wold-style-cast -fms-extensions -Wno-narrowing
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fvisibility=hidden -ffunction-sections -fdata-sections -stdlib=libc++
LOCAL_CPPFLAGS += -fvisibility=hidden -ffunction-sections -fdata-sections -w -std=c++14
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := src/Main/main.cpp \
                   src/Socket/ClientSocket.cpp \

LOCAL_CPP_INCLUDES += $(LOCAL_PATH)
LOCAL_CPP_INCLUDES += $(LOCAL_PATH)/src/MainLoader
LOCAL_CPP_INCLUDES += $(LOCAL_PATH)/src/Socket

LOCAL_LDLIBS := -llog -landroid
#LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_SHARED_LIBRARY)
#$(call import-module,android/native_app_glue)

include $(CLEAR_VARS)
LOCAL_MODULE := libPUBGMDaemon
LOCAL_CPPFLAGS += -pie -fPIE -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_LDFLAGS += -pie -fPIE -Wl,--gc-sections
LOCAL_CFLAGS := -Wno-error=format-security -fpermissive
LOCAL_CFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_CFLAGS += -fno-rtti -fno-exceptions
LOCAL_CFLAGS += -DNDEBUG
LOCAL_SRC_FILES := src/Daemon/main.cpp
LOCAL_CPP_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lz -llog
include $(BUILD_EXECUTABLE)
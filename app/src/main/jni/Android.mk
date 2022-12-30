# 指定LOCAL_PATH变量，用于查找源文件
LOCAL_PATH:=$(call my-dir) # 语句的意思是将LOCAL_PATH变量定义成本文件所在目录路径。

include $(CLEAR_VARS)
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -mfloat-abi=soft
endif

# 编译和连接参数
LOCAL_CFLAGS += -std=c++11
LOCAL_LDLIBS += -llog -landroid

# 源文件
LOCAL_SRC_FILES:= interface.cpp

# 本地依赖库文件
# LOCAL_SHARED_LIBRARIES := ijkffmpeg ijksdl # .so文件
# LOCAL_STATIC_LIBRARIES := android-ndk-profiler # ijksoundtouch # .a文件

LOCAL_MODULE:= interface
include $(BUILD_SHARED_LIBRARY)
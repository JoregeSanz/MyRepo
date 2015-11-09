LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Card.cpp \
                   ../../Classes/CardGroup.cpp \
                   ../../Classes/CardTable.cpp \
                   ../../Classes/Foundation.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/MovingCardGroup.cpp \
                   ../../Classes/StockRedeal.cpp \
                   ../../Classes/StockTemp.cpp \
                   ../../Classes/Utils.cpp \
                   ../../Classes/AndroidBridge.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../cocos2d/cocos/ui

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

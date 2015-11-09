#include "AppDelegate.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#include "AndroidBridge.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

void cocos_android_app_init(JNIEnv* env, jobject thiz) {
	LOGD("cocos_android_app_init");
	AppDelegate *pAppDelegate = new AppDelegate();
}

///////////////////////////////  native method  ///////////////////////////////////

extern "C" {
	void Java_org_cocos2dx_cpp_AppActivity_onQuitAndStartGame(JNIEnv *env, jobject thiz) {
		AndroidBridge::getInstance()->onQuitAndStartGame();
	}

	void Java_org_cocos2dx_cpp_AppActivity_onRestartGame(JNIEnv *env, jobject thiz) {
		AndroidBridge::getInstance()->onRestartGame();
	}

	void Java_org_cocos2dx_cpp_AppActivity_onSettings(JNIEnv *env, jobject thiz, jint nGeneral, jint nWallpaper, jint nCardStyle, jint nAdvanced, jint nNewGameStart) {
		AndroidBridge::getInstance()->onSetGame(nGeneral, nWallpaper, nCardStyle, nAdvanced, nNewGameStart);
	}

	void Java_org_cocos2dx_cpp_AppActivity_onChangedOrientation(JNIEnv *env, jobject thiz, jboolean isPortrait, jfloat w, jfloat h) {
		AndroidBridge::getInstance()->onChangedOrientation(isPortrait, w, h);
	}

	void Java_org_cocos2dx_cpp_AppActivity_saveState(JNIEnv *env, jobject thiz) {
		AndroidBridge::getInstance()->saveGameState();
	}
}

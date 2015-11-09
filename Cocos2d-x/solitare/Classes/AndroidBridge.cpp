/**
 * AndroidBridge cpp file
 *  author : LightSky
 *  since : 2015/02/18
 **/

#include "AndroidBridge.h"

#include "GameScene.h"
#include "Utils.h"

#define DELAY_SEC 0.1

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#define ANDROID_PACKAGE_CLASS "org/cocos2dx/cpp/AppActivity"
#endif

static AndroidBridge* s_instance = nullptr;

AndroidBridge* AndroidBridge::getInstance()
{
	if( !s_instance )
		s_instance = new AndroidBridge();

	return s_instance;
}

void AndroidBridge::setGameScene(GameScene* gameScene)
{
	m_GameScene = gameScene;
}

void AndroidBridge::onNewGame(std::string info, std::string result)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t, ANDROID_PACKAGE_CLASS, "showNewGameDialog", "(Ljava/lang/String;Ljava/lang/String;)V")) {

			jstring jinfo = t.env->NewStringUTF(info.c_str());
			jstring jresult = t.env->NewStringUTF(result.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jinfo, jresult);
			t.env->DeleteLocalRef(t.classID);
		}
	#endif
}

void AndroidBridge::didGameWon(std::string info, std::string result)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t, ANDROID_PACKAGE_CLASS, "showGameWonDialog", "(Ljava/lang/String;Ljava/lang/String;)V")) {

			jstring jinfo = t.env->NewStringUTF(info.c_str());
			jstring jresult = t.env->NewStringUTF(result.c_str());
			t.env->CallStaticVoidMethod(t.classID, t.methodID, jinfo, jresult);
			t.env->DeleteLocalRef(t.classID);
		}
	#endif
}

void AndroidBridge::onSetting(std::string filepath)
{
	log(" ## onSetting ##\n");

	int nGeneral = Utils::share()->getSettingsGeneral();
	int nWallpaper = Utils::share()->getWallpaperNumber();
	int nCard = Utils::share()->getSettingsCardStyle();
	int nAdvanced = Utils::share()->getAdvancedSettings();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t, ANDROID_PACKAGE_CLASS, "showSettingActivity", "(Ljava/lang/String;IIII)V")) {

			jstring jfilepath = t.env->NewStringUTF(filepath.c_str());
			t.env->CallStaticObjectMethod(t.classID, t.methodID, jfilepath, nGeneral, nWallpaper, nCard, nAdvanced);
			//res = (jint)t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	#endif
}

void AndroidBridge::onHighScores()
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t, ANDROID_PACKAGE_CLASS, "showHighScoresView", "()V")) {

			t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	#endif
}

void AndroidBridge::notifyNoWinnable()
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		JniMethodInfo t;

		if (JniHelper::getStaticMethodInfo(t, ANDROID_PACKAGE_CLASS, "showNoWinnableMessage", "()V")) {

			t.env->CallStaticObjectMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
	#endif
}

void AndroidBridge::onQuitAndStartGame()
{
	Utils::SetNewGame(true);
	Utils::SetKeepGame(false);
	onSetGame();
}

void AndroidBridge::onRestartGame()
{
	log(" ## onRestartGame() ##\n");
	Utils::SetNewGame(false);
	Utils::SetKeepGame(false);
	onSetGame();
}

void AndroidBridge::onSetGame(int nGeneral, int nWallpaper, int nCardStyle, int nAdvanced, int nNewGameStart)
{
	log(" ## onSetGame , %d, %d, %d, %d ##\n", nGeneral, nWallpaper, nCardStyle, nAdvanced);
	Utils::share()->setSettings(nGeneral, nWallpaper, nCardStyle, nAdvanced);

	if( nNewGameStart == 0 )
		Utils::SetKeepGame(true);
	else
		Utils::SetKeepGame(false);

//	m_GameScene->setWallpaper();	//for test
	onSetGame();
}

void AndroidBridge::onChangedOrientation(bool isPortrait, float w, float h)
{
	Utils::SetOrientation(isPortrait);

	/////////////////////////////
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview)
	{
		glview = GLViewImpl::create("My Game");
		director->setOpenGLView(glview);
	}

	log( " bridge / onChangedOrientaion %f, %f", w, h );

	//set design resolution
	glview->setFrameSize(w, h);

//	cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) w, (int) h);

//	if( isPortrait )
//	{
//		glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::NO_BORDER);
//	}
//	else
//	{
//		glview->setDesignResolutionSize(DESIGN_WIDTH_LANDSCAPE, DESIGN_HEIGHT_LANDSCAPE, ResolutionPolicy::NO_BORDER);
//	}

	glview->setViewPortInPoints(0, 0, w, h);
	glview->setScissorInPoints(0, 0, w, h);
	Rect rc = glview->getViewPortRect();
	log( " ## %f, %f, %f, %f ##\n", rc.origin.x, rc.origin.y, rc.size.width, rc.size.height );

	//////////////////////////////
	onSetGame();
}

void AndroidBridge::onSetGame()
{
	CallFunc * callback = CallFunc::create(CC_CALLBACK_0(AndroidBridge::replaceScene, this));
	Sequence * seq = Sequence::create(DelayTime::create(DELAY_SEC), callback , NULL);
	runAction(seq);

	Director::getInstance()->getActionManager()->resumeTarget( AndroidBridge::getInstance() );
}

void AndroidBridge::replaceScene()
{
	auto scene = GameScene::createScene();
	TextureCache::getInstance()->removeUnusedTextures();
	Director::getInstance()->replaceScene(scene);
}


void AndroidBridge::saveGameState()
{
	Utils::share()->saveGameState();
}

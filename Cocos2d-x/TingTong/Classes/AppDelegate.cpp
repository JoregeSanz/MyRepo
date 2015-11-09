#include "AppDelegate.h"
#include "MainMenu.h"
#include "LogoScene.h"
#include "GameSelectScene.h"
#include "CarGame.h"
#include "StatisticsScene.h"
#include "NinjaScene.h"

USING_NS_CC;

float g_fResScale = 1.0;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float fx = winSize.width / DESIGN_RESOLUTION_WIDTH;
    float fy = winSize.height / DESIGN_RESOLUTION_HEIGHT;
    if (fx > fy)
        g_fResScale = fx;
    else
        g_fResScale = fy;
    
    // create a scene. it's an autorelease object
    // run
    
    CCUserDefault* userdata = CCUserDefault::sharedUserDefault();
    if (!userdata->getIntegerForKey(LEVEL))
    {
        userdata->setIntegerForKey(LEVEL, 1);
    }
    
    CCScene *pScene = LogoScene::scene();
    pDirector->runWithScene(pScene);
//    pDirector->runWithScene(CarGame::scene());
//    pDirector->runWithScene(NinjaScene::scene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

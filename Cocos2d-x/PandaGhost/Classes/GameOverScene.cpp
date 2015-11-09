//
//  GameOverScene.cpp
//  PacMan
//
//  Created by jyn on 2/1/14.
//
//

#include "GameOverScene.h"

#include "SimpleAudioEngine.h"
#include "Config.h"
#include "GameScene.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"


#define main_activity_name "com/panda/ghost/PandaGhost"

//#import "MyGameBridge.h"

extern int g_nCurLevel;
extern int g_nTotalScore;

using namespace cocos2d;
using namespace CocosDenshion;
void addFadeOutEffect(CCSprite* sprite,
                   
                   const ccColor3B& colour, const CCSize& size)

{
    
    CCPoint pos = ccp(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    CCSprite* glowSprite = CCSprite::create("particle-fire.png");
    glowSprite->setScaleX(size.width);
    glowSprite->setScaleY(size.height);
    glowSprite->setColor(colour);
    glowSprite->setPosition(pos);
    glowSprite->setRotation(sprite->getRotation());
    
    _ccBlendFunc f = {GL_ONE, GL_ONE};
    glowSprite->setBlendFunc(f);
    sprite->addChild(glowSprite, -1);

    glowSprite->runAction(CCFadeOut::create(2));
    
}

CCScene* GameOverScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameOverScene *layer = GameOverScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // Set scale ratio
    Config::setScaleFactor(winSize);
        
    m_spGameOver = CCSprite::create("gameover_title.png");
    Config::setScale(m_spGameOver);
    m_spGameOver->setAnchorPoint(ccp(0.5,0));
    float titleY = winSize.height - m_spGameOver->getContentSize().height * m_spGameOver->getScaleY();
    m_spGameOver->setPosition( ccp(winSize.width/2, titleY) );
    this->addChild(m_spGameOver, 0);
    addFadeOutEffect(m_spGameOver, ccc3(0,174,239), CCSizeMake(3, 3));
    

    CCSprite* pRectangle = CCSprite::create("gameover_retangle.png");
    Config::setScale(pRectangle);
    pRectangle->setAnchorPoint(ccp(0.5,1));
    
    float rectangleY = pRectangle->getContentSize().height * pRectangle->getScaleY();
    pRectangle->setPosition( ccp(winSize.width/2, rectangleY) );
    this->addChild(pRectangle, 0);
    
    CCSprite* angelGhost = CCSprite::create("angelghost.png");
    Config::setScale(angelGhost);
    angelGhost->setScaleX(angelGhost->getScaleX() * 0.7);
    angelGhost->setScaleY(angelGhost->getScaleY() * 0.7);

    float angleY = (titleY - rectangleY) / 2 + rectangleY;
    angelGhost->setPosition( ccp(winSize.width/2, angleY) );
    this->addChild(angelGhost, 0);

    CCMenuItemImage *pPlayAgain = CCMenuItemImage::create(
                                                          "playagain.png",
                                                          "playagain.png",
                                                          this,
                                                          menu_selector(GameOverScene::onPlayAgain) );
    Config::setScale(pPlayAgain);
    
    float againY = rectangleY - pRectangle->getContentSize().height * 0.44 * pRectangle->getScaleY();
    pPlayAgain->setPosition( ccp(winSize.width / 2, againY) );
    
    float yButton = pPlayAgain->getPositionY() - pPlayAgain->getContentSize().height/2 * pPlayAgain->getScaleY() - pPlayAgain->getContentSize().height * pPlayAgain->getScaleY() / 2;
    CCMenuItemImage *pFacebook = CCMenuItemImage::create(
                                                          "facebook.png",
                                                          "facebook.png",
                                                          this,
                                                          menu_selector(GameOverScene::onFacebook) );
    Config::setScale(pFacebook);
    pFacebook->setScaleX(pFacebook->getScaleX()*0.4);
    pFacebook->setScaleY(pFacebook->getScaleY()*0.4);

    yButton -= pFacebook->getContentSize().height/2 * pFacebook->getScaleY();
    pFacebook->setPosition( ccp(winSize.width * 0.38, yButton) );
    
    CCMenuItemImage *pTwitter = CCMenuItemImage::create(
                                                          "twitter.png",
                                                          "twitter.png",
                                                          this,
                                                          menu_selector(GameOverScene::onTwitter) );
    Config::setScale(pTwitter);
    pTwitter->setPosition( ccp(winSize.width * 0.62, yButton) );
    pTwitter->setScaleX(pTwitter->getScaleX()*0.4);
    pTwitter->setScaleY(pTwitter->getScaleY()*0.4);
   
//    CCMenuItemImage *pGoogle = CCMenuItemImage::create(
//                                                          "google.png",
//                                                          "google.png",
//                                                          this,
//                                                          menu_selector(GameOverScene::onGoogle) );
//    Config::setScale(pGoogle);
//    pGoogle->setScaleX(pGoogle->getScaleX()*0.4);
//    pGoogle->setScaleY(pGoogle->getScaleY()*0.4);
//    pGoogle->setPosition( ccp(winSize.width / 4 * 3 - winSize.width * 0.05, yButton) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pPlayAgain, pFacebook, pTwitter, /*pGoogle,*/ NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SND_NEON,true);
    
    return true;
}
void GameOverScene::onPlayAgain(CCObject* pSender)
{
    g_nCurLevel = 0;
//added by dukce.pak
    g_nTotalScore = 0;
    GameScene::SetLives(Init_LifeNum);
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playEffect(SND_COINDROP);
    
    scheduleOnce(schedule_selector(GameOverScene::replaceGameScene), 1);
    
}
void GameOverScene::replaceGameScene()
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());

}
void GameOverScene::onFacebook(CCObject* pSender)
{
//    MyGameBride::shared()->facebook(g_nTotalScore);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, main_activity_name, "onReportMessageToFB", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, g_nTotalScore);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
    
}
void GameOverScene::onTwitter(CCObject* pSender)
{
//    MyGameBride::shared()->twitter(g_nTotalScore);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, main_activity_name, "onReportMessageToTwitter", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, g_nTotalScore);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
void GameOverScene::onGoogle(CCObject* pSender)
{
    
}

GameOverScene::~GameOverScene()
{
    removeAllChildrenWithCleanup(true);
}

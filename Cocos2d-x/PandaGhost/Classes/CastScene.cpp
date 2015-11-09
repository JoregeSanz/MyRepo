//
//  CastScene.cpp
//  PacMan
//
//  Created by jyn on 2/2/14.
//
//

#include "CastScene.h"

#include "SimpleAudioEngine.h"
#include "Config.h"
#include "GameScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

const char* castTitle[5] = {
    "..........Pac-Panda ",
    "................chachi ",
    ".............chancho ",
    "....................bert ",
    "...................terry ",
};
CCScene* CastScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CastScene *layer = CastScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

CastScene::~CastScene()
{
    removeAllChildrenWithCleanup(true);
}


// on "init" you need to initialize your instance
bool CastScene::init()
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
//    CCLabelTTF* lbCast = CCLabelTTF::create("Cast", "Marker Felt", 70);
    
    CCSprite * lbCast = CCSprite::create("cast_title.png");
    Config::setScale(lbCast);
    lbCast->setPosition(ccp(winSize.width * 0.5, (winSize.height / 10 * 8.7)));
    this->addChild(lbCast, 0);
    
    CCSprite* skel = CCSprite::create("skel.png");
    Config::setScale(skel);
    skel->setScaleX(skel->getScaleX() * 0.8);
    skel->setScaleY(skel->getScaleY() * 0.8);
    
    skel->setPosition(ccp(winSize.width * 0.5, (winSize.height / 10 * 1.5)));
    this->addChild(skel, 0);

    for (int i = 0; i < 5; i++) {
        char temp[20] = {0};
        sprintf(temp, "cast%d.png", i);
        CCSprite* cast = CCSprite::create(temp);
        Config::setScale(cast);
        cast->setScaleX(cast->getScaleX() * 0.4);
        cast->setScaleY(cast->getScaleY() * 0.4);

        float y = (winSize.height / 10 * (7.5 - i*1.1));
        cast->setPosition(ccp(winSize.width * 0.25, y));
        this->addChild(cast, 0);

        m_lbCast[i] = CCLabelTTF::create("", "Marker Felt.ttf", 50);
        Config::setScale(m_lbCast[i]);
        m_lbCast[i]->setPosition(ccp(winSize.width * 0.34, y));
        m_lbCast[i]->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_lbCast[i]->setAnchorPoint(ccp(0, 0.7));
        this->addChild(m_lbCast[i], 0);

    }
    frame = 0;
    schedule(schedule_selector(CastScene::animationText), 0.05f);
    return true;
}
void CastScene::animationText()
{
    frame++;
    
    if (frame > 33) {
        unschedule(schedule_selector(CastScene::animationText));
        schedule(schedule_selector(CastScene::finish), 1.0f);
    }
    for (int i = 0; i < 5; i++) {
        
        if (strlen(castTitle[i]) > frame) {
            char temp[50] = {0};
            memcpy(temp, castTitle[i], frame);
            m_lbCast[i]->setString(temp);
            
            
        }
    }

}
void CastScene::finish()
{
    unschedule(schedule_selector(CastScene::finish));

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* back = CCSprite::create("scoring.png");
    Config::setScale(back);
    back->setPosition(ccp(winSize.width * 0.5, winSize.height / 2));
    this->addChild(back, 0);
   
//    CCLabelTTF* lbCast = CCLabelTTF::create("Scoring", "Marker Felt", 70);
//    CCSprite* lbCast = CCSprite::create("scoring_title.png");
//    Config::setScale(lbCast);
//    lbCast->setPosition(ccp(winSize.width * 0.5, (winSize.height / 10 * 9)));
//    this->addChild(lbCast, 0);

//    CCSprite* skel = CCSprite::create("skel.png");
//    Config::setScale(skel);
//    skel->setScaleX(skel->getScaleX() * 0.8);
//    skel->setScaleY(skel->getScaleY() * 0.8);
//    
//    skel->setPosition(ccp(winSize.width * 0.5, (winSize.height / 10 * 1.5)));
//    this->addChild(skel, 0);

    schedule(schedule_selector(CastScene::goGameScene), 2.0f);

}
void CastScene::goGameScene()
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());

}
void CastScene::draw(void)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    ccDrawSolidRect(CCPoint(0, 0), CCPoint(winSize.width, winSize.height), ccc4f(35/255.f, 31/255.f, 32/255.f, 1.f));
}
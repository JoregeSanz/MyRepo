//
//  LogoScene.cpp
//  TingTong
//
//  Created by jyn on 4/10/14.
//
//

#include "LogoScene.h"

#include "Config.h"
#include "Define.h"
#include "ClipNode.h"
#include "MainMenu.h"

CCScene* LogoScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LogoScene *layer = LogoScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LogoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    Config::setScaleFactor(winSize);
    
    // Set background
    CCLayer* logo = CCLayer::create();
    logo->setAnchorPoint(ccp(0, 0));
    logo->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.32));
    this->addChild(logo);
    logo->setScale(g_fResScale);
    
    CCSprite* sp1= CCSprite::create("splash1.png");
//    sp1->setOpacity(0);
//    sp1->runAction(CCSequence::create(CCFadeIn::create(0.3),
//                                      CCScaleTo::create(0.1, 1.1),
//                                      CCScaleTo::create(0.1, 1.0),
//                                      NULL));
    logo->addChild(sp1, 1);
    
    ClipNode *clip=new ClipNode();
    clip->setClipsToBounds(true);
    clip->setClippingRegion(CCRect(0, 220,  winSize.width, winSize.height)); //in Points
    logo->addChild(clip);
    
    
    CCSprite* sp2= CCSprite::create("splash2.png");
    sp2->setPosition(ccp(10, -250));
    sp2->runAction(CCSequence::create(CCDelayTime::create(0.1),
                                      CCMoveTo::create(0.15, ccp(10, -20)),
                                      NULL));
    clip->addChild(sp2, 0);

    CCSprite* sp3= CCSprite::create("splash3.png");
    sp3->setPosition(ccp(-40, 90));
    sp3->setOpacity(0);
    sp3->runAction(CCSequence::create(CCDelayTime::create(0.2),
                                      CCFadeIn::create(0.1),
                                      CCScaleTo::create(0.1, 1.2),
                                      CCScaleTo::create(0.1, 1.0),
                                      NULL));
    logo->addChild(sp3, 2);

    CCSprite* spLearn = CCSprite::create("splash_learn.png");
    spLearn->setPosition(ccp(-100, 450));
    spLearn->setOpacity(0);
    spLearn->runAction(CCSequence::create(CCDelayTime::create(0.8),
                                      CCFadeIn::create(0.1),
                                      CCScaleTo::create(0.1, 1.2),
                                      CCScaleTo::create(0.2, 0.8),
                                      CCScaleTo::create(0.1, 1.0),
                                      NULL));
    logo->addChild(spLearn);
    
    CCSprite* spWith = CCSprite::create("splash_with.png");
    spWith->setPosition(ccp(300, 300));
    spWith->setOpacity(0);
    spWith->runAction(CCSequence::create(CCDelayTime::create(1.3),
                                          CCFadeIn::create(0.1),
                                          CCScaleTo::create(0.1, 1.2),
                                         CCScaleTo::create(0.2, 0.8),
                                         CCScaleTo::create(0.1, 1.0),
                                          NULL));
    logo->addChild(spWith);

    CCSprite* spShine = CCSprite::create("shine_gold.png");
    spShine->setPosition(ccp(500, 566));
    spShine->setOpacity(0);
    spShine->runAction(CCSequence::create(CCDelayTime::create(1.8),
                                         CCFadeIn::create(0.1),
                                          CCRotateTo::create(2.5, 360*3),
                                         NULL));
    logo->addChild(spShine);
    
    spShine = CCSprite::create("shine_gold.png");
    spShine->setPosition(ccp(333, 600));
    spShine->setOpacity(0);
    spShine->runAction(CCSequence::create(CCDelayTime::create(1.9),
                                         CCFadeIn::create(0.1),
                                         CCScaleTo::create(0.1, 1.2),
                                          CCRotateTo::create(2.5, 360*3),
                                         NULL));
    logo->addChild(spShine);

    spShine = CCSprite::create("shine_gold.png");
    spShine->setPosition(ccp(246, 400));
    spShine->setOpacity(0);
    spShine->runAction(CCSequence::create(CCDelayTime::create(2.1),
                                         CCFadeIn::create(0.1),
                                          CCRotateTo::create(2.5, 360*3),
                                         NULL));
    logo->addChild(spShine);

    scheduleOnce(schedule_selector(LogoScene::mainMenu), 3);
    return true;
}
void LogoScene::mainMenu()
{
    CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
 
}

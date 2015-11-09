//
//  StatisticsScene.cpp
//  TingTong
//
//  Created by jyn on 4/12/14.
//
//

#include "StatisticsScene.h"
#include "MainMenu.h"

const char* lpStatistics[4] = {"LEVEL:", "WORDS / SENTENCES MEMORIZED:", "GPA:", "STARS COLLECTED:"};
CCScene* StatisticsScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StatisticsScene *layer = StatisticsScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StatisticsScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    // Set background
    setBackground(1);
    
    CCSprite* sp = CCSprite::create("texttable1.png");
    sp->setPosition(ccp(0, 250 * g_fResScale));
    sp->setScale(g_fResScale);
    m_bgLayer->addChild(sp);
    
    CCSize szShadow = CCSize(10 * g_fResScale, -10 * g_fResScale);
    CCLabelTTF* lb = CCLabelTTF::create("OVERALL STATISTICS", "Luckiest Guy", 50);
    lb->enableShadow(szShadow, 1, 1, true);
    lb->setAnchorPoint(ccp(0.5, 0.5));
    lb->setPosition(ccp(0, 440 * g_fResScale));
    lb->setScale(g_fResScale);
    m_bgLayer->addChild(lb);

    for (int i = 0; i < 4; i++) {
        
        CCLabelTTF* lb = CCLabelTTF::create(lpStatistics[i], "Luckiest Guy", 50);
        lb->enableShadow(szShadow, 1, 1, true);
        lb->setAnchorPoint(ccp(0, 0.5));
        lb->setPosition(ccp(-560 * g_fResScale, (360 - i * 70) * g_fResScale));
        lb->setScale(g_fResScale);
        m_bgLayer->addChild(lb);

    }
    CCSprite* sp1 = CCSprite::create("texttable2.png");
    sp1->setPosition(ccp(0, -250 * g_fResScale));
    sp1->setScale(g_fResScale);
    m_bgLayer->addChild(sp1);

    CCLabelTTF* lb1 = CCLabelTTF::create("PROGRESS COMPARED TO LAST WEEK", "Luckiest Guy", 50);
    lb1->enableShadow(szShadow, 1, 1, true);
    lb1->setAnchorPoint(ccp(0.5, 0.5));
    lb1->setPosition(ccp(0, -60 * g_fResScale));
    lb1->setScale(g_fResScale);
    m_bgLayer->addChild(lb1);

    for (int i = 0; i < 4; i++) {
        
        CCLabelTTF* lb = CCLabelTTF::create(lpStatistics[i], "Luckiest Guy", 50);
        lb->enableShadow(szShadow, 1, 1, true);
        lb->setAnchorPoint(ccp(0, 0.5));
        lb->setPosition(ccp(-560 * g_fResScale, (-140 - i * 70) * g_fResScale));
        lb->setScale(g_fResScale);
        m_bgLayer->addChild(lb);
        
    }

    return true;
}
void StatisticsScene::menuCallback(CCObject* pSender)
{
    int nTag = ((MyMenuItem*)pSender)->getTag();
    if (nTag == MENU_SOUND_TAG) {
        
    }
    else if (nTag == MENU_EFFECT_TAG){
        
    }
    else if (nTag == MENU_HELP_TAG){
        
    }
    else if (nTag == MENU_RETURN_TAG){
        CCDirector::sharedDirector()->replaceScene(MainMenu::scene());
        
    }
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //
    //#endif
}

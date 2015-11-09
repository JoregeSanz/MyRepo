//
//  MainMenu.cpp
//  TingTong
//
//  Created by jyn on 4/10/14.
//
//

#include "MainMenu.h"
#include "GameSelectScene.h"
#include "StatisticsScene.h"


CCScene* MainMenu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenu *layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    setBackground(1);

    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    
    // Set background
    MyMenuItem *playItem = MyMenuItem::create("but1a.png",  this, menu_selector(MainMenu::play));
    playItem->setPosition(ccp(0, -winSize.height*0.05));
    

    MyMenuItem *reviewItem = MyMenuItem::create("but2.png",  this, menu_selector(MainMenu::review));
    reviewItem->setPosition(ccp(0, -winSize.height*0.32));

    m_mainmenu = CCMenu::create(playItem, reviewItem, NULL);
    m_mainmenu->setAnchorPoint(CCPointZero);
    m_mainmenu->setPosition( ccp(winSize.width*0.5, -winSize.height*0.5) );
    this->addChild(m_mainmenu);
    
    MyMenuItem* FBItem = MyMenuItem::create("fbbtn.png", this, menu_selector(MainMenu::connectFB));
    MyMenuItem* MGItem = MyMenuItem::create("mgbtn.png", this, menu_selector(MainMenu::moregames));
    m_extramenu = CCMenu::create(MGItem,FBItem,NULL);
    m_extramenu->setAnchorPoint(ccp(0.5f, 0.5f));
    m_extramenu->alignItemsHorizontallyWithPadding(winSize.width * 0.5);
    m_extramenu->setPosition(ccp(0.0f, -winSize.height*0.7f));
    this->addChild(m_extramenu);

    m_spLearn = CCSprite::create("splash_learn.png");
//    m_spLearn->setPosition(ccp(winSize.width*0.5, -winSize.height*0.75));
    this->addChild(m_spLearn);
    m_spLearn->setScale(g_fResScale);

    m_spWith = CCSprite::create("splash_with.png");
    m_spWith->setPosition(ccp(winSize.width*0.7, -winSize.height*0.6));
    this->addChild(m_spWith);
    m_spWith->setScale(g_fResScale);

    returnItem->setEnabled(false);
    returnItem->setOpacity(100);
    this->appearAnimation();
    

    return true;
}

void MainMenu::appearAnimation()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float t1 = 0.8, t2 = 0.3, t3 = 0.15;
    CCSequence* seq = CCSequence::create(CCDelayTime::create(1),
                                         CCMoveTo::create(t1, ccp(size.width*0.5, size.height*0.7)),
                                         CCMoveTo::create(t2, ccp(size.width*0.5, size.height*0.45)),
                                         CCMoveTo::create(t3, ccp(size.width*0.5, size.height*0.5)),
                       NULL);
    m_mainmenu->runAction(seq);
    CCSequence* seq3 = CCSequence::create(CCDelayTime::create(1),
                                         CCMoveTo::create(t1, ccp(size.width*0.5, size.height*0.3)),
                                         CCMoveTo::create(t2, ccp(size.width*0.5, size.height*0.05)),
                                         CCMoveTo::create(t3, ccp(size.width*0.5, size.height*0.1)),
                                         NULL);
    m_extramenu->runAction(seq3);
    
    CCSequence* seq1 = CCSequence::create(CCDelayTime::create(1),
                                          CCMoveTo::create(t1, ccp(size.width*0.5, size.height*0.95)),
                                         CCMoveTo::create(t2, ccp(size.width*0.5, size.height*0.70)),
                                          CCMoveTo::create(t3, ccp(size.width*0.5, size.height*0.75)),
                                         NULL);
    m_spLearn->runAction(seq1);

    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(1),
                                          CCMoveTo::create(t1, ccp(size.width*0.7, size.height*0.8)),
                                         CCMoveTo::create(t2, ccp(size.width*0.7, size.height*0.55)),
                                          CCMoveTo::create(t3, ccp(size.width*0.7, size.height*0.6)),
                                         NULL);
    m_spWith->runAction(seq2);

}
void MainMenu::disappearAnimation()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float t1 = 0.2, t2 = 0.8;
    CCSequence* seq = CCSequence::create(CCMoveTo::create(t1, ccp(size.width*0.5, size.height*0.7)),
                                         CCMoveTo::create(t2, ccp(size.width*0.5, -size.height*0.5)),
                                         NULL);
    m_mainmenu->runAction(seq);
    
    CCSequence* seq1 = CCSequence::create(CCMoveTo::create(t1, ccp(size.width*0.5, size.height*0.95)),
                                          CCMoveTo::create(t2, ccp(size.width*0.5, -size.height*0.75)),
                                          NULL);
    m_spLearn->runAction(seq1);
    
    CCSequence* seq2 = CCSequence::create(CCMoveTo::create(t1, ccp(size.width*0.7, size.height*0.8)),
                                          CCMoveTo::create(t2, ccp(size.width*0.7, -size.height*0.6)),
                                          NULL);
    m_spWith->runAction(seq2);
    
}

void MainMenu::play()
{
    disappearAnimation();
    scheduleOnce(schedule_selector(MainMenu::gameSelect), 1);
}
void MainMenu::gameSelect()
{
    CCDirector::sharedDirector()->replaceScene(GameSelectScene::scene());
}
void MainMenu::review()
{
    disappearAnimation();
    scheduleOnce(schedule_selector(MainMenu::statistics), 1);
}
void MainMenu::statistics()
{
    CCDirector::sharedDirector()->replaceScene(StatisticsScene::scene());
    
}

void MainMenu::connectFB()
{

}

void MainMenu::moregames()
{

}

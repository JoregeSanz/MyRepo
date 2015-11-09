//
//  GameSelectScene.cpp
//  TingTong
//
//  Created by jyn on 4/10/14.
//
//

#include "GameSelectScene.h"

#include "MainMenu.h"
#include "CarGame.h"
#include "NinjaScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

CCScene* GameSelectScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameSelectScene *layer = GameSelectScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameSelectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Set background
    setBackground(-1);

    
    MyMenuItem *carItem = MyMenuItem::create("car_frame1.png",  this, menu_selector(GameSelectScene::onGameObject));
    carItem->isDefaultAction = false;
    carItem->setScale(0.7);
    carItem->setTag(1);
    
    MyMenuItem *melonItem = MyMenuItem::create("watermelon.png",  this, menu_selector(GameSelectScene::onGameObject));
    melonItem->isDefaultAction = false;
    melonItem->setScale(0.7);
    melonItem->setTag(2);
    
    CCMenu* gamemenu = CCMenu::create(carItem,melonItem, NULL);
    gamemenu->setAnchorPoint(CCPointZero);
    gamemenu->setPosition( ccp(winSize.width*0.5, winSize.height*0.5) );
    gamemenu->alignItemsHorizontally();
    
    this->addChild(gamemenu);

    createBoardMenu();
    
    carselected = false;
    fruitselected = false;
    
    return true;
}
void GameSelectScene::createBoardMenu()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_boardMenu = CCLayer::create();
    m_boardMenu->setScale(g_fResScale);
//    Config::setScale(m_boardMenu);
    m_boardMenu->setAnchorPoint(ccp(0, 0));
    m_boardMenu->setPosition( ccp(winSize.width*0.5, winSize.height*1.5) );
    this->addChild(m_boardMenu, 10, 100);
    
    CCSprite* spBoard = CCSprite::create("board.png");
    m_boardMenu->addChild(spBoard);
    
    CCSize sz = spBoard->getContentSize();
    float bw = sz.width * Config::getxScale();
    float bh = sz.height * Config::getyScale();
    
//    CCSprite* spShine = CCSprite::create("shine.png");
//    m_boardMenu->addChild(spShine);
//    
//    CCSprite* spRays = CCSprite::create("rays.png");
//    m_boardMenu->addChild(spRays);
//    
//    CCLabelTTF* lb = CCLabelTTF::create("In here you will learn all the nouns connected with human body", "LuckiestGuyFixed.ttf", 100, CCSizeMake(bw * 0.9, bh * 0.3), kCCTextAlignmentCenter);
//    m_boardMenu->addChild(lb);
    
    CCSprite* spStar = CCSprite::create("star1.png");
    m_boardMenu->addChild(spStar);

    MyMenuItem *memorize = MyMenuItem::create("level_memorize.png",  this, menu_selector(GameSelectScene::onMemorize));
    CCSize sz1 = memorize->getRealSize();
    
    float dx = 300 * g_fResScale;
    float dy = -250 * g_fResScale;
    memorize->setPosition(ccp(-dx, dy));
    
    MyMenuItem *play = MyMenuItem::create("level_play.png",  this, menu_selector(GameSelectScene::onPlay));
    play->setPosition(ccp(dx, dy));
    
    float dx2 = 550 * g_fResScale;
    float dy2 = 320 * g_fResScale;

    MyMenuItem *close = MyMenuItem::create("level_x3a.png",  this, menu_selector(GameSelectScene::onClose));
    sz1 = close->getRealSize();
    close->setPosition(ccp(dx2, dy2));
    
    
    menu = CCMenu::create(memorize, play, close, NULL);
    menu->setPosition( ccp(winSize.width*0.5, winSize.height*1.5) );
    this->addChild(menu, 11);

}

void GameSelectScene::showBoardMenu()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSequence* seq = CCSequence::create(CCMoveTo::create(0.3, ccp(winSize.width*0.5, winSize.height*0.5)),
                                         CCJumpTo::create(0.3, ccp(winSize.width*0.5, winSize.height*0.5), winSize.height*0.1, 1),
                                         NULL);
    m_boardMenu->runAction(seq);
    seq = CCSequence::create(CCMoveTo::create(0.3, ccp(winSize.width*0.5, winSize.height*0.5)),
                                         CCJumpTo::create(0.3, ccp(winSize.width*0.5, winSize.height*0.5), winSize.height*0.1, 1),
                                         NULL);
    menu->runAction(seq);


}
void GameSelectScene::hideBoardMenu()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_boardMenu->runAction(CCSequence::create(CCMoveTo::create(0.2, ccp(winSize.width*0.5, winSize.height*0.4)),
                                              CCJumpTo::create(0.3, ccp(winSize.width*0.5, winSize.height*1.5), winSize.height*0.1, 1),
                                              NULL));
    menu->runAction(CCSequence::create(CCMoveTo::create(0.2, ccp(winSize.width*0.5, winSize.height*0.4)),
                                              CCJumpTo::create(0.3, ccp(winSize.width*0.5, winSize.height*1.5), winSize.height*0.1, 1),
                                              NULL));

}
void GameSelectScene::goGame()
{
    if(carselected)
        CCDirector::sharedDirector()->replaceScene(CarGame::scene());
    else
        CCDirector::sharedDirector()->replaceScene(NinjaScene::scene());
}
void GameSelectScene::onPlay(CCObject* pSender)
{
    hideBoardMenu();
    scheduleOnce(schedule_selector(GameSelectScene::goGame), 1);
}
void GameSelectScene::onMemorize(CCObject* pSender)
{
    hideBoardMenu();
}
void GameSelectScene::onClose(CCObject* pSender)
{
    hideBoardMenu();
}
void GameSelectScene::onGameObject(CCObject* pSender)
{
    int tag = ((CCMenuItem*)pSender)->getTag();
    if(tag == 1)
        carselected = true;
    else
        fruitselected = true;
    
    showBoardMenu();
}


void GameSelectScene::menuCallback(CCObject* pSender)
{
    int nTag = ((MyMenuItem*)pSender)->getTag();
 
    if (nTag == MENU_SOUND_TAG) {
        if (CCUserDefault::sharedUserDefault()->getBoolForKey(SOUNDONOFF))
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey(SOUNDONOFF, false);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey(SOUNDONOFF, true);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
        }
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

//
//  PandaIntroScene.cpp
//  PacMan
//
//  Created by jyn on 2/1/14.
//
//

#include "PandaIntroScene.h"

#include "SimpleAudioEngine.h"
#include "Config.h"
#include "CastScene.h"

using namespace cocos2d;
using namespace CocosDenshion;
CCScene* PandaIntroScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    PandaIntroScene *layer = PandaIntroScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

PandaIntroScene::~PandaIntroScene()
{
    removeAllChildrenWithCleanup(true);
}

// on "init" you need to initialize your instance
bool PandaIntroScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCShaderCache::sharedShaderCache()->purgeSharedShaderCache();
    CCDirector::sharedDirector()->purgeCachedData();
    	
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // Set scale ratio
    Config::setScaleFactor(winSize);
//    CCSprite* back = CCSprite::create("pandaintro.png");
//    Config::setScale(back);
//    back->setPosition(ccp(winSize.width * 0.5, winSize.height / 2));
//    this->addChild(back, 0);

    man = CCSprite::create("PacPanda.png");
    Config::setScale(man);

    man->setScaleX(man->getScaleX() * 0.6);
    man->setScaleY(man->getScaleY() * 0.6);
    man->setScaleX(man->getScaleY());

    this->addChild(man, zOrderMan);
    
    CCRect rcPac = CCRect(0, 0, man->getContentSize().width, man->getContentSize().height);
    CCArray *manFrames = CCArray::create();
    manFrames->addObject(CCSpriteFrame::create("PacPanda.png", rcPac));
    manFrames->addObject(CCSpriteFrame::create("PacPanda2.png", rcPac));
    
    // Create the animation object
    CCAnimation *walkAnim = CCAnimation::createWithSpriteFrames(manFrames, 0.1f);
    man->runAction(CCRepeatForever::create(CCAnimate::create(walkAnim)));
    
    float y = winSize.height * 0.3;
    float dw = winSize.width / 16;
    m_aryPellet = new CCArray;
    CCSprite* spPellet ;
    spPellet = CCSprite::create("small_pellet2.png");
    CCSize sz = spPellet->getContentSize();

    int n = 20 + 6 + 13 + 6 + 5;//50
    for (int i = 0; i < n; i++) {
        float offw = 0;
        if (i == n - 1) {
            spPellet = CCSprite::create("large_pellet2.png");
            offw = spPellet->getContentSize().width / 2 - sz.width/2;
        }
        else
            spPellet = CCSprite::create("small_pellet2.png");
        Config::setScale(spPellet);
        this->addChild(spPellet, zOrderPel);
        
        
        if (i < 20)
            spPellet->setPosition(ccp(-dw / 2 + i * dw, y - sz.height/2));
        else if(i < 26)
            spPellet->setPosition(ccp(-dw / 2 + 20 * dw, y - sz.height/2 + (i-20) * dw));
        else if(i < 39)
            spPellet->setPosition(ccp(-dw / 2 + 20 * dw - (i - 26) * dw, y - sz.height/2 + 6*dw));
        else if(i < 45)
            spPellet->setPosition(ccp(-dw / 2 + 7 * dw, y - sz.height/2 + 6 * dw + (i - 39) * dw));
        else 
            spPellet->setPosition(ccp(-dw / 2 + 7 * dw + (i - 45) * dw + offw, y - sz.height/2 + 6 * dw + 6 * dw));

        m_aryPellet->addObject(spPellet);
    }
    
    m_nEatIndex = 0;
    
    spPellet = (CCSprite*)m_aryPellet->objectAtIndex(0);
    man->setPosition(ccp(spPellet->getPosition().x, spPellet->getPosition().y + spPellet->getContentSize().height/2));

    schedule(schedule_selector(PandaIntroScene::animateEat), 0.1);
    return true;
}
void PandaIntroScene::animateEat()
{
    CCSprite* spPellet = (CCSprite*)m_aryPellet->objectAtIndex(m_nEatIndex);
    man->runAction(CCMoveTo::create(0.1, ccp(spPellet->getPosition().x, spPellet->getPosition().y + spPellet->getContentSize().height/2)));
    
    if (m_nEatIndex == 20) {
    }
    if (m_nEatIndex == 26) {
        man->setScaleX(-1 * man->getScaleX());
    }
    else if (m_nEatIndex == 39){
        man->setScaleX(-1*man->getScaleX());
        man->setRotation(M_PI / 2 * 180);
//        CCRotateTo *rotate = CCRotateTo::create(0.1, M_PI / 2 * 180);
//        man->runAction(rotate);
    }
    else if (m_nEatIndex == 45){
        CCRotateTo *rotate = CCRotateTo::create(0.1, 0);
        man->runAction(rotate);
    }
    else if (m_nEatIndex == 49){
        CCRotateBy *rotate = CCRotateBy::create(1, 360);
        CCSequence* seq = CCSequence::create(rotate, CCCallFunc::create(this, callfunc_selector(PandaIntroScene::finish)), NULL);
        man->runAction(seq);
        unschedule(schedule_selector(PandaIntroScene::animateEat));
    }
    unsigned int  soundId = SimpleAudioEngine::sharedEngine()->playEffect(SND_SMALLPELLET);
    removeChild(spPellet);
    m_nEatIndex++;

}

void PandaIntroScene::finish()
{
    man->stopAllActions();
    CCDirector::sharedDirector()->replaceScene(CastScene::scene());

}

void PandaIntroScene::draw(void)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    ccDrawSolidRect(CCPoint(0, 0), CCPoint(winSize.width, winSize.height), ccc4f(35/255.f, 31/255.f, 32/255.f, 1.f));
}
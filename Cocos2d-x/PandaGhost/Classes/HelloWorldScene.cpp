#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "TitleScene.h"
#include "Config.h"

using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
    removeAllChildrenWithCleanup(true);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // Set scale ratio
    Config::setScaleFactor(winSize);
    
    // Set background
    m_pSprite = CCSprite::create("splash1.png");
    m_pSprite->setAnchorPoint(CCPointZero);
    m_pSprite->setPosition(CCPointZero);
    Config::setScale(m_pSprite);
    this->addChild(m_pSprite);

    m_nPageIndex = 0;
    
    schedule(schedule_selector(HelloWorld::goTitleScene), 2.0f);
    return true;
}
void HelloWorld::goTitleScene(CCObject* pSender)
{
    
    m_nPageIndex++;
    if (m_nPageIndex == 1) {
        m_pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("splash2.png"));
    }
    else if(m_nPageIndex == 2) {
        m_pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("splash3.png"));
    }
    else if(m_nPageIndex == 3) {
        m_pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("splash4.png"));
    }
    else if(m_nPageIndex == 4) {
        m_pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("splash5.png"));
    }
    else if(m_nPageIndex == 5) {
        unschedule(schedule_selector(HelloWorld::goTitleScene));
        CCDirector::sharedDirector()->replaceScene(TitleScene::scene());
    }


}


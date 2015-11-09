#include "TitleScene.h"
#include "SimpleAudioEngine.h"
#include "Config.h"
#include "GameScene.h"
#include "PandaIntroScene.h"

using namespace cocos2d;
using namespace CocosDenshion;
void addFadeInOutEffect(CCSprite* sprite,
                      
                      const ccColor3B& colour, const CCSize& size)

{
    
    CCPoint pos = ccp(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    CCSprite* glowSprite = CCSprite::create("particle-fire.png");
    glowSprite->setScaleX(size.width);
    glowSprite->setScaleY(size.height);
    glowSprite->setColor(colour);
    glowSprite->setPosition(pos);
    glowSprite->setRotation(sprite->getRotation());
//    glowSprite->setOpacity(0.3);
    _ccBlendFunc f = {GL_ONE, GL_ONE};
    glowSprite->setBlendFunc(f);
    sprite->addChild(glowSprite, -1);
    
    CCSequence* seq = CCSequence::create(CCFadeIn::create(0.75), CCFadeOut::create(0.75), NULL);
    glowSprite->runAction(CCRepeatForever::create(seq));
    
}

TitleScene::~TitleScene()
{
    removeAllChildrenWithCleanup(true);
}
CCScene* TitleScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TitleScene *layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
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
    
    // Set background
    CCSprite *background = CCSprite::create("background.png");
    background->setAnchorPoint(CCPointZero);
    background->setPosition(CCPointZero);
    Config::setScale(background);
    this->addChild(background);

//    CCLabelTTF* pLabel = CCLabelTTF::create("PANDAS, PELLETS & POWER-UPS, THE ADVENTURES OF PAC-PANDA TO LOOK LIKE GLOWING NEON", "Thonburi", 34);
//    pLabel->setPosition( ccp(winSize.width / 2, winSize.height - 120) );
//    this->addChild(pLabel, 1);

    CCSprite* pSprite = CCSprite::create("title_top.png");
    Config::setScale(pSprite);
//    pSprite->setScaleY(pSprite->getScaleY() * 1.4);
//    pSprite->setScaleX(pSprite->getScaleX() * 1.3);
    pSprite->setPosition( ccp(winSize.width/2, winSize.height - pSprite->getContentSize().height/ 2*pSprite->getScaleY()) );
    this->addChild(pSprite, 0);
    
    CCSprite* pTitle = CCSprite::create("game_titlepage_tile.png");
    Config::setScale(pTitle);
    pTitle->setPosition( ccp(winSize.width/2, winSize.height - pTitle->getContentSize().height/ 2*pTitle->getScaleY()*1.1) );
    this->addChild(pTitle, 0);
    addFadeInOutEffect(pTitle, ccc3(0,87/2,119/2), CCSizeMake(3, 2));

    CCSprite* pBottomSprite = CCSprite::create("title_bottom.png");
    Config::setScale(pBottomSprite);
//    pBottomSprite->setScaleY(pBottomSprite->getScaleY() * 1.2);
//    pBottomSprite->setScaleX(pBottomSprite->getScaleX() * 1.1);

    pBottomSprite->setPosition( ccp(winSize.width/2, pBottomSprite->getContentSize().height / 2 * pBottomSprite->getScaleY()) );
    this->addChild(pBottomSprite, 0);

    CCSprite* pPanda = CCSprite::create("PacPanda2.png");
    pPanda->setPosition( ccp(winSize.width/2, winSize.height * 0.42) );
    Config::setScale(pPanda);
    this->addChild(pPanda, 0);

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "game_titlepage_start.png",
                                                          "game_titlepage_start.png",
                                                          this,
                                                          menu_selector(TitleScene::onStartBtn) );
    Config::setScale(pCloseItem);
    pCloseItem->setPosition( ccp(winSize.width / 2, pBottomSprite->getContentSize().height / 4 * pBottomSprite->getScaleY()) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    for (int i = 0; i < 4; i++) {
        char temp[20] = {0};
        sprintf(temp, "cast%d.png", i + 1);
        CCSprite* sp = CCSprite::create(temp);
        Config::setScale(sp);
        sp->setScaleX(sp->getScaleX() * 0.45);
        sp->setScaleY(sp->getScaleY() * 0.45);
        addChild(sp);
        sp->setPosition(ccp(winSize.width / 5 * i + winSize.width / 5, winSize.height * 0.2));

    }
    m_nLetterIndex = 0;
//    schedule(schedule_selector(TitleScene::animateTitle), 0.05);

//dukce.pak
    m_introSprite = NULL;
    m_pageIndex = 0;
//added by dukce.pak
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(SND_LEVEL(5));
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SND_LEVEL(5), true);
    return true;
}
void TitleScene::animateTitle()
{
    const char* title = "PANDAS,PELLETS & POWER-UPS THE AVENTURES OF PAC-PANDA";

    int nLen = strlen(title);
    if (m_nLetterIndex == nLen) {
        unschedule(schedule_selector(TitleScene::animateTitle));
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCSprite* pLetter = NULL;
    
    static float x = 0, y = 0;
    if (title[m_nLetterIndex] == ' ') {
        //x += 20 * Config::getxScale();
    }
    else{
        char str[21];
        sprintf(str, "game_titlepage_%c.png", title[m_nLetterIndex]);
        pLetter = CCSprite::create(str);
        
        float w1 = 40 * Config::getxScale(), w2 = 30 * Config::getxScale();
        if (pLetter) {
            Config::setScale(pLetter);
            this->addChild(pLetter, 0);
            x += pLetter->getContentSize().width * 0.85;
            
            if (m_nLetterIndex > 40) {
                x = 110 * Config::getxScale() + w2 * (m_nLetterIndex - 40);
                y = winSize.height * 0.63;
                
                pLetter->setScaleX(pLetter->getScaleX() * 0.8);
                pLetter->setScaleY(pLetter->getScaleY() * 0.8);
            }
            else if (m_nLetterIndex > 26) {
                x = 100 * Config::getxScale() + w2 * (m_nLetterIndex - 26);
                y = winSize.height * 0.69;
                pLetter->setScaleX(pLetter->getScaleX() * 0.8);
                pLetter->setScaleY(pLetter->getScaleY() * 0.8);

            }
            else if (m_nLetterIndex > 14) {
                x = 80 * Config::getxScale() + w1 * (m_nLetterIndex - 14);
                y = winSize.height * 0.76;
            }
            else{
                x = 50 * Config::getxScale() + w1 * m_nLetterIndex;
                y = winSize.height * 0.85;

            }

        }

    }

    m_nLetterIndex++;

    if (pLetter) {
        pLetter->setPosition( ccp(x, y) );
        
        addGlowEffect(pLetter, ccc3(0,174,239), CCSizeMake(1.30f, 1.30f));
    }

}

void TitleScene::onStartBtn(CCObject* pSender)
{
    //    unschedule(schedule_selector(TitleScene::animateTitle));
    //added by dukce.pak
    SimpleAudioEngine::sharedEngine()->playEffect(SND_COINDROP);
    
    scheduleOnce(schedule_selector(TitleScene::introduction), 1.0f);
}


void TitleScene::introduction(){
    if( m_pageIndex == 0){
        removeAllChildrenWithCleanup(true);
        m_introSprite = CCSprite::create("game_control.png");
        m_introSprite->setAnchorPoint(CCPointZero);
        m_introSprite->setPosition(CCPointZero);
        Config::setScale(m_introSprite);
        addChild(m_introSprite);
        m_pageIndex ++;
        CCDelayTime *delayTime = CCDelayTime::create(3.0f);
        CCCallFunc *functionToCall = CCCallFunc::create(this,callfunc_selector(TitleScene::introduction));
        CCFiniteTimeAction *actionSequence = CCSequence::create(delayTime,functionToCall,NULL);
        this->runAction(actionSequence);
        
    }else if(m_pageIndex == 1){
        m_introSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("game_disclaimer.png"));
        m_pageIndex ++;
        CCDelayTime *delayTime = CCDelayTime::create(3.0f);
        CCCallFunc *functionToCall = CCCallFunc::create(this,callfunc_selector(TitleScene::introduction));
        CCFiniteTimeAction *actionSequence = CCSequence::create(delayTime,functionToCall,NULL);
        this->runAction(actionSequence);
    }else if(m_pageIndex == 2){
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        SimpleAudioEngine::sharedEngine()->unloadEffect(SND_LEVEL(5));
        m_introSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage("starring.png"));
        m_pageIndex ++;
        CCDelayTime *delayTime = CCDelayTime::create(1.0f);
        CCCallFunc *functionToCall = CCCallFunc::create(this,callfunc_selector(TitleScene::introduction));
        CCFiniteTimeAction *actionSequence = CCSequence::create(delayTime,functionToCall,NULL);
        this->runAction(actionSequence);
    }else{
        unschedule(schedule_selector(TitleScene::introduction));
        goGameScene();
    }
}

void TitleScene::goGameScene()
{
//    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
    CCDirector::sharedDirector()->replaceScene(PandaIntroScene::scene());
}

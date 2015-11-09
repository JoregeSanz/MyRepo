//
//  WinScene.cpp
//  PacMan
//
//  Created by jyn on 2/1/14.
//
//

#include "WinScene.h"
#include "SimpleAudioEngine.h"
#include "ScoreScene.h"
#include "GameScene.h"
#include "TitleScene.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"


#define main_activity_name "com/panda/ghost/PandaGhost"

using namespace CocosDenshion;
extern int g_nCurLevel;
extern float g_fMarginTop;
extern float g_gridWidth;
extern float g_gridHeight;
//added by dukce.pak
extern int g_nTotalScore;


CCScene* WinScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    WinScene *layer = WinScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

WinScene::~WinScene()
{
    removeAllChildrenWithCleanup(true);
}

// on "init" you need to initialize your instance
bool WinScene::init()
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
    
 //added by dukce.pak
    
    char levelStr[50] = {0};
    sprintf(levelStr,"level_Completed%d.png", g_nCurLevel);
    
    CCSprite *spLevelComplete = CCSprite::create(levelStr);
    Config::setScale(spLevelComplete);
    spLevelComplete->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(spLevelComplete);
    
    man = CCSprite::create("PacPanda.png");
    Config::setScale(man);
    man->setScaleY(man->getScaleX());
    this->addChild(man, zOrderMan);
    
    CCRect rcPac = CCRect(0, 0, man->getContentSize().width, man->getContentSize().height);
    CCArray *manFrames = CCArray::create();
    manFrames->addObject(CCSpriteFrame::create("PacPanda.png", rcPac));
    manFrames->addObject(CCSpriteFrame::create("PacPanda2.png", rcPac));
    
    // Create the animation object
    CCAnimation *walkAnim = CCAnimation::createWithSpriteFrames(manFrames, 0.3f);
    man->runAction(CCRepeatForever::create(CCAnimate::create(walkAnim)));
    
    man->setPosition(ccp(-rcPac.size.width, winSize.height / 2));
//    man->runAction(CCMoveTo::create(1.0, ccp(winSize.width/2, winSize.height / 2)));
//    man->runAction(CCRotateBy::create(1.0, 360));

    CCSequence* seq = CCSequence::create(CCMoveTo::create(2.0, ccp(winSize.width/2, winSize.height / 2)),
                                         CCRotateBy::create(2.0, 360),
                                         CCCallFunc::create(this, callfunc_selector(WinScene::finishWinAnimation)),
                                         NULL);
    man->runAction(seq);

//    for (int i = 0; i < g_nCurLevel; i++) {
//        m_map[i] = NULL;
//        setLevel(i);
//    }
    return true;
}
void WinScene::finishWinAnimation()
{
    this->removeChild(man);
    if(g_nCurLevel == MAX_LEVEL)
    {
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *spLevelComplete = CCSprite::create("level_Completed.png");
        Config::setScale(spLevelComplete);
        spLevelComplete->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(spLevelComplete);

        float scoreRate = 1.5;
        char score[50] = {0};
        sprintf(score,"%d" ,g_nTotalScore);
        CCSprite * spScore = CCSprite::create("score.png");
        Config::setScale(spScore);
        
        spScore->setAnchorPoint(ccp(0,0.5));
        spScore->setScaleX(spScore->getScaleX() * scoreRate);
        spScore->setScaleY(spScore->getScaleY() * scoreRate);
//??? ??? ?? ???? ?? ??? ??? ??
        float lengthX = spScore->getContentSize().width * spScore->getScaleX() * ( 1 + SCORE_PADDING_RATE);
        
        int i = 0;
        while(score[i])
        {
            char number[20] = {0};
            int num = score[i] - 0x30;
            sprintf(number, "num_%d.png", num);
            CCSprite * ccsNum = CCSprite::create(number);
            Config::setScale(ccsNum);
            ccsNum->setScaleX(ccsNum->getScaleX() * scoreRate);
            ccsNum->setScaleY(ccsNum->getScaleY() * scoreRate);
            lengthX += ccsNum->getContentSize().width * ccsNum->getScaleX();
            i++;
        }
//--
        
        float spScoreX = 0;
        float scoreY = winSize.height - spScore->getContentSize().height * spScore->getScaleY() * 2;
        if(lengthX < winSize.width)
            spScoreX = (winSize.width - lengthX)/2;
        
        spScore->setPosition(ccp(spScoreX , scoreY));
        addChild(spScore);
        
        float ccsNum_xPos  = spScore->getPositionX() + spScore->getContentSize().width * spScore->getScaleX() * ( 1 + SCORE_PADDING_RATE);
        
        i = 0;
        while(score[i])
        {
            char number[20] = {0};
            int num = score[i] - 0x30;
            sprintf(number, "num_%d.png", num);
            CCSprite * ccsNum = CCSprite::create(number);
            Config::setScale(ccsNum);
            ccsNum->setScaleX(ccsNum->getScaleX() * scoreRate);
            ccsNum->setScaleY(ccsNum->getScaleY() * scoreRate);
            ccsNum->setPosition(ccp(ccsNum_xPos + ccsNum->getContentSize().width/2 * ccsNum->getScaleY(), scoreY));
            ccsNum_xPos += ccsNum->getContentSize().width * ccsNum->getScaleX();
            addChild(ccsNum);
            i++;
        }
                
        CCMenuItemImage *pPlayAgain = CCMenuItemImage::create(
                                                              "playagain.png",
                                                              "playagain.png",
                                                              this,
                                                              menu_selector(WinScene::onPlayAgain) );
        Config::setScale(pPlayAgain);
        pPlayAgain->setPosition( ccp(winSize.width / 2, winSize.height * 0.2) );
        
        CCMenuItemImage *pFacebook = CCMenuItemImage::create(
                                                             "facebook.png",
                                                             "facebook.png",
                                                             this,
                                                             menu_selector(WinScene::onFacebook) );
        Config::setScale(pFacebook);
        pFacebook->setScaleX(pFacebook->getScaleX()*0.4);
        pFacebook->setScaleY(pFacebook->getScaleY()*0.4);
        
        pFacebook->setPosition( ccp(winSize.width * 0.38, winSize.height * 0.1) );
        
        CCMenuItemImage *pTwitter = CCMenuItemImage::create(
                                                            "twitter.png",
                                                            "twitter.png",
                                                            this,
                                                            menu_selector(WinScene::onTwitter) );
        Config::setScale(pTwitter);
        pTwitter->setPosition( ccp(winSize.width * 0.62, winSize.height * 0.1) );
        pTwitter->setScaleX(pTwitter->getScaleX()*0.4);
        pTwitter->setScaleY(pTwitter->getScaleY()*0.4);
        
        CCMenu* pMenu = CCMenu::create(pPlayAgain, pFacebook, pTwitter,  NULL);
        pMenu->setPosition( CCPointZero );
        this->addChild(pMenu, 1);

        
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SND_LEVEL(6), true);

//added by dukce.pak
        GameScene::SetLives(Init_LifeNum);
        scheduleOnce(schedule_selector(WinScene::replaceTitleScene), 30.0);
    }
    else{
        removeAllChildrenWithCleanup(true);
        CCDirector::sharedDirector()->replaceScene(GameScene::scene());
    }
}

void WinScene::replaceGameScene()
{
    g_nCurLevel = 0;
    g_nTotalScore = 0;
    removeAllChildrenWithCleanup(true);
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void WinScene::replaceTitleScene()
{
    g_nCurLevel = 0;
    g_nTotalScore = 0;
    removeAllChildrenWithCleanup(true);
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CCDirector::sharedDirector()->replaceScene(TitleScene::scene());
}

void WinScene::onPlayAgain(CCObject* pSender)
{
    //added by dukce.pak
    unschedule(schedule_selector(WinScene::replaceTitleScene));
    GameScene::SetLives(Init_LifeNum);
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playEffect(SND_COINDROP);
    
    scheduleOnce(schedule_selector(WinScene::replaceGameScene), 1);
    
}


void WinScene::onFacebook(CCObject* pSender)
{
   #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, main_activity_name, "onReportMessageToFB", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, g_nTotalScore);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
     
}
void WinScene::onTwitter(CCObject* pSender)
{
   #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, main_activity_name, "onReportMessageToTwitter", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, g_nTotalScore);
		t.env->DeleteLocalRef(t.classID);
	}
#endif

}


/*
void WinScene::setLevel(int nLevel)
{
//    m_nLevel = nLevel;
    //read m_map info from file.
    char lpName[20] = {0};
    sprintf(lpName, "level%d.map", nLevel+1);
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(lpName);
    unsigned long size = 0;
    unsigned char * pBytes = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "rb", &size);
    char* mapTemp = new char[size];
    int nLen = 0;
    m_nRow = 0; m_nCol = 0;
    for (int i = 0; i < size; i++) {
        if (pBytes[i] != 0x0A) {
            mapTemp[nLen] = pBytes[i];
            nLen ++;
        }
        else
            m_nRow ++;
    }
    m_nRow ++;
    m_nCol = size / m_nRow;
    
    char* mapTemp2 = new char[size];
    for (int i = 0; i < m_nRow; i++) {
        for (int j = 0; j < m_nCol; j++) {
            char ch = mapTemp[(m_nRow - 1 - i) * m_nCol + j];
            mapTemp2[i * m_nCol + j] = ch;
            
        }
    }
    delete mapTemp;
    
    if (m_map[nLevel])
        delete m_map[nLevel];
    m_map[nLevel] = mapTemp2;
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    g_gridWidth = winSize.width / m_nCol;
    g_gridHeight = (winSize.height - g_fMarginTop * 2) / m_nRow;
    
}

void WinScene::addWallPath(int level, int x, int y, char s)
{
    if (s != 'e') {
        CCPoint pt;
        //    ccDrawSolidRect(CCPoint(x * g_gridWidth, y * g_gridHeight + g_fMarginTop), CCPoint(x * g_gridWidth + g_gridWidth, y * g_gridHeight - g_gridHeight + g_fMarginTop), ccc4f(0/255.f, 100/255.f, 100/255.f, 1.f));
        
        ccDrawColor4F(g_drawColor[level].r, g_drawColor[level].g, g_drawColor[level].b, 1.f);
//        ccDrawRect(CCPoint(x * g_gridWidth, y * g_gridHeight + g_fMarginTop), CCPoint(x * g_gridWidth + g_gridWidth, y * g_gridHeight - g_gridHeight + g_fMarginTop));
        
        if (s == 'V' || s == 'v') {
            ccDrawLine(ccp(x * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp(x * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
            
            ccDrawLine(ccp((x+1) * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
            
        }
        else if (s == 'H' || s == 'h'){
            ccDrawLine(ccp(x * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, y * g_gridHeight + g_fMarginTop));
            ccDrawLine(ccp(x * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
        }
        else if (s == 'L' || s =='l'){
            ccDrawLine(ccp(x * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp(x * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
            ccDrawLine(ccp(x * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, y * g_gridHeight + g_fMarginTop));
        }
        else if (s == 'R' || s =='r'){
            ccDrawLine(ccp((x+1) * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
            ccDrawLine(ccp(x * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, y * g_gridHeight + g_fMarginTop));
        }
        else if (s == 'G' || s == 'g'){
            ccDrawLine(ccp((x+1) * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
            ccDrawLine(ccp(x * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
        }
        else if (s == 'Y' || s == 'y'){
            ccDrawLine(ccp(x * g_gridWidth, y * g_gridHeight + g_fMarginTop),
                       ccp(x * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
            ccDrawLine(ccp(x * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop),
                       ccp((x+1) * g_gridWidth, (y-1) * g_gridHeight + g_fMarginTop));
        }

    }
}

void WinScene::draw(void)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    ccDrawSolidRect(CCPoint(0, 0), CCPoint(winSize.width, winSize.height), ccc4f(40/255.f, 40/255.f, 40/255.f, 1.f));
    
    for (int j = 0; j < MAX_LEVEL; j++) {
        if (m_map[j] != NULL) {
            
            for (int i=0; i<m_nRow * m_nCol; i++) {
                char s = m_map[j][i];
                GameRoleEnum roleEnum = getRoleEnum(s);
                int x = i%m_nCol;
                int y = i/m_nCol;
                switch (roleEnum) {
                    case GameRoleWallEnum:
                        addWallPath(j, x, y, s);
                        break;
                    default:
                        break;
                }
            }
            
        }

    }
}
 */

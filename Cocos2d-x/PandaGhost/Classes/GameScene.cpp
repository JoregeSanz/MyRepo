//
//  GameScene.cpp
//  PacMan
//
//  Created by jyn on 1/31/14.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
//#include "Config.h"
#include "WinScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"

using namespace CocosDenshion;

#define Accelerometer_Interval 1.0/10.0
#define Update_Interval 1.0/30.0f
#define Eat_GhostTime 6.0
#define Appear_ItemTime 10.0

#define Man_Speed 1.5*300*Update_Interval
#define Ghost_Speed 1.5*280*Update_Interval
#define Item_Speed 1.5*200*Update_Interval

#define OnePea_Score 10
#define BigPea_Score 25
//#define Ghost_Score 125

#define Life_Score 25000

#define TEXT_COLOR ccc3(255,255,255)//(64, 41, 229) ,(76,81,163)

#define PAUSESCR_PADDING_RATE 0.05
#define SCORE_PADDING_RATE 0.2
#define PANDA_LIFE_PADDING_RATE 0.0

int g_nCurLevel = 0;
int g_nTotalScore = 0;
float g_fMarginTop;
float g_fMarginLeft;
float g_gridWidth;
float g_gridHeight;

int direct[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
int bonusScore[MAX_LEVEL] = {250, 500, 750, 1000, 1500, 2000, 2500, 5000};
bool    g_bEatLevelBonus[MAX_LEVEL];

//modified by dukce.pak
int g_nGhostScore[4] = {125,250,500, 1000};
int GameScene::m_nLives;
float GameScene::m_itemXPos;
//-----

GameSprite* GameSprite::create(const char *pszFileName)
{
    GameSprite *pobSprite = new GameSprite();
    pobSprite->bGhostDied = false;
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        
        pobSprite->autorelease();

        
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void GameSprite::setGhostState(int n)
{
    ghostState = n;
    if (ghostState == GhostStateFadeIn) {

        this->runAction(CCSequence::create(CCFadeOut::create(0.3f),
                                           CCFadeIn::create(0.3f),
                                           CCCallFunc::create(this, callfunc_selector(GameSprite::findPath)),
                                           NULL));
    }
    else if(ghostState == GhostStateAngel)
        goHome();

}
void GameSprite::findPath()
{
    bFinedPath = false;
    setGhostState(GhostStateAngel);
}
void GameSprite::setPandaState(int n)
{
    pandaState = n;
    if (pandaState == PandaStateReady) {
        moveEnum = GameMoveNoneEnum;
        schedule(schedule_selector(GameSprite::pandaReady), 2);
    }

}
void GameSprite::pandaReady()
{
    unschedule(schedule_selector(GameSprite::pandaReady));
    setPandaState(PandaStateNormal);
}
void GameSprite::setSpeed(float f)
{
    speed = f;
}
void GameSprite::setEndPoint(CCPoint pt)
{
    endPoint = pt;
}

void GameSprite::moveToNextPoint()
{
    CCPoint center = this->getPosition();
	if (endPoint.x != center.x) {
		if (center.x < endPoint.x) {
			center.x += speed;
			if (center.x > endPoint.x) center.x = endPoint.x;
		}
		else {
			center.x -= speed;
			if (center.x < endPoint.x) center.x = endPoint.x;
		}
	}
	
	if (endPoint.y != center.y) {
		if (center.y < endPoint.y) {
			center.y += speed;
			if (center.y > endPoint.y) center.y = endPoint.y;
		}
		else {
			center.y -= speed;
			if (center.y < endPoint.y) center.y = endPoint.y;
		}
	}
	
	this->moveToPoint(center);

}
void GameSprite::moveToPoint(CCPoint p)
{
    this->setPosition(p);

}
bool GameSprite::canMove()
{
    CCPoint center = this->getPosition();
	if (center.x == endPoint.x && center.y == endPoint.y) {
		return false;
	}
	return true;    
    
}
void GameSprite::findShortestPath(char* mapInfo, int startx, int starty, int endx, int endy, int row, int col)
{
    bFinedPath = true;

    int nSize = row*col;
    vis = new bool[nSize];
    memset(vis, 0, sizeof(bool) * nSize);
    pathx = new int[nSize];
    pathy = new int[nSize];
    qx = new int[nSize];
    qy = new int[nSize];
    step = new int[nSize];
    map = mapInfo;
    stx = startx;
    sty = starty;
    zx = endx;
    zy = endy;
    rows = row;
    cols = col;
    
    
    int head = 0, tail = 0;
	qx[tail] = stx, qy[tail] = sty;
	tail ++;
	
	int i;
	int curx = stx, cury = sty;
	int x, y;
	char passchar[]="X *pm.Z";
	char c;
	step[curx + cury * cols ] = 0;
    vis[curx + cury * cols] = true;
    
	while( head < tail )
	{
		curx = qx[head], cury = qy[head];
        
		if( curx == zx && cury == zy )
			break;
		
		head ++;
		
		for( i = 0; i < 4; i ++)
		{
			x = curx + direct[i][0];
			y = cury + direct[i][1];
            
            //			if (x < 0 || x >= rows || y < 0 || y >= cols)
            //                continue;
            
			if( vis[x + y * cols] )
				continue;
            
			c = map[x + y * cols];
			if( strchr(passchar, c) )				// check
			{
				pathx[x + y * cols] = curx;
				pathy[x + y * cols] = cury;
                step[x + y * cols] = step[curx + cury * cols] + 1;
				
				qx[tail] = x;
				qy[tail] = y;
				vis[x + y * cols] = true;
				tail ++;
			}
		}
	}
    nStep = step[curx+cury*cols]+1;
    rltx = new int[nStep];
    rlty = new int[nStep];
    
    
    nIdx = 0;
	outpath(curx, cury);
    rltx[nIdx] = stx;
    rlty[nIdx] = sty - 2;
    nIdx++;
    
    delete vis;
    delete pathx;
    delete pathy;
    delete qx;
    delete qy;
    delete step;
//    solve();
}
void GameSprite::solve()
{

}
void GameSprite::goHome()
{
    CCArray* aryAction = CCArray::create();
    for (int i = 0; i < nIdx; i++) {
        float x = rltx[i]*g_gridWidth + g_gridWidth/2 + g_fMarginLeft;
        float y = rlty[i]*g_gridHeight + g_gridHeight/2 + g_fMarginTop;
        CCPoint pt = CCPoint(x, y);

        float time = 0.13;//(g_gridWidth / speed)/60;
        if (i == nIdx - 1) {
            time = time*2;
        }
        CCMoveTo* mv =  CCMoveTo::create(time, pt);
        aryAction->addObject(mv);
    }
    
    CCCallFunc* call = CCCallFunc::create(this, callfunc_selector(GameSprite::onRecovery));
    aryAction->addObject(call);
    runAction(CCSequence::create(aryAction));
    
}
void GameSprite::onRecovery()
{
    int x = rltx[nIdx-1];
    int y = rlty[nIdx-1];
    CCPoint centerP = ccp(x * g_gridWidth + g_gridWidth / 2 + g_fMarginLeft, y * g_gridHeight + g_gridHeight / 2 + g_fMarginTop);
    setEndPoint(centerP);
	moveToPoint(centerP);
    
    delete rltx;
    delete rlty;

    setGhostState(GhostStateNormal);
}
void GameSprite::outpath(int curx, int cury)
{
	if( curx == stx && cury == sty)
		return;
    rltx[nIdx] = curx;
    rlty[nIdx] = cury;
    nIdx++;
    
	outpath(pathx[curx + cury* cols], pathy[curx + cury * cols]);
}

GameScene::~GameScene()
{
    removeAllChildrenWithCleanup(true);
}

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
//    setAccelerometerEnabled(true); //If using cocos2d-x ver 2.0.2 or later..
    setTouchEnabled(true);
//    CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
//    swipe->setTarget(this, callfuncO_selector(HelloWorld::didSwipe));
//    swipe->setDirection(kSwipeGestureRecognizerDirectionRight | kSwipeGestureRecognizerDirectionLeft);
//    swipe->setCancelsTouchesInView(true);
//    this->addChild(swipe);

//dukce.pak 2014 - 04 - 10
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCShaderCache::sharedShaderCache()->purgeSharedShaderCache();
    CCDirector::sharedDirector()->purgeCachedData();
//----

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // Set scale ratio
    
    Config::setScaleFactor(winSize);
    
    m_map = NULL;
    m_arrayPea = new CCArray;
    m_arrayGhost = new CCArray;
    m_arrayLife = new CCArray;

    SimpleAudioEngine::sharedEngine()->stopAllEffects();

    removeAllChildrenWithCleanup(true);
    
    m_teBlueGhost = CCTextureCache::sharedTextureCache()->addImage("ghost5.png");
    m_teSkelGhost = CCTextureCache::sharedTextureCache()->addImage("ghost6.png");
    m_teAngelGhost = CCTextureCache::sharedTextureCache()->addImage("ghost7.png");

    m_spLoseSkel = CCSprite::create("skel.png");
    Config::setScale(m_spLoseSkel);
    m_spLoseSkel->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->addChild(m_spLoseSkel, zOrderBigItem);
    m_spLoseSkel->setVisible(false);
    //modified by dukce.pak
    manAteGhostNum = 0;
    
    m_spDyingPanda = CCSprite::create("dyingPanda.png");
    Config::setScale(m_spDyingPanda);
    m_spDyingPanda->setScaleX(m_spDyingPanda->getScaleY());
    m_spDyingPanda->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->addChild(m_spDyingPanda, zOrderBigItem);
    
    CCRect rcPac = CCRect(0, 0, m_spDyingPanda->getContentSize().width, m_spDyingPanda->getContentSize().height);
    CCArray *manFrames = CCArray::create();
    manFrames->addObject(CCSpriteFrame::create("dyingPanda.png", rcPac));
    manFrames->addObject(CCSpriteFrame::create("dyingPanda2.png", rcPac));
    CCAnimation *walkAnim = CCAnimation::createWithSpriteFrames(manFrames, 0.25f);
    m_spDyingPanda->runAction(CCRepeatForever::create(CCAnimate::create(walkAnim)));

    m_spDyingPanda->setVisible(false);


    
    m_bColorMaze = false;
//    m_bItemAppear = false;
//modified by ducke.pak
    itemState = BonusNone;

    setLevel(g_nCurLevel);
//    m_nLife = Init_LifeNum;
    for (int i = 0; i < GameScene::Lives() - 1; i++) {
        CCSprite* sprite = CCSprite::create("pacman2.png");
        Config::setScale(sprite);
        
        sprite->setPosition(ccp(sprite->getContentSize().width * sprite->getScaleX()*0.8* (i) + sprite->getContentSize().width/2 * sprite->getScaleX() + g_fMarginLeft, g_fMarginTop - sprite->getContentSize().height / 2 * sprite->getScaleY() - sprite->getContentSize().height * PANDA_LIFE_PADDING_RATE * sprite->getScaleY()));
        float height = sprite->getContentSize().height;
        height *= sprite->getScaleY();
        this->addChild(sprite, zOrderPel);
        m_arrayLife->addObject(sprite);
    }

    layoutGameSprites();
    
    m_spPause = CCSprite::create("pause.png");
    Config::setScale(m_spPause);
    float fY = 0;
//    if (winSize.height == 1136)
    if (winSize.width == 768 && winSize.height == 1024)
        fY = 0;
    else
        fY = 50*Config::getyScale();
    
    m_spPause->setPosition(ccp(winSize.width * 0.5, m_spPause->getContentSize().height/2 + fY));
    this->addChild(m_spPause, zOrderPel);
    
    loadHighScores();
    updateHighScore();

//modified by dukce.pak
    m_nScore = g_nTotalScore;
//--------

    updateScore();

    m_spReady = CCSprite::create("ready.png");
    m_spReady->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.5));
    Config::setScale(m_spReady);
    addChild(m_spReady, zOrderMan+1);
    m_spReadyNum = CCSprite::create("num3.png");
    m_spReadyNum->setPosition(CCPointMake(m_spReady->getContentSize().width*0.5, - m_spReady->getContentSize().height/2));
    Config::setScale(m_spReadyNum);
    m_spReady->addChild(m_spReadyNum, zOrderMan+1);

    preloadSound();
    
    stopGame();
    getReady();
   
    return true;
}

void GameScene::deleteArray()
{
    delete m_arrayPea;
    delete m_arrayGhost;
    delete m_arrayLife;
    delete m_arrHighScore;
    delete m_arrScore;
    

}
void GameScene::preloadSound()
{
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_LEVEL(m_nLevel + 1));
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_POWER(m_nLevel + 1));
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_BIGCHOMP);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_COINDROP);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_DEATH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_GATE);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_COMPLETE);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_NEON);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SND_SMALLPELLET);
}

void GameScene::getReady()
{
    m_bColorMaze = false;
    manEatGhostTime = 0;
    manMoveEnum = GameMoveNoneEnum;
    m_isPlaying = false;
    man->setPandaState(PandaStateReady);
    man->setEndPoint(man->startPoint);
    man->moveToPoint(man->startPoint);
    man->moveEnum = GameMoveNoneEnum;
    man->setRotation(0);

    for (int i = 0; i < m_arrayGhost->count(); i++) {
        GameSprite* ghost = (GameSprite*)m_arrayGhost->objectAtIndex(i);
        ghost->setTexture(ghost->oriTexture);
        ghost->setEndPoint(ghost->startPoint);
        ghost->moveToPoint(ghost->startPoint);

    }
    m_spReady->setVisible(true);
    m_spReadyNum->setTexture(CCTextureCache::sharedTextureCache()->addImage("num3.png"));

    m_nReadyNum = 3;
    schedule(schedule_selector(GameScene::startGame), 1.0);
    playBGSound(true);
}
void GameScene::playBGSound(bool bPlay)
{
    if(bPlay)
    {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SND_LEVEL(m_nLevel + 1), true);
        m_nowPlayingLevel = true;
    }
    else
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

}
void GameScene::startGame()
{
    m_nReadyNum--;
    if (m_nReadyNum == 0) {
        m_nReadyNum = 3;
        m_isPlaying = true;
        m_spReady->setVisible(false);
        unschedule(schedule_selector(GameScene::startGame));
        schedule(schedule_selector(GameScene::updateInterval), Update_Interval);
    }
    else{
        char temp[20] = {0};
        sprintf(temp, "num%d.png", m_nReadyNum);
        m_spReadyNum->setTexture(CCTextureCache::sharedTextureCache()->addImage(temp));
    }
}
void GameScene::stopGame()
{
    manEatGhostTime = 0;
    manMoveEnum = GameMoveNoneEnum;
    m_isPlaying = false;
	
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    unschedule(schedule_selector(GameScene::updateInterval));
}
void GameScene::updateEatItems()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    GameScene::setItemXPos(winSize.width - g_gridWidth - g_fMarginLeft);
    
    for (int i = 0; i <= m_nLevel; i++) {
        if (g_bEatLevelBonus[i] == true) {
            char temp[20] = {0};
            sprintf(temp, "item%02d.png", i + 1);
            CCSprite* sprite = CCSprite::create(temp);
            Config::setScale(sprite);
            
            float x = GameScene::itemXPos() - sprite->getContentSize().width/2 * sprite->getScaleX();
            GameScene::setItemXPos(x - sprite->getContentSize().width / 2 * sprite->getScaleX());
            
//            float y = g_fMarginTop - g_gridHeight - sprite->getContentSize().height / 2 - sprite->getContentSize().height / 10;
            float y = g_fMarginTop - sprite->getContentSize().height / 2 * sprite->getScaleY() - sprite->getContentSize().height * PANDA_LIFE_PADDING_RATE * sprite->getScaleY();
            if(g_bEatLevelBonus[m_nLevel] == true){
                if(i == m_nLevel){
                    sprite->setPosition(ccp(x, y));
                    this->addChild(sprite, zOrderPel);

                }
            }
            else{
                sprite->setPosition(ccp(x, y));
                this->addChild(sprite, zOrderPel);
            }
        }
    }
}
void GameScene::setLevel(int nLevel)
{
    m_nLevel = nLevel;
    if (nLevel == 0)
    {
        for (int i = 0; i < MAX_LEVEL; i++) {
            g_bEatLevelBonus[i] = false;
        }
        
    }
    updateEatItems();
    
    //read m_map info from file.
    char mapName[20] = {0};
    sprintf(mapName, "level%d.map", nLevel+1);
    
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(mapName);
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
    
    int nExit = 0;
    char* mapTemp2 = new char[size];
    memset(mapTemp2, 0, size);
    for (int i = 0; i < m_nRow; i++) {
        for (int j = 0; j < m_nCol; j++) {
            char ch = mapTemp[(m_nRow - 1 - i) * m_nCol + j];
            mapTemp2[i * m_nCol + j] = ch;
            
            if(ch == 'q'){
//                m_ptExit[nExit] = CCPoint(j, i);
//modified by dukce.pak
                m_outDoor[nExit].xx = j;
                m_outDoor[nExit].yy = i;
                GameMoveEnum enterDirection = GameMoveNoneEnum;
                if(i == 0)
                    enterDirection = GameMoveTopEnum;
                else if( i == m_nRow - 1)
                    enterDirection = GameMoveBottomEnum;
                else if( j == 0)
                    enterDirection = GameMoveRightEnum;
                else if( j == m_nCol - 1)
                    enterDirection = GameMoveLeftEnum;
                m_outDoor[nExit].enterDirection = enterDirection;
                
                nExit++;
            }
            else if(ch == 'Z'){
                m_ptGhostHome = CCPoint(j, i);
            }
        }
    }
    mapTemp2[size] = 0;
    delete mapTemp;
    
    if (m_map)
        delete m_map;
    m_map = mapTemp2;


    delete pBytes;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    if(winSize.width/winSize.height == 768.0/1024.0f)
        g_fMarginLeft = 60 * Config::getxScale();
    else
        g_fMarginLeft = 0;
    g_gridWidth = (winSize.width - g_fMarginLeft * 2)/m_nCol;
    g_fMarginTop = (winSize.height - g_gridWidth * (float)m_nRow) / (float)2.0f;// 100 * Config::getyScale();

    g_gridHeight = g_gridWidth;//(winSize.height - g_fMarginTop * 2) / m_nRow;
    
}


// Modified by dukce.pak 2014.02.11.17.50

void GameScene::drawWallPath(int x, int y, float margins, XTDirection d)
{
    float fx = x * g_gridWidth + g_fMarginLeft;
    float fy = (y+1) * g_gridHeight + g_fMarginTop;
    
    ccColor4F red = ccc4f(1, 0, 0, 1.0);
    if (m_bColorMaze) {
        if (d == LEFT)
            ccDrawSolidRect(ccp(fx - g_gridWidth * 0.27, fy + g_gridHeight * margins), ccp(fx + g_gridWidth * 0.27, fy - g_gridHeight - g_gridHeight * margins), red);
        else if (d == RIGHT)
            ccDrawSolidRect(ccp(fx + g_gridWidth * 0.73, fy + g_gridHeight * margins), ccp(fx + g_gridWidth * 1.27, fy - g_gridHeight - g_gridHeight * margins), red);
        else if (d == UP)
            ccDrawSolidRect(ccp(fx - g_gridWidth * margins, fy + g_gridHeight * 0.27 ), ccp(fx + g_gridWidth + g_gridWidth * margins, fy - g_gridHeight * 0.27), red);
        else if (d == DOWN)
            ccDrawSolidRect(ccp(fx - g_gridWidth * margins, fy - g_gridWidth * 0.73), ccp(fx + g_gridWidth + g_gridWidth * margins, fy - g_gridHeight * 1.27), red);
        else if (d == NONE)
            ccDrawSolidRect(ccp(fx- g_gridWidth * 0.27, fy + g_gridHeight * 0.27), ccp(fx + g_gridWidth * 1.27, fy - g_gridHeight * 1.27), red);
    }
}


void GameScene::addPeaWithPoint(int x, int y, bool bBig)
{
    if (x < 0 || y < 0 || x >= m_nCol || y >= m_nRow) {
        return;
    }

    GameSprite *pea;
    if (!bBig)
        pea = GameSprite::create("small_pellet.png");
    else
        pea = GameSprite::create("large_pellet.png");
    pea->isBig = bBig;
    pea->setPosition(ccp(x * g_gridWidth + g_gridWidth / 2 + g_fMarginLeft, y * g_gridHeight + g_gridHeight / 2 + g_fMarginTop));
    Config::setScale(pea);
    this->addChild(pea, zOrderPel);

    m_arrayPea->addObject(pea);

}
void GameScene::addManWithPoint(int x, int y)
{
    if (x < 0 || y < 0 || x >= m_nCol || y >= m_nRow) {
        return;
    }

    man = GameSprite::create("pacman1.png");
    Config::setScale(man);
    man->setScaleX(man->getScaleY());
    man->setRotation(0);
    this->addChild(man, zOrderMan);
    
    CCRect rcPac = CCRect(0, 0, man->getContentSize().width, man->getContentSize().height);
    CCArray *manFrames = CCArray::create();
    manFrames->addObject(CCSpriteFrame::create("pacman1.png", rcPac));
    manFrames->addObject(CCSpriteFrame::create("pacman2.png", rcPac));
    
    // Create the animation object
    CCAnimation *walkAnim = CCAnimation::createWithSpriteFrames(manFrames, 0.3f);
    
    
    man->runAction(CCRepeatForever::create(CCAnimate::create(walkAnim)));
    
    CCPoint centerP = ccp(x * g_gridWidth + g_gridWidth / 2 + g_fMarginLeft, y * g_gridHeight + g_gridHeight / 2 + g_fMarginTop);
    man->startPoint = centerP;
    man->setEndPoint(centerP);
	man->moveToPoint(centerP);
    man->setSpeed(Man_Speed + 15 * m_nLevel * Update_Interval);
    man->setPandaState(PandaStateReady);

}
void GameScene::addGhostWithPoint(int x, int y)
{
    if (x < 0 || y < 0 || x >= m_nCol || y >= m_nRow) {
        return;
    }

    int nGhost = m_arrayGhost->count();
    char lp[20] = {0};
    sprintf(lp, "ghost%d.png", nGhost + 1);
    GameSprite* ghost = GameSprite::create(lp);
    Config::setScale(ghost);
    this->addChild(ghost, zOrderGhost);
    CCPoint centerP = ccp(x * g_gridWidth + g_gridWidth / 2 + g_fMarginLeft, y * g_gridHeight + g_gridHeight / 2 + g_fMarginTop);
    ghost->index = nGhost;
    ghost->startPoint = centerP;
    ghost->setEndPoint(centerP);
	ghost->moveToPoint(centerP);
    ghost->setSpeed(Ghost_Speed + m_nLevel * 15 * Update_Interval);
    ghost->oriTexture = ghost->getTexture();
    m_arrayGhost->addObject(ghost);
}

void GameScene::addItem()
{
//    int x = m_ptExit[1].x; int y = m_ptExit[1].y;
//modified by dukce.pak
    int door = arc4random() % 2;
    int x = m_outDoor[door].xx;
    int y = m_outDoor[door].yy;
    char lp[20] = {0};
    sprintf(lp, "item%02d.png", m_nLevel + 1);
    m_spBonus = GameSprite::create(lp);
    m_spBonus->moveEnum  = m_outDoor[door].enterDirection;
//-------
    Config::setScale(m_spBonus);
    this->addChild(m_spBonus, zOrderGhost);
    CCPoint centerP = ccp(x * g_gridWidth + g_gridWidth / 2 + g_fMarginLeft, y * g_gridHeight + g_gridHeight / 2 + g_fMarginTop);
    m_spBonus->index = m_nLevel;
    m_spBonus->setEndPoint(centerP);
	m_spBonus->moveToPoint(centerP);
    m_spBonus->setSpeed(Item_Speed /*+ m_nLevel * 30 * Update_Interval*/);
    m_spBonus->oriTexture = m_spBonus->getTexture();
    
    CCPoint gridPoint = getGridPointWithPoint(m_spBonus->endPoint);

    GameRoleEnum roleEnum = getRoleEnumByPoint(gridPoint.x, gridPoint.y);
    if (roleEnum != GameRoleWallEnum){
        CCPoint center = this->getCenterWithPoint(gridPoint.x, gridPoint.y);
        m_spBonus->setEndPoint(center);
        m_spBonus->moveToPoint(center);
    }
}

CCPoint GameScene::getCenterWithPoint(float px, float py)
{
    float x = px*g_gridWidth + g_gridWidth/2 + g_fMarginLeft;
    float y = py*g_gridHeight + g_gridHeight/2 + g_fMarginTop;
    CCPoint pt = CCPoint(x, y);
	return pt;
}


CCPoint GameScene::getGridPointWithPoint(CCPoint p)
{
      float x = (p.x - g_fMarginLeft)/g_gridWidth;
      float y = (p.y - g_fMarginTop)/g_gridHeight;
      CCPoint pt = CCPoint((int)x, (int)y);
      return pt;
}
GameRoleEnum GameScene::getRoleEnumByPoint(float px, float py)
{
	if (px < 0 || px >= m_nCol) {
		return GameRoleWallEnum;
	}
	
	if (py < 0 || py >= m_nRow) {
		return GameRoleWallEnum;
	}
	
	int num = px + (py * m_nCol);
	return getRoleEnum(m_map[num]);
}
bool GameScene::isCrossPointWithSprite(GameSprite* spirit)
{
    CCPoint pt = spirit->endPoint;
    CCPoint gridPoint = getGridPointWithPoint(pt);
	GameRoleEnum roleEnum = getRoleEnumByPoint(gridPoint.x, gridPoint.y);
    if (roleEnum == GameRoleCrossEnum){
        
//        switch (spirit->moveEnum) {
//            case GameMoveTopEnum:
//                pt.y += spirit->speed;
//                break;
//            case GameMoveBottomEnum:
//                pt.y -= spirit->speed;
//                break;
//            case GameMoveLeftEnum:
//                pt.x -= spirit->speed;
//                break;
//            case GameMoveRightEnum:
//                pt.x += spirit->speed;
//                break;
//            default:
//                break;
//        }
//        CCPoint gridPoint2 = getGridPointWithPoint(pt);
//        if (gridPoint.x != gridPoint2.x || gridPoint.y != gridPoint2.y)
            return true;
        
    }
    return false;
}
bool GameScene::isTopPointWithSprite(GameSprite* spirit)
{
    CCPoint pt = spirit->endPoint;
    CCPoint gridPoint = getGridPointWithPoint(pt);
	GameRoleEnum roleEnum = getRoleEnumByPoint(gridPoint.x, gridPoint.y);
    if (roleEnum == GameRoleTopWayEnum || roleEnum == GameRoleHomeEnum)
        return true;
    return false;
}

//added by dukce.pak
void GameScene::panda_visible_true()
{
    if(man)
        man->setVisible(true);
}
//----------

// move the spirit left/right/up/down
bool GameScene::moveSpiritWidthOrientation(GameSprite* spirit, GameMoveEnum move, bool trans)
{
	CCPoint gridPoint = getGridPointWithPoint(spirit->endPoint);
    float fDeltaAngle = 0;

	switch (move) {
		case GameMoveTopEnum:
			gridPoint.y += 1;
            fDeltaAngle = M_PI/2;
			break;
		case GameMoveBottomEnum:
			gridPoint.y -= 1;
            fDeltaAngle = -M_PI/2;
			break;
		case GameMoveLeftEnum:
			gridPoint.x -= 1;
            fDeltaAngle = 1;
			break;
		case GameMoveRightEnum:
			gridPoint.x += 1;
			break;
        default:
            break;
	}
    CCRotateTo *rotate = CCRotateTo::create(0.1, fDeltaAngle * 180);
    
	GameRoleEnum roleEnum = getRoleEnumByPoint(gridPoint.x, gridPoint.y);
    
    if ((roleEnum == GameRoleTopWayEnum || roleEnum == GameRoleHomeEnum) &&
        spirit->moveEnum == GameMoveBottomEnum) {
        return false;
    }
    else if (roleEnum == GameRoleExitEnum) {
//modified by dukce.pak
        if ((gridPoint.x == m_outDoor[0].xx && gridPoint.y == m_outDoor[0].yy) || (gridPoint.x == m_outDoor[1].xx && gridPoint.y == m_outDoor[1].yy)) {

            GameMoveEnum enterDirection = GameMoveNoneEnum;
            if(man == spirit)
                man->setVisible(false);
            if(gridPoint.x == m_outDoor[0].xx && gridPoint.y == m_outDoor[0].yy)
            {
                gridPoint.setPoint(m_outDoor[1].xx, m_outDoor[1].yy);
                enterDirection = m_outDoor[1].enterDirection;
            }
            else if(gridPoint.x == m_outDoor[1].xx && gridPoint.y == m_outDoor[1].yy)
            {
                gridPoint.setPoint(m_outDoor[0].xx, m_outDoor[0].yy);
                enterDirection = m_outDoor[0].enterDirection;
            }
//			GameRoleEnum roleEnum = getRoleEnumByPoint(gridPoint.x, gridPoint.y);
//			if (roleEnum != GameRoleWallEnum){
				CCPoint center = this->getCenterWithPoint(gridPoint.x, gridPoint.y);
				spirit->setEndPoint(center);
				spirit->moveToPoint(center);
//			}
            if(man == spirit)
                manMoveEnum = enterDirection;
//-------
            return true;
		}
    }
	else if (roleEnum != GameRoleWallEnum) {
//    if (roleEnum != GameRoleWallEnum) {

		spirit->moveEnum = move;//manMoveEnum;//move;
		spirit->setEndPoint(this->getCenterWithPoint(gridPoint.x, gridPoint.y));
        if(spirit == man)
        {
            if (trans) {
                float scx = spirit->getScaleX();
                if (fDeltaAngle == 1) {
                    spirit->runAction(CCSequence::create(CCRotateTo::create(0.1, 0),CCCallFunc::create(this,callfunc_selector(GameScene::panda_visible_true)),NULL));
                    if(scx >= 0)
                        spirit->setScaleX(-1 * scx);
                }
                else{
                    if(scx < 0)
                        spirit->setScaleX(-1 * scx);
                    spirit->runAction(CCSequence::create(rotate,CCCallFunc::create(this,callfunc_selector(GameScene::panda_visible_true)),NULL));
                }
            }
        }
        else
        {
            if (trans) {
                float scx = spirit->getScaleX();
                if (fDeltaAngle == 1) {
                    spirit->runAction(CCRotateTo::create(0.1, 0));
                    if(scx >= 0)
                        spirit->setScaleX(-1 * scx);
                }
                else{
                    if(scx < 0)
                        spirit->setScaleX(-1 * scx);
                    spirit->runAction(rotate);
                }
            }
        }
        return true;
	}
	return false;
}

//modifed by dukce. pak
void GameScene::updateHighScore()
{
//    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    char lpHigh[50] = {0};
    sprintf(lpHigh, "%d", m_nHighScores[0]);
    if(!m_spHighScore)
    {
        m_spHighScore = CCSprite::create("highscore.png");
        Config::setScale(m_spHighScore);
        m_spHighScore->setScaleX(m_spHighScore->getScaleX() * 0.7);
        m_spHighScore->setScaleY(m_spHighScore->getScaleY() * 0.7);
        m_spHighScore->setPosition(ccp(m_spHighScore->getContentSize().width/2 * m_spHighScore->getScaleX() + g_fMarginLeft + g_gridWidth * 1.5, m_nRow * g_gridHeight + g_fMarginTop + m_spHighScore->getContentSize().height/2 * m_spHighScore->getScaleY()));
        addChild(m_spHighScore, 5);
    }

    if(!m_arrHighScore)
    {
        m_arrHighScore = new CCArray();
    }
    else
    {
        for (int id = m_arrHighScore->count() - 1; id >= 0; id--) {
            CCSprite* ccsnum = (CCSprite*)m_arrHighScore->objectAtIndex(id);
            removeChild(ccsnum);
            m_arrHighScore->removeObject(ccsnum);
        }
    }
    float ccsNum_xPos  = m_spHighScore->getPositionX() + m_spHighScore->getContentSize().width/2 * m_spHighScore->getScaleX() + m_spHighScore->getContentSize().width * m_spHighScore->getScaleX() * SCORE_PADDING_RATE;
    int i = 0;
    while(lpHigh[i])
    {
        char number[20] = {0};
        int num = lpHigh[i] - 0x30;
        sprintf(number, "num_%d.png", num);
        CCSprite * ccsNum = CCSprite::create(number);
        Config::setScale(ccsNum);
        ccsNum->setScaleX(ccsNum->getScaleX() * 0.7);
        ccsNum->setScaleY(ccsNum->getScaleY() * 0.7);
        ccsNum->setPosition(CCPointMake(ccsNum_xPos + ccsNum->getContentSize().width/2 * ccsNum->getScaleY(), m_nRow * g_gridHeight + g_fMarginTop + ccsNum->getContentSize().height/2 * ccsNum->getScaleY()));
        ccsNum_xPos += ccsNum->getContentSize().width * ccsNum->getScaleX();
        addChild(ccsNum, 5);
        m_arrHighScore->addObject(ccsNum);
        i++;
    }
}
//modifed by dukce. pak

void GameScene::updateScore()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    char score[50] = {0};
    sprintf(score, "%d", m_nScore);
    if(!m_spScore)
    {
        m_spScore = CCSprite::create("score.png");
        Config::setScale(m_spScore);
        m_spScore->setScaleX(m_spScore->getScaleX() * 0.7);
        m_spScore->setScaleY(m_spScore->getScaleY() * 0.7);
        m_spScore->setPosition(CCPointMake(winSize.width*0.65  , m_nRow * g_gridHeight + g_fMarginTop + m_spScore->getContentSize().height/2 * m_spScore->getScaleY()));
        addChild(m_spScore, 5);
    }
    
    if(!m_arrScore)
        m_arrScore = new CCArray();
    else
    {
        for (int id = m_arrScore->count() - 1; id >= 0; id--) {
            CCSprite* ccsnum = (CCSprite*)m_arrScore->objectAtIndex(id);
            removeChild(ccsnum);
            m_arrScore->removeObject(ccsnum);
        }
        
    }
    float ccsNum_xPos  = m_spScore->getPositionX() + m_spScore->getContentSize().width/ 2 * m_spHighScore->getScaleX() + m_spScore->getContentSize().width * m_spScore->getScaleX() * SCORE_PADDING_RATE;
    int i = 0;
    while(score[i])
    {
        char number[20] = {0};
        int num = score[i] - 0x30;
        sprintf(number, "num_%d.png", num);
        CCSprite * ccsNum = CCSprite::create(number);
        Config::setScale(ccsNum);
        ccsNum->setScaleX(ccsNum->getScaleX() * 0.7);
        ccsNum->setScaleY(ccsNum->getScaleY() * 0.7);
        ccsNum->setPosition(CCPointMake(ccsNum_xPos + ccsNum->getContentSize().width/2 * ccsNum->getScaleY(), m_nRow * g_gridHeight + g_fMarginTop + ccsNum->getContentSize().height/2 * ccsNum->getScaleY()));
        ccsNum_xPos += ccsNum->getContentSize().width * ccsNum->getScaleX();
        addChild(ccsNum, 5);
        m_arrScore->addObject(ccsNum);
        i++;
    }
/*
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    char temp[30] = {0};
    sprintf(temp, "Score : %d", m_nScore);
    if (m_pScoreLabel == NULL) {
        m_pScoreLabel = CCLabelTTF::create("Score : 0","Marker Felt.ttf", 37);
        m_pScoreLabel->setFontFillColor(TEXT_COLOR, true);
        float fY = 0;
        //        if (winSize.height == 1136)
        fY = g_gridHeight * 2;
        
        
        m_pScoreLabel->setPosition(CCPointMake(winSize.width*0.75, m_nRow * g_gridHeight + g_fMarginTop + m_pScoreLabel->getContentSize().height/2));
        Config::setScale(m_pScoreLabel);
        addChild(m_pScoreLabel, 5);
    }
    m_pScoreLabel->setString((const char *)temp);
*/
}
// update & refresh
void GameScene::updateInterval()
{
    if (man == NULL || man->pandaState == PandaStateDie || man->pandaState == PandaStateReady)
        return;

	manEatGhostTime -= Update_Interval;
    
    //added by dukce.pak
    if(itemState == BonusAppear)
    {
        itemAppearTime -= Update_Interval;
        if( itemAppearTime <= 0)
        {
            itemState = BonusOut;
            //Item delete
            this->removeChild(m_spBonus, true);
            m_spBonus = NULL;
            
        }
    }
    // ----
	if (manEatGhostTime < 0){
        manEatGhostTime = 0;
        if(!m_nowPlayingLevel)
        {
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SND_LEVEL(m_nLevel + 1), true);
            m_nowPlayingLevel = true;
        }
        m_bColorMaze = false;
    }
    else{
        int nUpateMaze = 60;
        if (((int)manEatGhostTime * 60 / nUpateMaze) % 2 == 0)
            m_bColorMaze = false;
        else
            m_bColorMaze = true;
        
    }
    
   {
       bool bManInCrossPoint = isCrossPointWithSprite(man);
       if (bManInCrossPoint && man->moveEnum != manMoveEnum)
           man->moveEnum = manMoveEnum;
    
       if (man->canMove()) {
           man->moveToNextPoint();
       }
       else {
           if(this->moveSpiritWidthOrientation(man, manMoveEnum, true) == false)
               this->moveSpiritWidthOrientation(man, man->moveEnum, true);
       }
       //    CCRect rectPlayer = man->boundingBox();
       //Pellet
       for (int i = m_arrayPea->count() - 1; i >= 0; i--) {
           GameSprite* pea = (GameSprite*)m_arrayPea->objectAtIndex(i);
           CCRect rcPea = pea->boundingBox();
           if(rcPea.containsPoint(man->getPosition()) == true)
           {
               if(pea->isBig){
                   pauseSchedulerAndActions();
                   if(m_nowPlayingLevel)
                   {
                       SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SND_POWER(m_nLevel + 1), true);
                       m_nowPlayingLevel = false;
                   }
                   manEatGhostTime = Eat_GhostTime;
                   m_bColorMaze = true;
                   addScore(BigPea_Score, pea->getPosition(), false);

                   for (int i = m_arrayGhost->count() - 1; i >= 0; i--) {
                       GameSprite* ghost = (GameSprite*)m_arrayGhost->objectAtIndex(i);
                       ghost->bGhostDied = false;
                   }
                   resumeSchedulerAndActions();

               }
               else{
                   SimpleAudioEngine::sharedEngine()->playEffect(SND_SMALLPELLET);
                   addScore(OnePea_Score, pea->getPosition(), false);
                }
               removeChild(pea);
               m_arrayPea->removeObject(pea);

               if (m_arrayPea->count() == 0) {
                   stopGame();
                   SimpleAudioEngine::sharedEngine()->playEffect(SND_COMPLETE);
                   scheduleOnce(schedule_selector(GameScene::win), 2.0);
               }
            break;
           }
       }
   }
	
    //Item Process
    {
        if (m_spBonus != NULL){
        
            if (isCrossPointWithSprite(m_spBonus)) {
                GameMoveEnum e = m_spBonus->moveEnum;
                while (true) {
                    GameMoveEnum m = (GameMoveEnum)(arc4random()%4 + 1);
                    if ((e == GameMoveTopEnum    && m != GameMoveBottomEnum) ||
                        (e == GameMoveBottomEnum && m != GameMoveTopEnum)   ||
                        (e == GameMoveLeftEnum   && m != GameMoveRightEnum) ||
                        (e == GameMoveRightEnum  && m != GameMoveLeftEnum)  ) {
                    
                        m_spBonus->moveEnum = m;
                        break;
                    }
                }
            }

            if (m_spBonus->canMove()) {
                m_spBonus->moveToNextPoint();
            }
            else {
                GameMoveEnum e = m_spBonus->moveEnum;
                while (true) {
                    if (e != GameMoveNoneEnum) {
                        if (this->moveSpiritWidthOrientation(m_spBonus, e, false)) {
                            break;
                        }
                    }
                    e = (GameMoveEnum)(arc4random()%4 + 1);
                }
            }
            
            CCRect rectPlayer = man->boundingBox();
            if (rectPlayer.containsPoint(m_spBonus->getPosition())) {
                
                addScore(bonusScore[m_nLevel], m_spBonus->getPosition(), true);

                removeChild(m_spBonus, true);
                m_spBonus = NULL;
//                m_bItemAppear = false;
                itemState = BonusOut;

                SimpleAudioEngine::sharedEngine()->playEffect(SND_BIGCHOMP);
                char lp[20] = {0};
                sprintf(lp, "bigItem%02d.png", m_nLevel + 1);
                GameSprite* big = GameSprite::create(lp);
                Config::setScale(big);
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                big->setPosition(ccp(winSize.width / 2, winSize.height/2));
                this->addChild(big, zOrderBigItem);
                CCSequence* seq = CCSequence::create(CCFadeIn::create(0.75), CCFadeOut::create(0.75), NULL);
                big->runAction(seq);

                g_bEatLevelBonus[m_nLevel] = true;
                updateEatItems();
                
            }
        }
    }

    //Ghost Process
    {
        for (int i = m_arrayGhost->count() - 1; i >= 0; i--) {
            GameSprite* ghost = (GameSprite*)m_arrayGhost->objectAtIndex(i);

            if (ghost->ghostState == GhostStateFadeIn || ghost->ghostState == GhostStateAngel) {
            
            }
            else{
                if (manEatGhostTime){
                    if(!ghost->bGhostDied)
                    {
                        if(m_bColorMaze)
                            ghost->setTexture(m_teSkelGhost);
                        else
                            ghost->setTexture( m_teBlueGhost);
                    }
                    else
                        ghost->setTexture(ghost->oriTexture);
                }
                else {
                    ghost->setTexture(ghost->oriTexture);
                    //modified by dukce.pak
                    ghost->bGhostDied = false;
                    manAteGhostNum = 0;
                }
            
                if (isCrossPointWithSprite(ghost)) {
                    GameMoveEnum e = ghost->moveEnum;
                    while (true) {
                        GameMoveEnum m = (GameMoveEnum)(arc4random()%4 + 1);
                        if ((e == GameMoveTopEnum    && m != GameMoveBottomEnum) ||
                            (e == GameMoveBottomEnum && m != GameMoveTopEnum)   ||
                            (e == GameMoveLeftEnum   && m != GameMoveRightEnum) ||
                            (e == GameMoveRightEnum  && m != GameMoveLeftEnum)  ) {
                        
                            ghost->moveEnum = m;
                            break;
                        }
                    }
                }
            
                if (isTopPointWithSprite(ghost)) {
                    ghost->moveEnum = GameMoveTopEnum;
                }
                if (ghost->canMove()) {
                    ghost->moveToNextPoint();
                }
                else {
                    GameMoveEnum e = ghost->moveEnum;
                    while (true) {
                        if (e != GameMoveNoneEnum) {
                            if (this->moveSpiritWidthOrientation(ghost, e, false)) {
                                break;
                            }
                        }
                        e = (GameMoveEnum)(arc4random()%4 + 1);
                    }
                }
            
                CCRect rectPlayer = man->boundingBox();
                if (rectPlayer.containsPoint(ghost->getPosition()) && ghost->ghostState != GhostStateAngel) {
                    if (manEatGhostTime && !ghost->bGhostDied) {
                    //modified by dukce.pak
                        manAteGhostNum ++;
                        ghost->bGhostDied = true;
                        ghost->setTexture(m_teAngelGhost);
                        ghost->setGhostState(GhostStateFadeIn);
                    //come back to ghost's home
                        if (ghost->bFinedPath == false) {
                            CCPoint ptG = getGridPointWithPoint(ghost->endPoint);

                            ghost->findShortestPath(m_map, m_ptGhostHome.x, m_ptGhostHome.y,  (int)ptG.x, (int)ptG.y,   m_nRow, m_nCol);
//                            CCPoint ptS = getGridPointWithPoint(ghost->startPoint);
//                            ghost->findShortestPath(m_map, ptS.x, ptS.y,  (int)ptG.x, (int)ptG.y,   m_nRow, m_nCol);
                        }
                    //modified by dukce.pak
                        int score;
                        int score_id = (manAteGhostNum - 1 ) % 4;
                        score = g_nGhostScore[score_id];
                    
                        SimpleAudioEngine::sharedEngine()->playEffect(SND_GATE);
                        addScore(score, ghost->getPosition(), true);
                    //--
                        if (itemState == BonusNone) {
                            itemState = BonusAppear;
                        //modified by dukce.pak
                            itemAppearTime = Appear_ItemTime;
                            addItem();
                        }
                    }
                    else
                    {
                        if (man->pandaState == PandaStateNormal ) {
                        
                            pandaDied();
                        }

                    }
                    break;
                
                }

            }
        }
    }
}
void GameScene::pandaDied()
{
    man->setPandaState(PandaStateDie);
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playEffect(SND_DEATH);

    m_isPlaying = false;
    
    m_spLoseSkel->setVisible(true);
    m_spDyingPanda->setVisible(true);
    float fInteval = 0.3;
    CCSequence* seq = CCSequence::create(CCRotateBy::create(fInteval, 90),
                                         CCRotateBy::create(fInteval, 0),
                                         CCRotateBy::create(fInteval, 90),
                                         CCRotateBy::create(fInteval, 0),
                                         CCRotateBy::create(fInteval, 90),
                                         CCRotateBy::create(fInteval, 0),
                                         CCRotateBy::create(fInteval, 90),
                                         CCRotateBy::create(fInteval, 0),
                                         CCCallFunc::create(this, callfunc_selector(GameScene::finishLoseAnimation)), NULL);
    m_spDyingPanda->runAction(seq);
    

    

}
void GameScene::addScore(int nScore, CCPoint pt, bool bPopup)
{
    int oldScore = m_nScore;
    m_nScore += nScore;
    if (bPopup) {
        char temp[20] = {0};
        sprintf(temp, "+%d", nScore);
        
        CCLabelTTF* lb = CCLabelTTF::create(temp, "Marker Felt.ttf", 20);
        lb->setPosition(ccp(pt.x, pt.y + g_gridHeight * 3));
        lb->runAction(CCSequence::create(CCFadeIn::create(1), CCFadeOut::create(1), NULL));
        addChild(lb);
    }
    
    
    updateScore();
    
    int nTemp = m_nScore / Life_Score;
    if (nTemp > 0 && oldScore < nTemp * Life_Score && m_nScore >= nTemp * Life_Score) {
        addLife();
    }

}
void GameScene::addLife()
{
    //modified by dukce.pak
    int lives = GameScene::Lives();
    
    CCSprite* sprite = CCSprite::create("pacman2.png");
    Config::setScale(sprite);
    sprite->setPosition(ccp(sprite->getContentSize().width * sprite->getScaleX() *0.8* (lives - 1) + sprite->getContentSize().width/2 * sprite->getScaleX() + g_fMarginLeft  , g_fMarginTop - sprite->getContentSize().height / 2 * sprite->getScaleY() - sprite->getContentSize().height * PANDA_LIFE_PADDING_RATE * sprite->getScaleY()));
    this->addChild(sprite, zOrderPel);
    m_arrayLife->addObject(sprite);

    //modified by dukce.pak
    lives++;
    GameScene::SetLives(lives);
    //---------

}
void GameScene::finishLoseAnimation()
{
    //modified by dukce.pak
    int lives = GameScene::Lives();
    lives --;
    GameScene::SetLives(lives);
    if (lives > 0) {
        
        getReady();

        CCSprite* sp = (CCSprite*)m_arrayLife->objectAtIndex(lives - 1);
        m_arrayLife->removeObject(sp);
        removeChild(sp);
    }
    else{
//        removeChild(man);
//        man = NULL;
        saveHighScores();
        scheduleOnce(schedule_selector(GameScene::gameOver), 2.0);
    }

    m_spLoseSkel->setVisible(false);
    m_spDyingPanda->setVisible(false);
    
    

}
void GameScene::gameOver()
{
    removeAllChildrenWithCleanup(true);
    deleteArray();
    CCDirector::sharedDirector()->replaceScene(GameOverScene::scene());
}
void GameScene::win()
{
    g_nCurLevel++;
    removeAllChildrenWithCleanup(true);
//added by dukce.pak
    saveHighScores();
    deleteArray();

    CCDirector::sharedDirector()->replaceScene(WinScene::scene());
    
}
void GameScene::loadHighScores()
{
    for(int i = 0; i< HIGHSCORE_NUM; i++)
    {
        char temp[20] = {0};
        sprintf(temp, "high%dst",i);
        m_nHighScores[i] = CCUserDefault::sharedUserDefault()->getIntegerForKey(temp,0);
    }

}
void GameScene::saveHighScores()
{
    int n = HIGHSCORE_NUM;
    for (int i = 0; i < n; i++) {
        if (m_nScore > m_nHighScores[i]) {
            
            for (int j = n - 1; j > i; j--) {
                m_nHighScores[j] = m_nHighScores[j - 1];
            }
            m_nHighScores[i] = m_nScore;

            break;
        }
    }

    g_nTotalScore = m_nScore;
    for(int i = 0; i < HIGHSCORE_NUM; i++)
    {
        char temp[20] = {0};
        sprintf(temp, "high%dst",i);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(temp, m_nHighScores[i]);
    }
    
}

void GameScene::layoutGameSprites(void)
{    
    if (m_map != NULL) {
        for (int i = m_arrayPea->count() - 1; i >= 0; i--)
            removeChild((CCSprite*)m_arrayPea->objectAtIndex(i), true);
        
        m_arrayPea->removeAllObjects();
        for (int i = m_arrayGhost->count() - 1; i >= 0; i--)
            removeChild((CCSprite*)m_arrayGhost->objectAtIndex(i), true);
        
        m_arrayGhost->removeAllObjects();

        for (int i=0; i<m_nRow * m_nCol; i++) {
            char s = m_map[i];
            GameRoleEnum roleEnum = getRoleEnum(s);
            int x = i%m_nCol;
            int y = i/m_nCol;
            switch (roleEnum) {
                case GameRoleManEnum:
                    addManWithPoint(x, y);
                    break;
                case GameRoleGhostEnum:
                    addGhostWithPoint(x, y);
                    break;
                case GameRoleSmallPeaEnum:
                    addPeaWithPoint(x, y, false);
                    break;
                case GameRoleCrossEnum:
                    addPeaWithPoint(x, y, false);
                    break;
                case GameRoleBigPeaEnum:
                    addPeaWithPoint(x, y, true);
                    break;
                case GameRoleWallEnum:
                    addPeace(x, y, s);
                default:
                    break;
            }
        }
        
    }
    
}
void GameScene::addPeace(int x, int y, char s)
{
    if (x < 0 || y < 0 || x >= m_nCol || y >= m_nRow) {
        return;
    }
    
    float fx = x * g_gridWidth + g_gridWidth/2 + g_fMarginLeft;
    float fy = y * g_gridHeight + g_fMarginTop + g_gridHeight/2;
    if (s != 'e') {
        CCSprite* sp = NULL;
        char lpCurve[20] = {0};
        char lpPiece[20] = {0};

        sprintf(lpCurve, "curve%d.png", m_nLevel+1);
        sprintf(lpPiece, "piece%d.png", m_nLevel+1);
//modified by dukce.pak
        if (s == 'H' || s == 'h' || s == 'V' || s == 'v' )
        {
            sp = CCSprite::create(lpPiece);
            sp->setScale(g_gridWidth / (sp->getContentSize().width ));
            sp->setScaleX(sp->getScaleX() * 1.53);
        }
        else if (s == 'L'|| s == 'l' || s == 'R' || s == 'r' || s == 'Y' || s == 'y' || s == 'G' || s == 'g')
        {
            sp = CCSprite::create(lpCurve);
            sp->setScale(g_gridWidth / sp->getContentSize().width);
        }
        
        if (sp) {
//            sp->setScale(g_gridWidth / sp->getContentSize().width);
//-------
            if (s=='V' || s == 'v')
                sp->setRotation(90);
            else if(s=='R' || s == 'r')
                sp->setScaleX(-1* sp->getScaleX());
            else if(s=='Y' || s == 'y')
                sp->setScaleY(-1* sp->getScaleY());
            else if(s=='G' || s == 'g'){
                sp->setScaleX(-1* sp->getScaleX());
                sp->setScaleY(-1* sp->getScaleY());
            }
            addChild(sp);
            sp->setPosition(ccp(fx, fy));
        }
    }


}
void GameScene::draw(void)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    ccDrawSolidRect(CCPoint(0, 0), CCPoint(winSize.width, winSize.height), ccc4f(35/255.f, 31/255.f, 32/255.f, 1.f));

//    if (winSize.height == 1136)
    {
        float fw =3;//g_gridHeight / 4.0;
        glLineWidth(fw);
        
        for (int  i = 0; i < 8; i++) {
            if(i <= m_nLevel)
                ccDrawColor4F(g_drawLevelLineColor[i].r, g_drawLevelLineColor[i].g, g_drawLevelLineColor[i].b, 1.f);
            else
                ccDrawColor4F(g_borderColor.r, g_borderColor.g, g_borderColor.b, 1.f);

            ccDrawLine(ccp(0, fw * ((8 - i)*2)), ccp(winSize.width, fw * ((8 - i)*2)));
            
            ccDrawLine(ccp(0, winSize.height - fw * ((8 - i)*2)), ccp(winSize.width, winSize.height - fw * ((8 - i)*2)));
            
        }
    }

    
    if (m_map != NULL) {

        for (int i=0; i<m_nRow * m_nCol; i++) {
            
            if (m_map[i] == 0) {
                break;
            }
            char s = m_map[i];
            GameRoleEnum roleEnum = getRoleEnum(s);
            int x = i%m_nCol;
            int y = i/m_nCol;
  //modified by dukce.pak
            if (roleEnum == GameRoleWallEnum && s != 'e') {
                    float inner_margin = 0.73;
                    float outter_margin = 0.27;
                
                    if (x - 1  < 0 || y - 1 < 0 || x + 1 >= m_nCol || y + 1 >= m_nRow) {
                        continue;
                    }

                    char sl = m_map[y * m_nCol + x - 1];
                    char sr = m_map[y * m_nCol + x + 1];
                    char st = m_map[(y+1) * m_nCol + x];
                    char sb = m_map[(y-1) * m_nCol + x];
                
                    if( s == 'V')
                    {
                        if(sl == '_' || sl == 'Y' || sl == 'G' || sl == 'L' || sl == 'R'|| sl == 'V')
                        {
                            if(st == 'L' || sb == 'Y')
                                drawWallPath(x, y, inner_margin, LEFT);
                            else
                                drawWallPath(x, y, outter_margin, LEFT);
                        }
                        else if(sr == '_' || sr == 'Y' || sr == 'G' || sr == 'L' || sr == 'R'|| sr == 'V')
                        {
                            if(st == 'R' || sb == 'G')
                                drawWallPath(x, y, inner_margin, RIGHT);
                            else
                                drawWallPath(x, y, outter_margin, RIGHT);
                        }
                    }
                    else if(s == 'H')
                    {
                        if(st == '_' || st == 'Y' || st == 'G' || st == 'L' || st == 'R'|| st == 'H')
                        {
                            if(sl == 'L' || sr == 'R')
                                drawWallPath(x, y, inner_margin, UP);
                            else
                                drawWallPath(x, y, outter_margin, UP);
                        }
                        else if(sb == '_' || sb == 'Y' || sb == 'G' || sb == 'L' || sb == 'R' || sb == 'H')
                        {
                            if(sl == 'Y' || sr == 'G')
                                drawWallPath(x, y, inner_margin, DOWN);
                            else
                                drawWallPath(x, y, outter_margin, DOWN);
                        }
                    }
                    else if( s == '_')
                        drawWallPath(x, y, outter_margin, NONE);
  // ---------
            }
        }
    }

//    if (m_isPlaying)
//        updateInterval();
    
}

void GameScene::onResume()
{
    removeChild(m_spPauseScreen, true);
    m_isPlaying = true;
    preloadSound();
    schedule(schedule_selector(GameScene::updateInterval), Update_Interval);

}
void GameScene::onRestart()
{
    g_nCurLevel = 0;
    g_nTotalScore = 0;
    removeAllChildrenWithCleanup(true);
    //modified by dukce.pak
    GameScene::SetLives(Init_LifeNum);
    deleteArray();

    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
//added by dukce.pak
void GameScene::onQuit()
{
//    SimpleAudioEngine::sharedEngine()->playEffect(SND_COINDROP);
    removeAllChildrenWithCleanup(true);
    deleteArray();

    CCDirector::sharedDirector()->replaceScene(TitleScene::scene());
}

// Override of touches
void GameScene::ccTouchesBegan(cocos2d::CCSet* _touches, cocos2d::CCEvent* event)
{
    m_bDrag = false;
    CCTouch* touch;
    touch = (CCTouch*)_touches->anyObject();
    
    m_ptPrev = touch->getLocationInView();
    
}

void GameScene::ccTouchesMoved(cocos2d::CCSet* _touches, cocos2d::CCEvent* event)
{
    m_bDrag = true;
    
}

void GameScene::ccTouchesEnded(cocos2d::CCSet* _touches, cocos2d::CCEvent* event)
{
    CCTouch* touch;
    touch = (CCTouch*)_touches->anyObject();
    
    CCPoint pt = touch->getLocationInView();
    if (m_isPlaying) {
        if (m_bDrag) {
            
            float dx = fabsf(pt.x - m_ptPrev.x);
            float dy = fabsf(pt.y - m_ptPrev.y);
            if (dx > dy) {
                if (pt.x > m_ptPrev.x)
                    manMoveEnum = GameMoveRightEnum;
                else
                    manMoveEnum = GameMoveLeftEnum;
            }
            else{
                if (pt.y < m_ptPrev.y)
                    manMoveEnum = GameMoveTopEnum;
                else
                    manMoveEnum = GameMoveBottomEnum;
                
            }
        }
        else{
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            
                CCRect rcPause = m_spPause->boundingBox();
            pt.y = winSize.height - pt.y;
                if (rcPause.containsPoint(pt)) {
                    m_isPlaying = false;
                    m_spPauseScreen = CCSprite::create("background.png");
                    Config::setScale(m_spPauseScreen);
                    m_spPauseScreen->setPosition(ccp(winSize.width/2, winSize.height/2));
                    m_spPauseScreen->setOpacity(200);
            
                    addChild(m_spPauseScreen, zOrderMan+100);
                    
//added by dukce.pak
            
                    CCMenuItemImage *restartItem = CCMenuItemImage::create("restart.png", "restart.png", "restart.png",this, menu_selector(GameScene::onRestart));
//                    Config::setScale(restartItem);

                    
                    CCMenuItemImage *resumeItem = CCMenuItemImage::create("resume.png", "resume.png", "resume.png",this, menu_selector(GameScene::onResume));
//                    Config::setScale(resumeItem);
                    
                    CCPoint pos(m_spPauseScreen->getPosition());
                    
                    CCMenuItemImage *quitItem = CCMenuItemImage::create("quit.png", "quit.png", "quit.png",this, menu_selector(GameScene::onQuit));
//                    Config::setScale(quitItem);
                    
                    CCMenu *pauseMenu = CCMenu::create(resumeItem,restartItem, quitItem, NULL);
                    pauseMenu->alignItemsVerticallyWithPadding(winSize.height * PAUSESCR_PADDING_RATE);
                    pauseMenu->setPosition(ccp(m_spPauseScreen->getContentSize().width/2 ,m_spPauseScreen->getContentSize().height/2));
                    m_spPauseScreen->addChild(pauseMenu);
                    unschedule(schedule_selector(GameScene::updateInterval));
                }

        }

    }
    
}


void GameScene::didAccelerate(cocos2d::CCAcceleration* pAccelerationValue)
{
    //    pAccelerationValue>x; //horizontal acceleration
    //    pAccelerationValue>y; //vertical acceleration
    //    //If you want to move sprite then can use it to change the position..
    //    sprite>setPosition.x+pAccelerationValue>x*3,sprite->getPosition().y)); //I have not tested it but to move sprite horizontally with some speed like x*6..you can change it as accordingly..
//    if (m_isPlaying) {
//		if (fabsf(pAccelerationValue->x) > fabsf(pAccelerationValue->y))
//			manMoveEnum = (pAccelerationValue->x > 0)?GameMoveRightEnum:GameMoveLeftEnum;
//		else
//			manMoveEnum = (pAccelerationValue->y > 0)?GameMoveTopEnum:GameMoveBottomEnum;
//	}
    
}

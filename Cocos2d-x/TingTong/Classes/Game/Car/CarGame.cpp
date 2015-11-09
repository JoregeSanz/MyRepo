//
//  CarGame.cpp
//  TingTong
//
//  Created by jyn on 4/12/14.
//
//

#include "CarGame.h"
#include "GameSelectScene.h"
#include <stdio.h>
#include "ClipNode.h"
#include "StatisticsScene.h"
#include "Obstacle.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace cocos2d::extension;
using namespace std;

#define ROAD_MAX    4

#define SCREEN_H    640
#define SCREEN_W    1136

#define ani_car_kind    9

const float roadline_posx_skewx[4][2] = {{-170, -40}, {-40, -15}, {30, 10}, {200, 39}};
const char *ani_car_name[ani_car_kind] = {"idle", "idleleftturn", "idlerightturn", "handstand", "handwave", "runrightcar", "standuponcar", "thumbsup", "thumbsupboth"};
//run crushing animation

//initialize scene
CCScene* CarGame::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CarGame *layer = CarGame::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance

//init layer
bool CarGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }
    setTouchEnabled(true);

    CCDirector::sharedDirector()->setProjection(kCCDirectorProjection3D);
    levelinfo = new CCSVParse;
    levelinfo->openFile("car1.csv");

    m_levelInfo.Levelnum = CCUserDefault::sharedUserDefault()->getIntegerForKey(LEVEL);
    m_levelInfo.Time = atoi(levelinfo->getData(0, 1).c_str());
    m_levelInfo.Lifes = atoi(levelinfo->getData(3, 1).c_str());
    m_levelInfo.Speed = atoi(levelinfo->getData(1, 1).c_str());
    m_levelInfo.Ticks = atoi(levelinfo->getData(7, 1).c_str());
    m_levelInfo.Points1 =atoi(levelinfo->getData(4, 1).c_str());
    m_levelInfo.Points2 =atoi(levelinfo->getData(5, 1).c_str());
    m_levelInfo.Points3 =atoi(levelinfo->getData(6, 1).c_str());

    m_nRoadLine = 2;
    m_nTicknum = 10;
    obstacleinterval=0.01f / m_levelInfo.Speed;
    
    m_car = CCSkeletonAnimation::createWithFile("spine/skeleton.json", "spine/skeleton.atlas");
//    m_car->setMix(ani_car_name[1], ani_car_name[0], 0.4f);
//    m_car->setMix(ani_car_name[2], ani_car_name[0], 0.4f);
    m_car->setTag(1);
    m_car->setAnimation(ani_car_name[0], true);
    m_car->setScale(g_fResScale);
	m_car->timeScale = 1.0f;
    m_car->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(1, CCPoint(15 * g_fResScale, 0)), CCMoveBy::create(1, CCPoint(-15 * g_fResScale, 0)), NULL)));
	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    float fRoadColW = 100 * g_fResScale;
	m_car->setPosition(ccp(windowSize.width / 2 - fRoadColW, 20*g_fResScale));
	this->addChild(m_car,5);

    CCSprite* carshadow = CCSprite::create("carshadow.png");
    carshadow->setScale(g_fResScale);
    carshadow->setPosition(ccp(windowSize.width/2.0f + 20.0f * g_fResScale, 100.0f*g_fResScale));
    carshadow->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(1, CCPoint(15 * g_fResScale, 0)), CCMoveBy::create(1, CCPoint(-15 * g_fResScale, 0)), NULL)));
    this->addChild(carshadow,4);
    
    char m_lev[10];
    sprintf(m_lev, "Level:%d", m_levelInfo.Levelnum);
    lbl_levelNum = CCLabelTTF::create(m_lev, "LuckiestGuyFixed", 80 * g_fResScale);
    lbl_levelNum->setAnchorPoint(ccp(0.5f,0.0f));
    lbl_levelNum->setPosition(ccp(winSize.width*0.1f, winSize.height*0.8f));
    lbl_levelNum->setColor(ccBLUE);
    this->addChild(lbl_levelNum);
    

    
    for (int i = 1; i <= m_levelInfo.Lifes; i++)
    {
        CCSprite* heart = CCSprite::create("heart.png");
        heart->setTag(i+HEART_TAG);
        heart->setPosition(ccp(winSize.width*0.6f + i*winSize.width/16, winSize.height*0.8f));
        heart->setAnchorPoint(ccp(0.0f, 0.0f));
        heart->setScale(g_fResScale);
        this->addChild(heart);
    }
    
    char m_Score[20];
    //m_Score = 0;
    sprintf(m_Score, "Score:0");
    lbl_Score = CCLabelTTF::create(m_Score, "LuckiestGuyFixed", 80 * g_fResScale);
    lbl_Score->setAnchorPoint(ccp(0.5f,0.0f));
    lbl_Score->setPosition(ccp(winSize.width*0.3f, winSize.height*0.8f));
    lbl_Score->setColor(ccYELLOW);
    this->addChild(lbl_Score);

    lbl_Time = CCLabelTTF::create("time:", "LuckiestGuyFixed", 80 * g_fResScale);
    lbl_Time->setAnchorPoint(ccp(0.5f,0.0f));
    lbl_Time->setPosition(ccp(winSize.width*0.5f, winSize.height*0.8f));
    lbl_Time->setColor(ccGREEN);
    this->addChild(lbl_Time);

    time_Temp = 1.0f;
    number_Level = 1;
    str_Line = 0;

    lbl_Target = CCLabelTTF::create("", "BlackChancery-Regular.ttf", 120 * g_fResScale);
    lbl_Target->setPosition(ccp(winSize.width*0.5f, winSize.height*0.7f));
    lbl_Target->setColor(ccBLACK);
    this->addChild(lbl_Target);
    
    readWordInfo();
    
    setBackground(18);
    
    createRoadLayer();
    
    startstream = false;
    setRoadLine(m_nRoadLine);
    
    return true;
}


//create the roadLayer
void CarGame::createRoadLayer()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    m_roadLayer = CCLayer::create();
    m_roadLayer->setAnchorPoint(ccp(0.5f, 0.5f));
    m_bgLayer->addChild(m_roadLayer);
    m_aryRoadTexture = new CCArray();
    m_aryRoadRight = new CCArray();
    m_aryObstacle = new CCArray();
    m_aryStreamLoad = new CCArray();
    
    CCSize sz;
    int nTex = ROAD_TEX_NUM;

    float y = -winSize.height/2;
    float f = 1322.0f/1500.0f;

    for (int i = 0; i < 17; i++) {

        road = CCSprite::create("new road fr0.png");

        float ff = powf(f, i);
        float fmod = 0.0f;
//        if (i > 4)
            fmod = i * i / 5000.0f;
        float sx = 1.3f * g_fResScale * ff;
        float sy = 0.6f * g_fResScale * ff - fmod;
        road->setScaleY(sy);
        road->setScaleX(sx);

        road->setAnchorPoint(ccp(0.5f, 0.0f));
        sz = road->getContentSize();
        sz.width *= sx;
        sz.height *= sy;
        
        m_fRoadScale[i][0] = sx;
        m_fRoadScale[i][1] = sy;
        m_fRoadY[i] = y;
//        CCLOG("%i,  %f,     %f,     %f", i, y, sx, sy);

        m_aryRoadTexture->addObject(road);
        road->setPosition(ccp(0, y));
        y += sz.height;
        m_roadLayer->addChild(road, nTex - i);
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 0; j < 12; j++) {
            char temp[20] = {0};
            sprintf(temp, "new road fr%d.png", j );
            animation->addSpriteFrameWithFileName(temp);

        }
        animation->setDelayPerUnit(1.0f / 60.0f);
        animation->setRestoreOriginalFrame(true); // Return to the 1st frame after the 14th frame is played.

        CCAnimate *action = CCAnimate::create(animation);

        road->runAction(CCRepeatForever::create(action));
    }
    


}


//set the position of obstacles and their labels
void CarGame::roadStream()
{
    if(iscrashing)
        return;
    
    if (strcmp("x", levelinfo->getData(m_nTicknum, 4).c_str()) == 0)
    {
        CCLog(levelinfo->getData(m_nTicknum, 4).c_str(),NULL);
        CCLOG("firstcol-%d",atoi(levelinfo->getData(m_nTicknum, 0).c_str()));
        updatelbltarget();
    }
    for (int i = 0; i < 4; i++)
    {
        if (!(strcmp("", levelinfo->getData(m_nTicknum, i).c_str()) == 0))
        {
            Obstacle* sp = Obstacle::create(atoi(levelinfo->getData(m_nTicknum, i).c_str()), i, m_nRoadLine,this->wordinfo,lbl_Target->getTag());
            sp->m_gamescene = this;
            m_aryObstacle->addObject(sp);
            CCSprite* firstroad = (CCSprite*)m_aryRoadTexture->objectAtIndex(16);
            float y = m_fRoadY[16] + firstroad->getContentSize().height * firstroad->getScaleY() * g_fResScale;
            float x = firstroad->getRotationX();
            CCSize size = m_roadLayer->getContentSize();
            if (sp->m_nRoadIndex == 0) {
                sp->setPosition(ccp(x - 72 * g_fResScale , y));
                
            }
            
            if (sp->m_nRoadIndex == 1) {
                sp->setPosition(ccp(x - 24 * g_fResScale , y));
                
            }
            
            if (sp->m_nRoadIndex == 2) {
                sp->setPosition(ccp(x + 24 * g_fResScale , y));
                
            }
            
            if (sp->m_nRoadIndex == 3) {
                sp->setPosition(ccp(x + 72 * g_fResScale , y));
                
            }
            
            sp->setScale(0.0f);
            sp->time = 0.1f;
            sp->schedule(schedule_selector(Obstacle::update), obstacleinterval);
            CCLOG("sp-skex%f",sp->getSkewX());
            if (sp->m_nType == 6)
            {
                sp->setSkewX(sp->getSkewX() + 10.0f * (m_nRoadLine-sp->m_nRoadIndex));
            }
            m_roadLayer->addChild(sp, 1);
        }

    }
    

    m_nTicknum ++;
}


//set the road line of the car
void CarGame::setRoadLine(int n)
{
    
    m_roadLayer->stopActionByTag(1);

    float dt = 0.3f;
    m_nRoadLine = n;
    float sx = 1;//winSize.width / 1136.0f;
    CCPoint pt = ccp(roadline_posx_skewx[n][0] * sx, 0);
    CCMoveTo* mov = CCMoveTo::create(dt, pt);
    
    mov->setTag(1);
    m_roadLayer->runAction(mov);
    m_roadLayer->runAction(CCSkewTo::create(dt, roadline_posx_skewx[n][1], 0));
    
    if (startstream)
    {
    for (int i =0 ; i<m_aryObstacle->count(); i++)
    {
        Obstacle* sp = (Obstacle*)m_aryObstacle->objectAtIndex(i);
        
        if (sp->m_nType == 6)
        {
            sp->runAction(CCSkewTo::create(dt, 10.0f * (m_nRoadLine-sp->m_nRoadIndex)-roadline_posx_skewx[n][1] , 0));
        }
        else
            sp->runAction(CCSkewTo::create(dt, -roadline_posx_skewx[n][1], 0));
        

    }
    }
    startstream = true;
}


//update
void CarGame::update(float delta)
{
    time_Temp += delta;    

        m_levelInfo.Time -=delta;
        char time_label[10];
        sprintf(time_label, "time:%d", (int)m_levelInfo.Time);
        lbl_Time->setString(time_label);
        if ((int)m_levelInfo.Time == 0)
            goGameOver();
        

    
    BaseLayer::update(delta);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    for (int i = 0; i < m_aryObstacle->count(); i++)
    {
        
        Obstacle* sp = (Obstacle*)m_aryObstacle->objectAtIndex(i);

        CCPoint point = sp->getPosition();
        
        if (sp->m_nRoadIndex == m_nRoadLine && m_car->getTag() != 2)
        {
            if(sp->getPositionY() + winSize.height / 2< winSize.height / 4  &&   sp->getPositionY() + winSize.height / 2 > winSize.height / 10)
            {
                
                m_aryObstacle->removeObjectAtIndex(i);
                if (sp->m_nType >= 6)
                {
                    if (sp->m_nType == 6)
                    {
                        m_car->setTag(2);
                        m_car->runAction(CCEaseOut::create(CCMoveBy::create(0.3f, ccp(0,100)),5));
                        scheduleOnce(schedule_selector(CarGame::landcar), 2);
                    }
                    else if(sp->m_nType == 7)
                    {
                        m_levelInfo.Score += 50;
                        char num_Score[10];
                        sprintf(num_Score, "Score:%d", m_levelInfo.Score);
                        lbl_Score->setString(num_Score);
                    }
                    else
                    {
                            obstacleinterval = obstacleinterval / 2.0f;
                            for (int i = 0; i < m_aryObstacle->count(); i++)
                            {
                                Obstacle* sp = (Obstacle*)m_aryObstacle->objectAtIndex(i);
                                sp->unscheduleAllSelectors();
                                sp->schedule(schedule_selector(Obstacle::update), obstacleinterval);
                            }
                            scheduleOnce(schedule_selector(CarGame::stoppowerspeed), 2.0f);
                    }

                    sp->removeObstacle();

                }
                
                
                else/*crash with labeled obstacle*/
                {

                    
                if(sp->getTag() == lbl_Target->getTag())
                {
                    sp->AnimateTrue();
                    
                    CCSequence* tint = CCSequence::create(CCTintTo::create(0.2f, 0.0f, 0.0f, 170.0f),CCTintTo::create(0.2f, 255.0f, 255.0f,255.0f),NULL);
                    m_car->runAction(tint);
                    
//                    updatelbltarget();
                    m_levelInfo.Score += 50;
                    char num_Score[10];
                    sprintf(num_Score, "Score:%d", m_levelInfo.Score);
                    lbl_Score->setString(num_Score);
                    
                    if (m_levelInfo.Score == m_levelInfo.Points1)
                    {
                        m_levelInfo.Levelnum += 1;
                        CCUserDefault::sharedUserDefault()->setIntegerForKey(LEVEL, m_levelInfo.Levelnum);
                        CCDirector::sharedDirector()->replaceScene(CarGame::scene());
                    }
                }
                
                //play crashing Animation
                else
                {
                    sp->AnimateFalse();
                    this->stopRunning();
                    
                    CCSequence* tint = CCSequence::create(CCTintTo::create(0.2f, 170.0f, 0.0f, 0.0f),CCTintTo::create(0.2f, 255.0f, 255.0f,255.0f),NULL);
                    m_car->runAction(tint);
                    m_levelInfo.Lifes--;
                    CCSprite* heart = (CCSprite*)this->getChildByTag(HEART_TAG+m_levelInfo.Lifes+1);
                    heart->removeFromParent();
                    
                    if (m_levelInfo.Lifes == 0)
                        goGameOver();
                }
                }
                
                
                continue;
            }
        }

        if (sp->getPositionY() <= -winSize.height/2)
        {
            sp->removeFromParentAndCleanup(true);
            m_aryObstacle->removeObjectAtIndex(i);
            
            continue;
        }
    
    }
    
    if (time_Temp > (60.0f/m_levelInfo.Ticks))
    {
        roadStream();
        time_Temp = 0;
    }
    
    if (m_car)
    {
        if (m_car->states[0]->loop)
        {
            if (m_car->states[0]->time > 3)
            {
                int nAniIdx = arc4random() % 6 + 3;
                m_car->setAnimation(ani_car_name[nAniIdx], false);
            }
        }
        else
        {
            AnimationState* state = m_car->states[0];
            if (AnimationState_isComplete(state))
            {
                m_car->setAnimation(ani_car_name[0], true);
            }
        }
    }
    
}


void CarGame::readWordInfo()
{
    wordinfo = new CCArray;
    wordinfo->initWithCapacity(30);
    CCSVParse* csvFile = new CCSVParse();
    csvFile->openFile("russian.csv");

    
    int num_Rows = csvFile->getRows();
    for (int i=0; i<num_Rows; i++)
    {
        CCString* s_Num = new CCString();
        CCString* s_Eng = new CCString();
        CCString* s_Tran = new CCString();

        int lev = atoi(csvFile->getData(i, 0).c_str());
        if(lev == m_levelInfo.Levelnum)
        {
            CCLOG("levelinfo %d",lev);
            CCLog(csvFile->getData(i, 1).c_str(),NULL);
            string temp = csvFile->getData(i, 1);
            for (int i = 0; i < temp.length(); i++) {
                temp[i] = toupper(temp[i]);
            }
//            s_Eng -> initWithFormat("%s",csvFile->getData(i, 1).c_str());
//            s_Tran -> initWithFormat("%s",csvFile->getData(i, 2).c_str());
            s_Eng -> initWithFormat("%s",temp.c_str());
            s_Tran -> initWithFormat("%s",csvFile->getData(i, 1).c_str());

            if(s_Num != NULL)
            {
                CCLog("Added");
                CCArray* row = new CCArray;
                row->initWithCapacity(2);
                row->addObject(s_Eng);
                row->addObject(s_Tran);
                wordinfo->addObject(row);
            }
        }
    }
    updatelbltarget();
}

void CarGame::updatelbltarget()
{
    int target = arc4random() % wordinfo->count();
    CCArray *targetrow = (CCArray*) wordinfo->objectAtIndex(target);
    CCString *targetword = (CCString*) targetrow->objectAtIndex(0);
    lbl_Target->setString(targetword->getCString());
    lbl_Target->setTag(target);
}

void CarGame::landcar()
{
    m_car->setTag(1);
    m_car->runAction(CCEaseIn::create(CCMoveBy::create(0.3f, ccp(0,-100)),5));
}

void CarGame::stoppowerspeed()
{
    obstacleinterval = obstacleinterval*2.0f;
    for (int i = 0; i < m_aryObstacle->count(); i++)
    {
        Obstacle* sp = (Obstacle*)m_aryObstacle->objectAtIndex(i);
        sp->unscheduleAllSelectors();
        sp->schedule(schedule_selector(Obstacle::update), obstacleinterval);
    }
}

void CarGame::stopRunning()
{
    iscrashing = true;
    CCObject* road;
    CCARRAY_FOREACH(m_aryRoadTexture, road)
    {
        ((CCSprite*)road)->pauseSchedulerAndActions();
    }
//    m_car->pauseSchedulerAndActions();
    
    CCObject* obstacle;
    CCARRAY_FOREACH(m_aryObstacle, obstacle)
    ((Obstacle*)obstacle)->unscheduleAllSelectors();
    
    scheduleOnce(schedule_selector(CarGame::continueRunning), 2.0f);
}

void CarGame::continueRunning()
{
    iscrashing = false;
    CCObject* road;
    CCARRAY_FOREACH(m_aryRoadTexture, road)
    {
        ((CCSprite*)road)->resumeSchedulerAndActions();
    }
    
//    m_car->resumeSchedulerAndActions();
    
    CCObject* obstacle;
    CCARRAY_FOREACH(m_aryObstacle, obstacle)
    ((Obstacle*)obstacle)->schedule(schedule_selector(Obstacle::update), obstacleinterval);
}

void CarGame::menuCallback(CCObject* pSender)
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
        
        onClickMenuBtn();
        
        scheduleOnce(schedule_selector(CarGame::goGameSelect), 0.4f);

        
    }
}



void CarGame::goGameSelect()
{
    CCDirector::sharedDirector()->replaceScene(GameSelectScene::scene());
}

void CarGame::goGameOver()
{
    CCSprite* game_over_sprite = CCSprite::create("game_over.png");
    game_over_sprite->setScale(g_fResScale);
    this->addChild(game_over_sprite);
    game_over_sprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->unschedule(schedule_selector(CarGame::update));
    this->setTouchEnabled(false);
    this->scheduleOnce(schedule_selector(CarGame::goGameSelect), 3.0f);
}

void CarGame::ccTouchesBegan(cocos2d::CCSet* _touches, cocos2d::CCEvent* event)
{
    if(iscrashing)
        return;
    
    CCTouch* touch = (CCTouch*)_touches->anyObject();
    CCPoint touchPoint = touch->getLocationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);


    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_nPrevline = m_nRoadLine;
        
    if (touchPoint.x < winSize.width / 2)
    {
        if(m_nRoadLine == 0)
            return;
        m_nRoadLine--;
        m_car->setAnimation(ani_car_name[2], false);

    }
    else
    {
        if(m_nRoadLine == ROAD_MAX - 1)
            return;
        m_nRoadLine++;
        m_car->setAnimation(ani_car_name[1], false);

    }
    
    setRoadLine(m_nRoadLine);

    for (int i = 0; i < m_aryObstacle->count(); i++)
    {
        Obstacle* sp = (Obstacle*)m_aryObstacle->objectAtIndex(i);
//        if (sp->m_nType == 5 || sp->m_nType == 6)
//        {
//            if(sp->m_nRoadIndex<m_nRoadLine)
//            {
//                sp->getChildByTag(1)->setVisible(true);
//                sp->getChildByTag(2)->setVisible(false);
//                sp->getChildByTag(0)->setVisible(false);
//            }
//            else if(sp->m_nRoadIndex>m_nRoadLine)
//            {
//                sp->getChildByTag(1)->setVisible(false);
//                sp->getChildByTag(2)->setVisible(true);
//                sp->getChildByTag(0)->setVisible(false);
//            }
//            else
//            {
//                sp->getChildByTag(1)->setVisible(false);
//                sp->getChildByTag(2)->setVisible(false);
//                sp->getChildByTag(0)->setVisible(true);
//            }
//
//        }

    }
    
    

}

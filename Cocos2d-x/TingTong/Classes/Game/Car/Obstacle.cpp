//
//  Obstacle.cpp
//  TingTong
//
//  Created by admin on 6/3/14.
//
//

#include "Obstacle.h"

#define CAR_SPEED 50.0f
const float roadline_posx_skewx[4][2] = {{-170, -40}, {-40, -15}, {30, 10}, {200, 39}};


void Obstacle::crushAnimation()
{
    if (m_nType== 0)
    {
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 7; j++) {
            char temp[20] = {0};
            sprintf(temp, "barrel_%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        
        CCAnimate *action = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(action,CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle)),NULL);
        this->runAction(seq);
        
        
    }
    
    
    if (m_nType == 1) {
        
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 6; j++) {
            char temp[20] = {0};
            sprintf(temp, "tire_%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        
        CCAnimate *action = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(action,CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle)),NULL);
        this->runAction(seq);
        
        
    }
    
    if (m_nType == 2) {
        
        
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 7; j++) {
            char temp[20] = {0};
            sprintf(temp, "pole_%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        this->setPositionY(this->getPositionY()+this->getContentSize().height*this->getScaleY());
        CCAnimate *action = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(action,CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle)),NULL);
        this->runAction(seq);
        
        
    }
    
    if (m_nType == 3) {
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 8; j++) {
            char temp[20] = {0};
            sprintf(temp, "rock_%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        
        CCAnimate *action = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(action,CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle)),NULL);
        this->runAction(seq);
        
    }
    
    if (m_nType == 4) {
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 7; j++) {
            char temp[20] = {0};
            sprintf(temp, "pole_%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        
        CCAnimate *action = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(action,CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle)),NULL);
        this->runAction(seq);
        
    }
    
    if (m_nType == 5) {
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 7; j++) {
            char temp[20] = {0};
            sprintf(temp, "wall_%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        
        CCAnimate *action = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(action,CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle)),NULL);
        this->runAction(seq);
    }
    
    if (m_nType == 6) {
        
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 7; j++) {
            char temp[20] = {0};
            sprintf(temp, "wall_%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        
        CCAnimate *action = CCAnimate::create(animation);
        CCSequence* seq = CCSequence::create(action,CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle)),NULL);
        this->runAction(seq);
        
    }
    
    
}

//remove the obstacle


//create the obstacle
Obstacle* Obstacle::create(int Type, int road, int roadst, CCArray* wordarray, int curtarget)
{
    int type = Type/10;
//    int type = 8;
    Obstacle* obstacle = new Obstacle;
    obstacle->init();
    
    CCSprite* pobSprite = new CCSprite;
    char pszFileName[30] = {0};
    if(type == 0)
    {
        pobSprite->initWithFile("barrel.png");

    }
    else if (type == 1) {
        pobSprite->initWithFile("tires.png");

    }
    
    else if (type == 2) {
        pobSprite->initWithFile("obstacle.png");

    }
    else if (type == 3) {
        pobSprite->initWithFile("stone.png");

    }
    else if (type == 4) {
        pobSprite->initWithFile("obstacle1.png");

    }
    
    else if (type == 5) {
        pobSprite->init();
        CCSprite* normal = CCSprite::create("wall.png");
        normal->setVisible(true);
        normal->setTag(0);
        normal->setAnchorPoint(ccp(0.5f, 0.0f));
        pobSprite->addChild(normal);
        CCSprite* Left = CCSprite::create("wallL.png");
        Left->setVisible(false);
        Left->setTag(1);
        Left->setAnchorPoint(ccp(0.5f, 0.0f));
        pobSprite->addChild(Left);
        CCSprite* Right = CCSprite::create("wallR.png");
        Right->setVisible(false);
        Right->setTag(2);
        Right->setAnchorPoint(ccp(0.5f, 0.0f));
        pobSprite->addChild(Right);
    }
    
    else if (type == 6) {
        pobSprite->init();
        CCSprite* normal = CCSprite::create("jboard.png");
        normal->setVisible(true);
        normal->setTag(0);
        normal->setAnchorPoint(ccp(0.5f, 0.0f));
        pobSprite->addChild(normal);
        CCSprite* Left = CCSprite::create("jboardL.png");
        Left->setVisible(false);
        Left->setTag(1);
        Left->setAnchorPoint(ccp(0.5f, 0.0f));
        pobSprite->addChild(Left);
        CCSprite* Right = CCSprite::create("jboardR.png");
        Right->setVisible(false);
        Right->setTag(2);
        Right->setAnchorPoint(ccp(0.5f, 0.0f));
        pobSprite->addChild(Right);
    }

    else if (type == 7) {
        pobSprite->initWithFile("coin1.png");
        CCAnimation *animation = CCAnimation::create();
        
        for (int j = 1; j < 5; j++) {
            char temp[20] = {0};
            sprintf(temp, "coin%d.png", j);
            animation->addSpriteFrameWithFileName(temp);
            
        }
        animation->setDelayPerUnit(1.0f / 14.0f);
//        animation->setRestoreOriginalFrame(false); // Return to the 1st frame after the 14th frame is played.
        
        CCAnimate *action = CCAnimate::create(animation);
        pobSprite->runAction(CCRepeatForever::create(action));
    }
    
    else if (type == 8) {
        pobSprite->initWithFile("puddle.png");
        
    }
    
    if ((Type % 10) > 0)
    {        
        CCLabelTTF* name_label = CCLabelTTF::create("", "starjout.ttf", 180 * g_fResScale);
        name_label->setTag(1);
        
        CCLabelTTF* name_labelframe = CCLabelTTF::create("", "starjout.ttf", 180 * g_fResScale);
        name_labelframe->setTag(1);
        
        int target = arc4random() % wordarray->count();
        
        if ((Type % 10) == 2)
        {
            target = curtarget;
        }
        
        
        CCArray *targetrow = (CCArray*) wordarray->objectAtIndex(target);
        CCString *targetword = (CCString*) targetrow->objectAtIndex(1);
        name_label->setString(targetword->getCString());
        name_labelframe->setString(targetword->getCString());
        name_labelframe->enableStroke(ccWHITE, 5);
        obstacle->setTag(target);
        
        name_label->setColor(ccBLACK);
        name_label->setAnchorPoint(ccp(0.0f,0.0f));
        name_labelframe->setColor(ccWHITE);
        name_labelframe->setAnchorPoint(ccp(0.5f,0.0f));
        
        if (type==5)
        {
//            name_label->setPosition(ccp(pobSprite->getContentSize().width/2, pobSprite->getChildByTag(0)-> getContentSize().height));
            name_labelframe->setPosition(ccp(0, pobSprite->getChildByTag(0)-> getContentSize().height));

        }
        else
        {
//            name_label->setPosition(ccp(pobSprite->getContentSize().width/2, pobSprite->getContentSize().height));
            name_labelframe->setPosition(ccp(0, pobSprite->getContentSize().height));

        }
        obstacle->addChild(name_labelframe);
        name_labelframe->addChild(name_label);
    }
    pobSprite->setTag(11);
    obstacle->addChild(pobSprite);
    obstacle->m_nType = type;
    obstacle->m_nRoadIndex = road;
    obstacle->setSkewX(-roadline_posx_skewx[roadst][1]);
    obstacle->setSkewY(0.0f);
    obstacle->setAnchorPoint(ccp(0.5f,0.0f));
    
    return obstacle;
    
    CC_SAFE_DELETE(obstacle);
    
    
}

void Obstacle::update()
{
    float dh = CAR_SPEED * g_fResScale;
    time = time + 0.02;
    
    CCPoint pt = this->getPosition();
    float xratio = m_gamescene->m_fRoadScale[16][0];;
    float yratio = m_gamescene->m_fRoadScale[16][1];
    float m_sx = Config::getxScale();
    float m_sy = Config::getyScale();
    for (int i = 1; i < 17 ; i++)
    {
        if(this->getPositionY() < m_gamescene->m_fRoadY[i])
        {
            xratio = m_gamescene->m_fRoadScale[i-1][0];
            yratio = m_gamescene->m_fRoadScale[i-1][1];
            this->setZOrder(17-i+100);
            break;
        }
        
    }
//           if(this->m_nRoadIndex == 0)
//           pt.x -= 26 * m_sx * time/5.0f;
//           
//           else if(this->m_nRoadIndex == 1)
//           pt.x -= 8 * m_sx * time/5.0f;
//           
//           else if(this->m_nRoadIndex == 2)
//           pt.x += 8 * m_sx * time/5.0f;
//           
//           else if(this->m_nRoadIndex == 3)
//           pt.x += 27 * m_sx * time/5.0f;
    
//           pt.y -= dh * time/5.0f;
    
    if(this->m_nRoadIndex == 0)
        pt.x -= 15.0f * xratio * g_fResScale;
    
    else if(this->m_nRoadIndex == 1)
        pt.x -= 5.0f * xratio * g_fResScale;
    
    else if(this->m_nRoadIndex == 2)
        pt.x += 5.0f * xratio * g_fResScale;
    
    else if(this->m_nRoadIndex == 3)
        pt.x += 15.0f * xratio * g_fResScale;
    
    pt.y -= dh * yratio;
    
           this->setPosition(pt);
           //float fs = (620 * m_sy - pt.y) / (620 * m_sy);
    if(this->m_nType == 8)
    {
        this->setScaleX(time*0.5f*g_fResScale);
        this->setScaleY(time*0.1f*g_fResScale);

    }
    else
        this->setScale(time*0.5f*g_fResScale);
}


void Obstacle::AnimateTrue()
{
    this->unschedule(schedule_selector(Obstacle::update));
    CCLabelTTF* label = (CCLabelTTF*)this->getChildByTag(1)->getChildByTag(1);
    CCSequence* tint = CCSequence::create(CCTintTo::create(0.3f, 0.0f, 170.0f, 0.0f),CCTintTo::create(0.3f, 255.0f, 255.0f,255.0f),NULL);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(Obstacle::RemoveLabel));
    CCSequence* seq = CCSequence::create(tint,func,NULL);
    label->runAction(seq);
    
    CCSprite* pobsprite = (CCSprite*)this->getChildByTag(11);
    CCScaleBy* scale = CCScaleBy::create(0.3f, 0.0f, -0.5f*g_fResScale);
    CCRotateBy* rotate = CCRotateBy::create(0.3f, 30.0f);
    CCMoveBy* move = CCMoveBy::create(0.3f, ccp(0.0f,-200.0f));
    CCSpawn* spawn = CCSpawn::create(scale,rotate,move,NULL);
    CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle));
    pobsprite->runAction(CCSequence::create(spawn,func1,NULL));
}

void Obstacle::AnimateFalse()
{
    this->unschedule(schedule_selector(Obstacle::update));
    CCSequence* tint = CCSequence::create(CCTintTo::create(0.2f, 170.0f, 0.0f, 0.0f),CCTintTo::create(0.2f, 255.0f, 255.0f,255.0f),NULL);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(Obstacle::removeObstacle));
    CCSequence* seq = CCSequence::create(tint,func,NULL);
    
    CCSprite* pobsprite = (CCSprite*)this->getChildByTag(11);
    pobsprite->runAction(seq);

}

void Obstacle::removeObstacle()
{
    
    this->removeFromParentAndCleanup(true);
    
}

void Obstacle::RemoveLabel()
{
    this->removeChildByTag(1);
}
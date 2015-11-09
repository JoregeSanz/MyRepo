//
//  Object.cpp
//  TingTong
//
//  Created by lion on 6/25/14.
//
//

#include "Object.h"
#include "Define.h"
#include "NinjaScene.h"
#include <stdio.h>
#include <fstream>

USING_NS_CC;

const char* objectname[14] = {"woodenb","watermelon","keyboard","pillow","sack","shoe","dynamite","box","money","tv","tomato","candy","bottle","glass"};

bool Object::create(b2World* w, NinjaScene* scene, int direction,int type,int curtarget,CCArray* wordarray)
{
    world = w;
    if(!CCNode::create())
       return false;
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.userData = this;
    
    if(direction < 3)
        bd.position.Set(0.0f, WinHeight/PTM_RATIO/4.0f*(1+direction));
    else if (direction>9)
        bd.position.Set(WinWidth/PTM_RATIO, WinHeight/PTM_RATIO/4.0f*(direction-10));
    else
        bd.position.Set(WinWidth/PTM_RATIO/8.0f*(direction-2), 0.0f);

    b2CircleShape circle;
    circle.m_radius = 2.0 * BodyScale;
    b2FixtureDef fd;
    fd.shape = &circle;
    fd.density = 1.0f;
    fd.filter.maskBits = 0;
    
    body = world->CreateBody(&bd);
    body->CreateFixture(&fd);

    if(direction<3)
        body->SetLinearVelocity(b2Vec2(1.5f, 5));
    else if(direction>9)
        body->SetLinearVelocity(b2Vec2(-1.5f, 5));
    else
        body->SetLinearVelocity(b2Vec2(arc4random()%2, 7));

    body->SetAngularVelocity(arc4random()%2/10.0f);
    
    char name[20] = {0};    
    sprintf(name, "%s.png",objectname[type/10]);
    
    
    sprite = new PhysicsSprite;
    sprite->init(name);
    sprite->setPhysicsBody(body);
    sprite->objarray = scene->objary;
    sprite->obj = this;
    sprite->setTag(type/10);
    
    if ((type % 10) > 0)
    {
        CCLabelTTF* name_label = CCLabelTTF::create("", "starjout.ttf", 100 * g_fResScale);
        name_label->setTag(1);
        
        CCLabelTTF* name_labelframe = CCLabelTTF::create("", "starjout.ttf", 100 * g_fResScale);
        name_labelframe->setTag(1);
        
        int target = arc4random() % wordarray->count();
        
        if ((type % 10) == 2)
        {
            target = curtarget;
        }
        
        
        CCArray *targetrow = (CCArray*) wordarray->objectAtIndex(target);
        CCString *targetword = (CCString*) targetrow->objectAtIndex(1);
        name_label->setString(targetword->getCString());
        name_labelframe->setString(targetword->getCString());
        name_labelframe->enableStroke(ccWHITE, 5);
        this->setTag(target);
        
        name_label->setColor(ccBLACK);
        name_label->setAnchorPoint(ccp(0.0f,0.0f));
        name_labelframe->setColor(ccWHITE);
        name_labelframe->setAnchorPoint(ccp(0.5f,0.5f));
        name_labelframe->setPosition(ccp(sprite->getContentSize().width/2.0f,sprite->getContentSize().height/2.0f));
        name_labelframe->addChild(name_label);

        sprite->addChild(name_labelframe);
    }
    else
        this->setTag(-1);
    
    
    scene->addChild(sprite);
        
    return true;
} 
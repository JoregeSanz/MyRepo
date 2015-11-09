//
//  PhysicsSprite.h
//  TingTong
//
//  Created by lion on 6/25/14.
//
//

#ifndef __TingTong__PhysicsSprite__
#define __TingTong__PhysicsSprite__

#include <iostream>
#include "Box2D.h"
#include "cocos2d.h"
class Object;

USING_NS_CC;

class PhysicsSprite:public CCSprite
{
private:
    
public:
    b2Body* body;
    
public:
    CCArray* objarray;
    Object* obj;
    PhysicsSprite* init(char*);
    PhysicsSprite* initwithtexture(CCTexture2D*,CCRect);
    void setPhysicsBody(b2Body*);
    void update(float dt);
    
};
#endif /* defined(__TingTong__PhysicsSprite__) */

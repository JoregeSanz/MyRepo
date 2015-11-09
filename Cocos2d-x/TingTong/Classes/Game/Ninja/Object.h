//
//  Object.h
//  TingTong
//
//  Created by lion on 6/25/14.
//
//

#ifndef __TingTong__Object__
#define __TingTong__Object__

#include <iostream>
#include "Box2D.h"
#include "cocos2d.h"
#include "PhysicsSprite.h"
class NinjaScene;

USING_NS_CC;
class Object:public CCNode {
private:
    
public:
    b2World* world;
    b2Body* body;
    PhysicsSprite* sprite;
    virtual bool create(b2World* w, NinjaScene* scene, int direction,int type,int curtarget,CCArray* wordarray);
    
};
#endif /* defined(__TingTong__Object__) */

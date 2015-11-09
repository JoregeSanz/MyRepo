//
//  PhysicsSprite.cpp
//  TingTong
//
//  Created by lion on 6/25/14.
//
//

#include "PhysicsSprite.h"
#include "NinjaScene.h"
#include "Object.h"

USING_NS_CC;




PhysicsSprite* PhysicsSprite::init(char* name)
{
    this->initWithFile(name);
    return this;
}

PhysicsSprite* PhysicsSprite::initwithtexture(CCTexture2D* texture, CCRect rect)
{
    this->initWithTexture(texture, rect);
    return this;
}

void PhysicsSprite::setPhysicsBody(b2Body* bd)
{
    this->setScale(g_fResScale);
    body = bd;
    schedule(schedule_selector(PhysicsSprite::update),0.025f);
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.


void PhysicsSprite::update(float dt)
{
    if (body != NULL)
    {
        b2Vec2 bodypos = body->GetPosition();
        this->setPosition(ccp(bodypos.x * PTM_RATIO, bodypos.y * PTM_RATIO));
        this->setRotation(CC_RADIANS_TO_DEGREES(-body->GetAngle()));
        
        if(body->GetPosition().y < 0)
        {
            b2World* world = body->GetWorld();
            world->DestroyBody(body);
            this->removeFromParentAndCleanup(true);
            if((Object*)body->GetUserData() != NULL)
            {
                objarray->removeObject((Object*)body->GetUserData());
                delete obj;
                obj = NULL;
            }
        }
    }
}
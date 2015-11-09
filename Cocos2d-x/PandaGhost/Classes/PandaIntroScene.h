//
//  PandaIntroScene.h
//  PacMan
//
//  Created by jyn on 2/1/14.
//
//

#ifndef __PacMan__PandaIntroScene__
#define __PacMan__PandaIntroScene__

#include "cocos2d.h"
#include "Config.h"

class PandaIntroScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    CCSprite* man;
    CCArray* m_aryPellet;
    
    int m_nEatIndex;
    void animateEat();
    void finish();
    
    void draw(void);
    
    ~PandaIntroScene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PandaIntroScene);
};

#endif /* defined(__PacMan__PandaIntroScene__) */

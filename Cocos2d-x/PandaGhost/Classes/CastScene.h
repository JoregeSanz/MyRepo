//
//  CastScene.h
//  PacMan
//
//  Created by jyn on 2/2/14.
//
//

#ifndef __PacMan__CastScene__
#define __PacMan__CastScene__

#include "cocos2d.h"
using namespace cocos2d;

class CastScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    CCLabelTTF* m_lbCast[5];
    void finish();
    void draw(void);
    int frame;
    void animationText();
    
    void goGameScene();
    
    ~CastScene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CastScene);
};

#endif /* defined(__PacMan__CastScene__) */

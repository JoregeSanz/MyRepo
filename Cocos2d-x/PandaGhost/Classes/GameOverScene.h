//
//  GameOverScene.h
//  PacMan
//
//  Created by jyn on 2/1/14.
//
//

#ifndef __PacMan__GameOverScene__
#define __PacMan__GameOverScene__

#include "cocos2d.h"
#include "Config.h"

class GameOverScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    CCSprite* m_spGameOver;
    void onPlayAgain(CCObject* pSender);
    void onFacebook(CCObject* pSender);
    void onTwitter(CCObject* pSender);
    void onGoogle(CCObject* pSender);
    
    void replaceGameScene();
    ~GameOverScene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameOverScene);
};

#endif /* defined(__PacMan__GameOverScene__) */

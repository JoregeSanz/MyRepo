//
//  WinScene.h
//  PacMan
//
//  Created by jyn on 2/1/14.
//
//

#ifndef __PacMan__WinScene__
#define __PacMan__WinScene__

#include "cocos2d.h"
#include "Config.h"

class WinScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    CCSprite* man;
    
    char *m_map[MAX_LEVEL];
    int m_nRow;
    int m_nCol;

//    virtual void draw(void);
//    void addWallPath(int level, int x, int y, char s);
//    void setLevel(int nLevel);
    
    void finishWinAnimation();
    
    ~WinScene();
    
    void replaceGameScene();
    void replaceTitleScene();
    
    void onPlayAgain(CCObject* pSender);
    
    void onFacebook(CCObject* pSender);
    void onTwitter(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(WinScene);
};

#endif /* defined(__PacMan__WinScene__) */

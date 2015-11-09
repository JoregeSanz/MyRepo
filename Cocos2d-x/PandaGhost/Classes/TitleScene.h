#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class TitleScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void onStartBtn(CCObject* pSender);
    void goGameScene();
    
    void animateTitle();
    ~TitleScene();
    int m_nLetterIndex;
    
    //dukce.pak 2014-04-10 reason: add two screen after title screen
    CCSprite * m_introSprite;
    void introduction();
    int m_pageIndex;

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(TitleScene);
};

#endif // __TITLE_SCENE_H__

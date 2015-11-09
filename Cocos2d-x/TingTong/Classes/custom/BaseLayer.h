//
//  BaseLayer.h
//  TingTong
//
//  Created by jyn on 4/12/14.
//
//

#ifndef __TingTong__BaseLayer__
#define __TingTong__BaseLayer__

#include "cocos2d.h"
#include "BgLayer.h"
#include "MyMenuItem.h"
#include "Config.h"
#include "Define.h"
#include "CCRippleSprite.h"

USING_NS_CC;

class BaseLayer : public cocos2d::CCLayer
{
private:

    CCSprite* m_spArc;
    CCMenu *m_menu;
    bool m_bMenuShow;
    MyMenuItem *menuItem;
    
    void bg_map();
    void bg_1();
    void bg_18();
    
    //bg1
    pgeRippleSprite *m_river;
    CCLayer* book;
    void onOpenedBook();
    void animClounds();
    
public:
    CCSize winSize;

    BgLayer* m_bgLayer;
    
    MyMenuItem *returnItem;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    void setBackground(int n);

    void onClickMenuBtn();

    // a selector callback
    void onMenuBtn(CCObject* pSender);
    virtual void menuCallback(CCObject* pSender);
    
    virtual void update(float delta);
    
    // implement the "static node()" method manually
    CREATE_FUNC(BaseLayer);
    
};

#endif /* defined(__TingTong__BaseLayer__) */

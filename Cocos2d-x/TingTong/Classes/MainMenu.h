//
//  MainMenu.h
//  TingTong
//
//  Created by jyn on 4/10/14.
//
//

#ifndef __TingTong__MainMenu__
#define __TingTong__MainMenu__

#include "cocos2d.h"
#include "BaseLayer.h"
USING_NS_CC;

class MainMenu : public BaseLayer
{
private:
    
    CCSprite* m_spLearn;
    CCSprite* m_spWith;
    CCMenu *m_mainmenu;
    CCMenu* m_extramenu;
    void appearAnimation();
    void disappearAnimation();
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    void play();
    void review();
    void gameSelect();
    void statistics();
    void connectFB();
    void moregames();
    // implement the "static node()" method manually
    CREATE_FUNC(MainMenu);
    
    
};

#endif /* defined(__TingTong__MainMenu__) */

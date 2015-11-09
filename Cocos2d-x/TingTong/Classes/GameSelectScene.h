//
//  GameSelectScene.h
//  TingTong
//
//  Created by jyn on 4/10/14.
//
//

#ifndef __TingTong__GameSelectScene__
#define __TingTong__GameSelectScene__

#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class GameSelectScene : public BaseLayer
{
private:
    bool carselected;
    bool fruitselected;
    
    void goGame();
    
    void onGameObject(CCObject* pSender);
    void onPlay(CCObject* pSender);
    void onMemorize(CCObject* pSender);
    void onClose(CCObject* pSender);

    void createBoardMenu();
    void showBoardMenu();
    void hideBoardMenu();

    CCLayer* m_boardMenu;
    
    CCMenu* menu;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameSelectScene);
    
    
};

#endif /* defined(__TingTong__GameSelectScene__) */

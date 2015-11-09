//
//  LogoScene.h
//  TingTong
//
//  Created by jyn on 4/10/14.
//
//

#ifndef __TingTong__LogoScene__
#define __TingTong__LogoScene__

#include "cocos2d.h"
USING_NS_CC;

class LogoScene : public cocos2d::CCLayer
{
private:

    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    void mainMenu();
    
    // implement the "static node()" method manually
    CREATE_FUNC(LogoScene);
    
    
};
#endif /* defined(__TingTong__LogoScene__) */

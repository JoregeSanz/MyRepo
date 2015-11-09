//
//  StatisticsScene.h
//  TingTong
//
//  Created by jyn on 4/12/14.
//
//

#ifndef __TingTong__StatisticsScene__
#define __TingTong__StatisticsScene__

#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class StatisticsScene : public BaseLayer
{
private:
    
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    void menuCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(StatisticsScene);
    
    
};

#endif /* defined(__TingTong__StatisticsScene__) */

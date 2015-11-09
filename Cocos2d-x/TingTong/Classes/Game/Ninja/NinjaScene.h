//
//  NinjaScene.h
//  TingTong
//
//  Created by lion on 6/24/14.
//
//

#ifndef __TingTong__NinjaScene__
#define __TingTong__NinjaScene__

#include <iostream>
#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>
#include "CCSvParse.h"
#include "Box2D.h"
#include "GLES-Render.h"
#include "CCBlade.h"
#include "BaseLayer.h"
#include "Object.h"

USING_NS_CC;
using namespace cocos2d::extension;

class NinjaScene:public BaseLayer
{
private:
    std::map< cocos2d::CCTouch *, CCBlade * >  _map;
    CCLabelTTF* lbl_levelNum;
    CCLabelTTF* lbl_Score;
    CCLabelTTF* lbl_Time;
    CCLabelTTF* lbl_Target;
    
    CCPoint startpos;
    CCPoint endpos;

    
    int m_nTicknum;
    float timetemp;
    
    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void readWordInfo();
    void updatelbltarget();
    void goGameSelect();
    void goGameOver();
    void removeColorLayer(CCLayerColor* sender);
public:
    b2World* world;
    GLESDebugDraw *m_debugDraw;		// strong ref
    LevelInfo m_levelInfo;
    CCArray* wordinfo;
    CCSVParse* levelinfo;
    CCArray* objary;

    virtual bool init();
    virtual void draw();
    static CCScene* scene();
    void update(float dt);
    void tossobject();
    void menuCallback(CCObject* pSender);

    CREATE_FUNC(NinjaScene)
};
#endif /* defined(__TingTong__NinjaScene__) */

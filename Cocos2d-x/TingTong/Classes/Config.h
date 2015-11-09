//
//  Config.h
//  QuillysRun
//
//  Created by Ry on 2013.9.5
//
//

#ifndef __Config_h__
#define __Config_h__

#include "cocos2d.h"
#include "Define.h"

USING_NS_CC;


class MenuAction
{
public:
    static CCRepeatForever* defaultMenuAction();
    static CCSequence* cornerMenuAction();
};

class Config
{
public:
    static void setScaleFactor(CCSize size);
    static void setScale(CCNode *node);
    static void setScale(CCNode *node, float factorX, float factorY);
      
    static float getX(float x);
    static float getY(float y);
    static float getyScale();
    static float getxScale();
    
};
static float __attribute__((unused)) _scaleX;
static float __attribute__((unused)) _scaleY;

#endif /* defined(__Config_h__) */

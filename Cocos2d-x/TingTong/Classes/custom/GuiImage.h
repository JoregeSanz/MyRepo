//
//  GuiImage.h
//  TingTong
//
//  Created by jyn on 4/11/14.
//
//

#ifndef __TingTong__GuiImage__
#define __TingTong__GuiImage__

#include "AnimClasses.h"

#include "cocos2d.h"

USING_NS_CC;

class GuiImage : public CCSprite
{
public:
    CCPoint RelPos;
    int Width = 0;
    int Height = 0;
    float PivotX = 0;
    float PivotY = 0;
    float Rot = 0.0f;
    float Scale = 1.0f;
    float ResScale;
    int BgColor = 0xFFFFFFFF;
    CCSprite* BgSprite;

public:
    AnimParams* Animator;
    void setPosition(CCPoint pt);
    static GuiImage* create(const char *pszFileName);
    bool onUpdate(float dt, bool parentMoved);
    void onUpdateRec(float dt, bool parentMoved);
};
#endif /* defined(__TingTong__GuiImage__) */

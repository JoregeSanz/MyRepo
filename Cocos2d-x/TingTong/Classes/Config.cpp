//
//  Config.cpp
//  QuillysRun
//
//  Created by Ry on 2013.9.5
//
//

#include "Config.h"

CCRepeatForever* MenuAction::defaultMenuAction()
{
    CCScaleTo* scl1 = CCScaleTo::create(0.5, g_fResScale * 1.09, g_fResScale * 0.9);
    CCScaleTo* scl2 = CCScaleTo::create(0.5, g_fResScale * 0.9, g_fResScale * 1.09);
    CCSequence* seq = CCSequence::create(scl1, scl2, NULL);
    CCRepeatForever* rep = CCRepeatForever::create(seq);
    rep->setTag(kDefaultActionTag);
    return rep;

}
CCSequence* MenuAction::cornerMenuAction()
{
    return CCSequence::create( CCScaleTo::create(0.2, g_fResScale * 1.1),
                              CCScaleTo::create(0.15, g_fResScale * 0.95),
                              CCScaleTo::create(0.1, g_fResScale * 1),
                              NULL);
}



void Config::setScaleFactor(CCSize size)
{
    _scaleX = size.width / DESIGN_RESOLUTION_WIDTH;
    _scaleY = size.height / DESIGN_RESOLUTION_HEIGHT;
}

void Config::setScale(CCNode *node)
{
    node->setScaleX(_scaleX);
    node->setScaleY(_scaleY);
    
}

void Config::setScale(CCNode *node, float factorX, float factorY)
{
    node->setScaleX(_scaleX * factorX);
    node->setScaleY(_scaleY * factorY);
}

float Config::getX(float x)
{
    return _scaleX * x;
}

float Config::getyScale()
{
    return _scaleY;
}

float Config::getxScale()
{
    return _scaleX;
}

float Config::getY(float y)
{
    return _scaleY * y;
}

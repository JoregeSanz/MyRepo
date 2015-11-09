//
//  BgLayer.h
//  TingTong
//
//  Created by jyn on 4/11/14.
//
//

#ifndef __TingTong__BgLayer__
#define __TingTong__BgLayer__

#include "cocos2d.h"
#include "GuiImage.h"

USING_NS_CC;


class BgLayer : public CCLayer
{
public:
    static BgLayer *create();
    void update(float delta);

};

#endif /* defined(__TingTong__BgLayer__) */

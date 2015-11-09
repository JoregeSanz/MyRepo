//
//  BgLayer.cpp
//  TingTong
//
//  Created by jyn on 4/11/14.
//
//

#include "BgLayer.h"
#include "Define.h"

BgLayer *BgLayer::create()
{
    BgLayer *pRet = new BgLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
void BgLayer::update(float delta)
{
    CCArray* childs = getChildren();
    if (childs) {
        for (int i = 0; i < childs->count(); i++) {
            CCNode *child = (CCNode*)childs->objectAtIndex(i);
            GuiImage* g = dynamic_cast<GuiImage*>(child);
//            ClipNode* c = dynamic_cast<ClipNode*>(child);
            if(g != 0) {
                g->onUpdateRec(delta, false);
            }
//            else if(c){
//                for (int i = 100 ; i < 120; i++) {
//                    if (c->getChildByTag(i) != NULL){
//                        GuiImage* gg = (GuiImage*)c->getChildByTag(i);
//                        gg->onUpdateRec(delta, false);
//                    }
//                    else
//                        break;
//                }
//            }
        }
        
    }
}

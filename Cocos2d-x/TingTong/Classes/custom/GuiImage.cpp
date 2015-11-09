//
//  GuiImage.cpp
//  TingTong
//
//  Created by jyn on 4/11/14.
//
//

#include "GuiImage.h"

GuiImage* GuiImage::create(const char *pszFileName)
{
    GuiImage *pobSprite = new GuiImage();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}
bool GuiImage::onUpdate(float dt, bool parentMoved)
{
    bool moved = parentMoved;
    CCPoint pt = RelPos;
    if(Animator != NULL)
    {
        AnimParams* ani = Animator;
        if(ani->Update(dt))
        {
            moved = true;
            pt = ani->getPos(RelPos);
            Rot = ani->getRot(Rot);
            Scale = ani->getScale(Scale);
            
        }
        RelPos = pt;
        this->setPosition(pt);
        this->setRotation(Rot);
        this->setScale(Scale);

        BgColor = Animator->getColor(BgColor);
        int  Color = BgColor;//|0xFF000000;//jin
        int r = (Color>>16) & 0xff;
        int g = (Color>>8) & 0xff;
        int b = Color & 0xff;
        int a = (Color>>24) & 0xff;
//        if(BgSprite != NULL){
//            BgSprite->setColor(ccc3(r, g, b));
//            BgSprite->setOpacity(a);
//        }
        setColor(ccc3(r, g, b));
        setOpacity(a);

    }
    return moved;
}
void GuiImage::setPosition(CCPoint pt)
{
    RelPos = pt;
    CCSprite::setPosition(pt);
}
void GuiImage::onUpdateRec(float dt, bool parentMoved)
{
    bool moved = onUpdate(dt, parentMoved);
    
    CCArray* childs = getChildren();
    if (childs) {
        for (int i = 0; i < childs->count(); i++) {
            GuiImage* g = (GuiImage*)childs->objectAtIndex(i);
            if (g) {
                g->onUpdateRec(dt, moved||parentMoved);
            }
        }
        
    }
    
}

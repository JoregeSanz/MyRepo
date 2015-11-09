//
//  MenuScene.cpp
//  QuillysRun
//
//  Created by Ry on 2013.9.2
//

#include "MyMenuItem.h"
#include "Config.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define MENU_ITEM "pop3.mp3"//moon

MyMenuItem* MyMenuItem::create(const char *normalImage, CCObject* target, SEL_MenuHandler selector)
{
    MyMenuItem *pobSprite = new MyMenuItem();
    if (pobSprite && pobSprite->initWithNormalImage(normalImage, normalImage, normalImage, target, selector))
    {
        pobSprite->setScale(g_fResScale);
        pobSprite->isDefaultAction = true;
        pobSprite->runAction(MenuAction::defaultMenuAction());
        pobSprite->autorelease();
        
        
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;

}
MyMenuItem* MyMenuItem::create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
{
    MyMenuItem *pobSprite = new MyMenuItem();
    if (pobSprite && pobSprite->initWithNormalImage(normalImage, selectedImage, normalImage, target, selector))
    {
        
        pobSprite->autorelease();
        
        
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;
    
}
CCSize MyMenuItem::getRealSize()
{
    CCSize sz = getContentSize();
    sz.width *= g_fResScale;
    sz.height *= g_fResScale;
    return sz;
}
void MyMenuItem::selected()
{
    // subclass to change the default action
    if(isEnabled()) {
        SimpleAudioEngine::sharedEngine()->playEffect(MENU_ITEM);//moon

        CCMenuItem::selected();
        scx = getScaleX();
        scy = getScaleY();
        stopActionByTag(kDefaultActionTag);
        stopActionByTag(kZoomActionTag);
        CCAction *zoomAction = CCScaleTo::create(0.1f, scx*0.8f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}
void MyMenuItem::unselected()
{
    // subclass to change the default action
    if(isEnabled()) {
        CCMenuItem::unselected();
        stopActionByTag(kZoomActionTag+1);
        CCScaleTo *zoomAction = CCScaleTo::create(0.1f, scx*1.0f);
        zoomAction->setTag(kZoomActionTag+1);
        CCSequence* seq = CCSequence::create(zoomAction, CCCallFunc::create(this, callfunc_selector(MyMenuItem::resumeDefaultAction)), NULL);
        this->runAction(seq);
    }
}
void MyMenuItem::resumeDefaultAction()
{
    // subclass to change the default action
    if(isEnabled() && isDefaultAction) {
        this->runAction(MenuAction::defaultMenuAction());
    }
}


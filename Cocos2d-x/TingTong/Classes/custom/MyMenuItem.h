//
//  MenuScene.h
//  QuillysRun
//
//  Created by Ry on 2013.9.2
//

#ifndef __MyMenuItem_h__
#define __MyMenuItem_h__

#include "cocos2d.h"



USING_NS_CC;

class MyMenuItem : public cocos2d::CCMenuItemImage
{
    float scx;
    float scy;
public:
    static MyMenuItem* create(const char *normalImage, CCObject* target, SEL_MenuHandler selector);
    static MyMenuItem* create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector);
//    virtual bool init();
    CCSize getRealSize();
    virtual void selected();
    virtual void unselected();
    void    resumeDefaultAction();
    bool isDefaultAction;
    // Auto-generated function
    CREATE_FUNC(MyMenuItem);

};


#endif /* defined(__MyMenuItem_h__) */

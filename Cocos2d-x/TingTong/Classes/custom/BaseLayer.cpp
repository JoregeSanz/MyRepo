//
//  BaseLayer.cpp
//  TingTong
//
//  Created by jyn on 4/12/14.
//
//

#include "BaseLayer.h"
#include "ClipNode.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

CCScene* BaseLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    BaseLayer *layer = BaseLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BaseLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    Config::setScaleFactor(winSize);
    
    
    // Set background
    m_bgLayer = BgLayer::create();
    m_bgLayer->setPosition(ccp(winSize.width*0.5, winSize.height*0.5));
    m_bgLayer->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(m_bgLayer);
    
    //CirleMenu
    m_bMenuShow = false;
    
    m_spArc = CCSprite::create("but5.png");
    m_spArc->setAnchorPoint(ccp(1, 1));
    m_spArc->setPosition(ccp(winSize.width, winSize.height));
    m_spArc->setScale(g_fResScale);
    this->addChild(m_spArc, 10);
    CCSize sz = m_spArc->getContentSize();
    sz.width *= g_fResScale;
    sz.height *= g_fResScale;
    m_spArc->setScale(0);
    
    MyMenuItem *soundItem = MyMenuItem::create("but8a.png",  this, menu_selector(BaseLayer::menuCallback));
    soundItem->setTag(MENU_SOUND_TAG);
    soundItem->setPosition(ccp(-sz.width * 0.85, - sz.height * 0.18));
    
    MyMenuItem *effectItem = MyMenuItem::create("but9a.png",  this, menu_selector(BaseLayer::menuCallback));
    effectItem->setTag(MENU_EFFECT_TAG);
    effectItem->setPosition(ccp(-sz.width * 0.765, - sz.height * 0.52));
    
    MyMenuItem *helpItem = MyMenuItem::create("but7a.png",  this, menu_selector(BaseLayer::menuCallback));
    helpItem->setTag(MENU_HELP_TAG);
    helpItem->setPosition(ccp(-sz.width * 0.52, - sz.height * 0.78));
    
    returnItem = MyMenuItem::create("but6a.png",  this, menu_selector(BaseLayer::menuCallback));
    returnItem->setTag(MENU_RETURN_TAG);
    returnItem->setPosition(ccp(-sz.width * 0.17, - sz.height * 0.86));
    
    m_menu = CCMenu::create(soundItem, effectItem, helpItem, returnItem, NULL);
    m_menu->setAnchorPoint(CCPointZero);
    m_menu->setPosition( ccp(winSize.width, winSize.height) );
    this->addChild(m_menu, 10);
    m_menu->setScale(0);
    
    
    menuItem = MyMenuItem::create("but4a.png",  this, menu_selector(BaseLayer::onMenuBtn));
    sz = menuItem->getRealSize();

    menuItem->setPosition(ccp(winSize.width/2 - sz.width * 0.1, winSize.height/2  - sz.height * 0.1));

    CCMenu* menu = CCMenu::create(menuItem, NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition( ccp(winSize.width*0.5, winSize.height*0.5) );
    this->addChild(menu);
    
    
    
    return true;
}
void BaseLayer::setBackground(int n){
    switch (n) {
        case -1://Map
            bg_map();
            break;
        case 1:
            bg_1();
            break;
        case 18:
            bg_18();
            break;

        default:
            break;
    }
    unschedule(schedule_selector(BaseLayer::update));
    schedule(schedule_selector(BaseLayer::update));
}
void BaseLayer::bg_map()
{
    CCSprite *background = CCSprite::create("map.jpg");
    Config::setScale(background);
    m_bgLayer->addChild(background);

}
void BaseLayer::bg_1()
{
    CCSize sz;
    CCSprite *background = CCSprite::create("sky.png");
    Config::setScale(background);
    sz = background->getContentSize();
    sz.width *= Config::getxScale();
    sz.height *= Config::getyScale();
    background->setPosition(ccp(0, (winSize.height - sz.height) / 2));
    m_bgLayer->addChild(background);
    

//    CCParticleSystem*sun = CCParticleSun::create();
//    sun->retain();
//    sun->setSpeed(0.04f);
//    m_bgLayer->addChild(sun);
//    sun->setTexture( CCTextureCache::sharedTextureCache()->addImage("sun1.png") );
//    sun->setPosition(0, 10 * Config::getyScale());
//    sun->setScale(g_fResScale * 5);

    CCSprite *land = CCSprite::create("land.png");
    Config::setScale(land);
    CCSize sz2 = land->getContentSize();
    sz2.width *= Config::getxScale();
    sz2.height *= Config::getyScale();
    land->setPosition(ccp(0, -210*Config::getyScale()));
    m_bgLayer->addChild(land);


    m_river = pgeRippleSprite::create("river.png");
    m_river->setCenter(ccp(960, 380));
    Config::setScale(m_river);
    m_river->setPosition(ccp(-winSize.width/2, -winSize.height/2));
    m_bgLayer->addChild(m_river);
    

    animClounds();
    

    float baloon_Pos[2][3] = {{-winSize.width/10, winSize.height * 0.34, 0}, {winSize.width/2, winSize.height * 0.3, 0}};
    GuiImage* baloon[2];
    for (int i = 0; i < 2; i++) {
        char temp[20] = {0};
        sprintf(temp, "baloon%d.png", i+1);
        baloon[i] = GuiImage::create(temp);
        baloon[i]->Animator = new AnimTravelOffset(60, winSize.width/2 + 50, baloon_Pos[i][1], -winSize.width/2-50, baloon_Pos[i][1], baloon_Pos[i][0], baloon_Pos[i][1], 0, 0.5f, 0.5f, 15, 20);
        m_bgLayer->addChild(baloon[i], baloon_Pos[i][2]);
        baloon[i]->setScale(g_fResScale);
        
    }

    book = CCLayer::create();
    book->setAnchorPoint(ccp(0.0f, 0.0f));
    book->setPosition(ccp(0, -230 * g_fResScale));
    book->setScale(g_fResScale);
    m_bgLayer->addChild(book);
    
    float fbookAnimTime = 0.5f;
    
    CCSprite* bookmiddle = CCSprite::create("book_middle.png");
    bookmiddle->setAnchorPoint(ccp(0.5f, 0));
    bookmiddle->setPosition(ccp(0, 0));
    book->addChild(bookmiddle);

    float x , y;
    for (int i = -1; i < 6; i++) {
        char temp[20] = {0};
        char temp1[20] = {0};

        if(i == -1){
            x = 50; y = 10;
        }else{
            x = 50-i*10; y = 30 + i * 2;
        }
        if(i == -1){
            sprintf(temp, "book_left.png");         sprintf(temp1, "book_right.png");
        }else if(i < 5){
            sprintf(temp, "pages%d.png", 5 - i);    sprintf(temp1, "pages%d_right.png", 5 - i);
        }else{
            sprintf(temp, "grass_left.png");        sprintf(temp1, "grass_right.png");
        }
        
        CCSprite* left = CCSprite::create(temp);
        left->setAnchorPoint(ccp(1, 0));
        left->setPosition(ccp(-x, y));
        left->setRotation(66);
        book->addChild(left);
        

        CCSprite* right = CCSprite::create(temp1);
        right->setAnchorPoint(ccp(0, 0));
        right->setPosition(ccp(x, y));
        right->setRotation(-66);
        book->addChild(right);
        
//        if (i == 5)
        {
            left->setScaleY(0.2f);
            right->setScaleY(0.2f);
        }

            left->runAction(CCSpawn::create(CCMoveTo::create(fbookAnimTime, ccp(5*g_fResScale, y)), CCRotateTo::create(fbookAnimTime, 0),CCScaleTo::create(fbookAnimTime, 1.0f), NULL));
            right->runAction(CCSpawn::create(CCMoveTo::create(fbookAnimTime, ccp(-5*g_fResScale, y)), CCRotateTo::create(fbookAnimTime, 0),CCScaleTo::create(fbookAnimTime, 1.0f), NULL));

    }

//    CCSprite* bookshadow = CCSprite::create("shadow.png");
//    bookshadow->setAnchorPoint(ccp(0.5, 0));
//    bookshadow->setPosition(ccp(0, -20));
//    bookshadow->setOpacity(0);
//    bookshadow->runAction(CCSequence::create(CCDelayTime::create(fbookAnimTime), CCFadeIn::create(0), NULL));
//    book->addChild(bookshadow);

    bookmiddle->runAction(CCSequence::create(CCDelayTime::create(fbookAnimTime), CCMoveTo::create(0, ccp(0, -10)), NULL));

    scheduleOnce(schedule_selector(BaseLayer::onOpenedBook), fbookAnimTime);
    
    float sparkle_pos[5][3] = {{-750, -260, 0.4}, {-350, -290, 0.4}, {150, -360, 0.5}, {250, -470, 0.7}, {650, -270, 0.3}};
    
    for (int i = 0; i < 5; i++) {
        GuiImage* sparkle = GuiImage::create("shine-gold.png");
        sparkle->Animator = new AnimSparkleRandom(0.5f, 1.5f, 4.0f);
        sparkle->setPosition(ccp(sparkle_pos[i][0] * g_fResScale, sparkle_pos[i][1] * g_fResScale));
        m_bgLayer->addChild(sparkle);
        sparkle->setScale(g_fResScale * sparkle_pos[i][2]);
    }
    

}
void BaseLayer::animClounds()
{
    float cloud_Pos[3][3] = {{winSize.width/10, winSize.height * 0.34, 0}, {winSize.width/2, winSize.height * 0.3, 0}, {-winSize.width/3, winSize.height * 0.25, 0}};
    GuiImage* cloud[3];
    for (int i = 0; i < 3; i++) {
        char temp[20] = {0};
        sprintf(temp, "claud%d.png", i+1);
        cloud[i] = GuiImage::create(temp);
        //        cloud[i]->setPosition(ccp(baloon_Pos[i][0], baloon_Pos[i][1]));
        cloud[i]->Animator = new AnimTravelOffset(100 + i * 10, winSize.width/2 + 50, cloud_Pos[i][1], -winSize.width/2-50, cloud_Pos[i][1], cloud_Pos[i][0], cloud_Pos[i][1], 0, 1.0f, 0.0f, 0, 0);
        cloud[i]->setScale(g_fResScale);
        m_bgLayer->addChild(cloud[i], cloud_Pos[i][2]);
        
    }
    

}
void BaseLayer::onOpenedBook()
{
    book->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(2, 1 * g_fResScale, 0.97f * g_fResScale),
                                                               CCScaleTo::create(2, 1 * g_fResScale, 1.0 * g_fResScale), NULL)));
    CCSprite *bridge = CCSprite::create("bridge.png");
    bridge->setPosition(ccp(0, 160));
    bridge->setScale(0);
    bridge->runAction(CCSequence::create(CCScaleTo::create(0.2, 1.2),
                                         CCScaleTo::create(0.1, 1.0),
                                         NULL));
    book->addChild(bridge);
    
    float Tree_Pos[8][3] = {{-440, 320, 3}, {-266, 300, 1}, {320, 290, 3}, {-368, 264, 2}, {386, 274, 2}, {166, 234, 1}, {-162, 210}, {262, 212}};
    float Tree_Anchor[8][2] = {{95, 4}, {81, 4}, {88, 3}, {66, 0}, {48, 2}, {43, 3}, {20, 0}, {36, 0}};
    CCSprite* tree[8];
    for (int i = 0; i < 8; i++) {
        char temp[20] = {0};
        sprintf(temp, "tree%d.png", i+1);
        tree[i] = CCSprite::create(temp);
        CCSize sz = tree[i]->getContentSize();
        tree[i]->setPosition(ccp(Tree_Pos[i][0] - (sz.width/2 - Tree_Anchor[i][0]), Tree_Pos[i][1] - (sz.height/2 - Tree_Anchor[i][1])));
        book->addChild(tree[i], Tree_Pos[i][2]);
        tree[i]->setScale(0);
        tree[i]->runAction(CCSequence::create(CCDelayTime::create(i * 0.2 + 0.2),
                                              CCScaleTo::create(0.2, 1.2),
                                              CCScaleTo::create(0.2, 1),
                                              NULL));
        
        tree[i]->setAnchorPoint(ccp(Tree_Anchor[i][0] / sz.width, Tree_Anchor[i][1] / sz.height));
        float da = 0;
        if (i == 5) {
            da = 12;
        }
        tree[i]->setRotation(-2-da);
        tree[i]->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(2 + nextFloat(), 4-da),
                                                                      CCRotateTo::create(2 + nextFloat(), -4-da),
                                                                      NULL)));
    }
    CCSprite* bookshadow = CCSprite::create("shadows.png");
    bookshadow->setAnchorPoint(ccp(0.5, 0));
    bookshadow->setPosition(ccp(-40, 160));
    bookshadow->setOpacity(0);
    bookshadow->runAction(CCSequence::create(CCDelayTime::create(1), CCFadeIn::create(0), NULL));
    book->addChild(bookshadow);
    
    CCSprite* waterfall = CCSprite::create("waterfall2.png");
    waterfall->setAnchorPoint(ccp(0.5, 1));
    waterfall->setPosition(ccp(0, 130));
    waterfall->setOpacity(0);
    waterfall->runAction(CCSequence::create(CCDelayTime::create(0.3), CCFadeIn::create(1.0f),
                                             NULL));
    waterfall->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.3, 1, 0.9),
                                                                    CCScaleTo::create(0.3, 1, 1.1), NULL)));
    book->addChild(waterfall);

    CCSprite* water = CCSprite::create("water.png");
//    water->setAnchorPoint(ccp(0.5, 0));
    water->setPosition(ccp(0, 50));
    water->setOpacity(0);
    water->runAction(CCSequence::create(CCDelayTime::create(0.3), CCFadeIn::create(1.0f), NULL));
    book->addChild(water);

    CCSprite* waterfall3 = CCSprite::create("waterfall3.png");
    //    waterfall3->setAnchorPoint(ccp(0.5, 0));
    waterfall3->setPosition(ccp(0, -25));
    waterfall3->setOpacity(0);
    waterfall3->runAction(CCSequence::create(CCDelayTime::create(0.3), CCFadeIn::create(1.0f), NULL));
    waterfall3->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.8, 1),
                                                                    CCScaleTo::create(0.8, 1.2), NULL)));
    book->addChild(waterfall3);

}
void BaseLayer::bg_18()
{
    CCSize sz;
    
//    CCSprite *sea = CCSprite::create("bg18_2.png");
//    Config::setScale(sea);
//    sea->setPosition(ccp(0, 0));
//    m_bgLayer->addChild(sea);
    m_river = pgeRippleSprite::create("bg18_2.png");
    m_river->setCenter(ccp(960, 600));
    Config::setScale(m_river);
    m_river->setPosition(ccp(-winSize.width/2, -winSize.height/2));
    m_bgLayer->addChild(m_river);

    CCSprite *background = CCSprite::create("bg18_1.png");
    Config::setScale(background);
    sz = background->getContentSize();
    sz.width *= Config::getxScale();
    sz.height *= Config::getyScale();
    background->setPosition(ccp(0, 0));
    m_bgLayer->addChild(background);

    animClounds();
    
    
    float sx = Config::getxScale(), sy = Config::getyScale();


    float fBolbbyTime = 3;
    CCLayer* islLayer = CCLayer::create();
    m_bgLayer->addChild(islLayer);
    islLayer->setAnchorPoint(ccp(0.5, 0.5));
    islLayer->setPosition(ccp(0, -50 * sy));
  
    
    /*

    CCSprite *islShadow = CCSprite::create("isl_shadow.png");
    Config::setScale(islShadow);
    islShadow->setPosition(ccp(0, -340 * sy));
    islLayer->addChild(islShadow);
    islShadow->setOpacity(0);
    islShadow->runAction(CCFadeIn::create(fBolbbyTime));
    

    float fingerL_Pos[4][2] = {{-516, 112}, {-560, 112}, {-630, 134}, {-660, 120}};
    float fingerL_Anchor[4][2] = {{30/44.0, 5/123.0}, {56/68.0, 5/161.0}, {36/59.0, 6/169.0}, {66/95.0, 5/238.0}};
    float fingerR_Pos[4][2] = {{516, 112}, {590, 128}, {640, 134}, {660, 120}};
    float fingerR_Anchor[4][2] = {{12/51.0, 5/124.0}, {13/55.0, 4/135.0}, {17/59.0, 4/168.0}, {24/99.0, 15/236.0}};
    float fingerTime = 1;
    for (int i = 0; i < 4; i++) {
        char temp[20] = {0};
        sprintf(temp, "left_h_fing%d.png", i+1);
        CCSprite *finger1 = CCSprite::create(temp);
        Config::setScale(finger1);
        finger1->setPosition(ccp(fingerL_Pos[i][0] * sx, fingerL_Pos[i][1] * sy));
        islLayer->addChild(finger1);
        finger1->setOpacity(0);
        finger1->runAction(CCFadeIn::create(fBolbbyTime));
        finger1->setAnchorPoint(ccp(fingerL_Anchor[i][0], fingerL_Anchor[i][1]));
        finger1->runAction(CCRepeatForever::create(CCSequence::create(
                                                   CCRotateTo::create(fingerTime, 2),
                                                   CCRotateTo::create(fingerTime * 2, -4),
                                                   NULL)));
        char temp1[20] = {0};
        sprintf(temp1, "right_h_fing%d.png", i+1);
        CCSprite *fingerR = CCSprite::create(temp1);
        Config::setScale(fingerR);
        fingerR->setPosition(ccp(fingerR_Pos[i][0] * sx, fingerR_Pos[i][1] * sy));
        islLayer->addChild(fingerR);
        fingerR->setOpacity(0);
        fingerR->runAction(CCFadeIn::create(fBolbbyTime));
        fingerR->setAnchorPoint(ccp(fingerR_Anchor[i][0], fingerR_Anchor[i][1]));
        fingerR->runAction(CCRepeatForever::create(CCSequence::create(
                                                                      CCRotateTo::create(fingerTime, -2),
                                                                      CCRotateTo::create(fingerTime * 2, 4),
                                                                      NULL)));

    }
    CCSprite *isl = CCSprite::create("isl.png");
    Config::setScale(isl);
    isl->setPosition(ccp(0, 0));
    islLayer->addChild(isl);
    isl->setOpacity(0);
    isl->runAction(CCFadeIn::create(fBolbbyTime));
 */

    float fingerTime = 1;


    CCSprite *islReflect = CCSprite::create("reflect.png");
    Config::setScale(islReflect);
    islReflect->setPosition(ccp(0, -720 * sy));
    islLayer->addChild(islReflect);
    islReflect->setOpacity(0);
    islReflect->runAction(CCFadeIn::create(fBolbbyTime));

    CCSprite *boShadow = CCSprite::create("boat_shadow.png");
    Config::setScale(boShadow);
    boShadow->setPosition(ccp(530 * sx, -384 * sy));
    islLayer->addChild(boShadow);
    boShadow->setOpacity(0);
    boShadow->runAction(CCFadeIn::create(fBolbbyTime));
    boShadow->runAction(CCRepeatForever::create(
                                          CCSequence::create(
                                                             CCRotateBy::create(fingerTime, 2),
                                                             CCRotateBy::create(fingerTime, -2),
                                                             NULL)));
    ClipNode *clip1=new ClipNode();
    clip1->setClipsToBounds(true);
    clip1->setClippingRegion(CCRect(0, 0,  winSize.width, 100 * sy)); //in Points
    islLayer->addChild(clip1);
    
    CCSprite *bo1 = CCSprite::create("bo2.png");
    //    Config::setScale(bo1);
    if (sx < sy)
        bo1->setScale(sx);
    else
        bo1->setScale(sy);
    bo1->setPosition(ccp(600 * sx, -468 * sy));
//    bo1->setRotation(20 + 140);
    clip1->addChild(bo1);
    bo1->setOpacity(0);
    bo1->runAction(CCFadeIn::create(fBolbbyTime));
    bo1->runAction(CCRepeatForever::create(
                                           CCSequence::create(
                                                              CCRotateBy::create(fingerTime, -3),
                                                              CCRotateBy::create(fingerTime, 3),
                                                              NULL)));

    ClipNode *clip=new ClipNode();
    clip->setClipsToBounds(true);
    clip->setClippingRegion(CCRect(0, 204 * sy,  winSize.width, winSize.height)); //in Points
    islLayer->addChild(clip);


    CCSprite *bo = CCSprite::create("bo.png");
//    Config::setScale(bo);
    if (sx < sy)
        bo->setScale(sx);
    else
        bo->setScale(sy);
    bo->setPosition(ccp(600 * sx, -310 * sy));
//    bo->setRotation(20);
    clip->addChild(bo);
    bo->setOpacity(0);
    bo->runAction(CCFadeIn::create(fBolbbyTime));
    bo->runAction(CCRepeatForever::create(
                                                  CCSequence::create(
                                                                     CCRotateBy::create(fingerTime, 3),
                                                                     CCRotateBy::create(fingerTime, -3),
                                                                     NULL)));


    CCSprite *boatShadow = CCSprite::create("boat_shadow.png");
    Config::setScale(boatShadow);
    boatShadow->setPosition(ccp(-260 * sx, -394 * sy));
    islLayer->addChild(boatShadow);
    boatShadow->setOpacity(0);
    boatShadow->runAction(CCFadeIn::create(fBolbbyTime));
    boatShadow->runAction(CCRepeatForever::create(
                                            CCSequence::create(
                                                               CCRotateTo::create(fingerTime, -2),
                                                               CCRotateTo::create(fingerTime * 2, -4),
                                                               NULL)));
//    boatShadow->runAction(CCRepeatForever::create(CCMoveBy::create(fingerTime, ccp(-3*sx, -2*sy))));

    CCSprite *boat = CCSprite::create("boat.png");
    Config::setScale(boat);
    boat->setPosition(ccp(-200 * sx, -340 * sy));
    islLayer->addChild(boat);
    boat->setOpacity(0);
    boat->runAction(CCFadeIn::create(fBolbbyTime));
    boat->runAction(CCRepeatForever::create(
                                            CCSequence::create(
                                                               CCRotateTo::create(fingerTime, -2),
                                                               CCRotateTo::create(fingerTime * 2, -4),
                                                               NULL)));
//    boat->runAction(CCRepeatForever::create(CCMoveBy::create(fingerTime, ccp(-3*sx, -2*sy))));

}
void BaseLayer::update(float delta)
{
    if (m_bgLayer) {
        m_bgLayer->update(delta);
    }
    if (m_river) {
        m_river->update(delta);
    }
}

void BaseLayer::onMenuBtn(CCObject* pSender)
{
//    MyMenuItem* menuItem = (MyMenuItem*)pSender;
    m_bMenuShow = !m_bMenuShow;
    if (m_bMenuShow) {
        
        m_menu->runAction(CCSequence::create( CCScaleTo::create(0.2, 1.1),
                                             CCScaleTo::create(0.15, 0.95),
                                             CCScaleTo::create(0.1, 1),
                                             NULL));
        m_spArc->runAction(MenuAction::cornerMenuAction());
        menuItem->initWithNormalImage("but3a.png", "but3a.png", "but3a.png", this, menu_selector(BaseLayer::onMenuBtn));
    }
    else{
        m_menu->runAction(CCScaleTo::create(0.2, 0));
        m_spArc->runAction(CCScaleTo::create(0.2, 0));
        menuItem->initWithNormalImage("but4a.png", "but4a.png", "but4a.png", this, menu_selector(BaseLayer::onMenuBtn));
    }
}
void BaseLayer::onClickMenuBtn(){
    onMenuBtn(menuItem);
}

void BaseLayer::menuCallback(CCObject* pSender)
{
    int nTag = ((MyMenuItem*)pSender)->getTag();
    if (nTag == MENU_SOUND_TAG) {
        if (CCUserDefault::sharedUserDefault()->getBoolForKey(SOUNDONOFF))
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey(SOUNDONOFF, false);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey(SOUNDONOFF, true);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
        }
    }
    else if (nTag == MENU_EFFECT_TAG){
        
    }
    else if (nTag == MENU_HELP_TAG){
        
    }
    else if (nTag == MENU_RETURN_TAG){
        
    }
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    
    //#endif
}

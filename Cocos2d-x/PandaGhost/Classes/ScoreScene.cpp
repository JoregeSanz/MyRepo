//
//  ScoreScene.cpp
//  PacMan
//
//  Created by jyn on 2/3/14.
//
//

#include "ScoreScene.h"
#include "GameScene.h"

CCScene* ScoreScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ScoreScene *layer = ScoreScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

ScoreScene::~ScoreScene()
{
    removeAllChildrenWithCleanup(true);
}

bool ScoreScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    Config::setScaleFactor(winSize);
    
    
    CCLabelTTF *highScoreLabel = CCLabelTTF::create("High Scores", "Marker Felt.ttf", 70);
    Config::setScale(highScoreLabel);
    highScoreLabel->setPosition( ccp(winSize.width / 2, winSize.height *0.85));
    this->addChild(highScoreLabel);
    
    CCMenuItemImage *backbuttonitem = CCMenuItemImage::create("backbtn.png", "backbtn.png", "backbtn.png",this, menu_selector(ScoreScene::onBack));
    Config::setScale(backbuttonitem);

    backbutton = CCMenu::create(backbuttonitem, NULL);
    backbutton->setPosition(winSize.width*0.1,winSize.height*0.9);
    addChild(backbutton);
    
    backbuttonitem->setPosition(backbuttonitem->getPosition().x, backbuttonitem->getPosition().y+50);
    CCAction* dropTitle = CCMoveBy::create(3, CCPoint(0, - 50));
    CCAction* easeDrop = CCEaseBounceOut::create((CCActionInterval*)dropTitle);
    backbuttonitem->runAction(easeDrop);
    
    CCSprite *pea = CCSprite::create("skel.png");
    pea->setPosition(ccp(winSize.width / 2, winSize.height *0.15));
    Config::setScale(pea);
    this->addChild(pea, zOrderPel);

    showHighScores();
    return true;
}
void ScoreScene::onBack()
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
void ScoreScene::showHighScores()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B lbscorecolor = {255,255,255};
    ccColor3B lbbestcolor = { 255, 10, 10 };
    CCLabelTTF* scoresNo[HIGHSCORE_NUM];
    
    for(int i = 0; i < HIGHSCORE_NUM; i++)
    {
        char temp1[20] = {0};
        sprintf(temp1, "%dth", i+1);
        
        
        scoresNo[i] = CCLabelTTF::create(temp1, "Marker Felt.ttf", 50);
        if ( i < 3 ){
            scoresNo[i]->setColor(lbbestcolor);
        }
        else
            scoresNo[i]->setColor(lbscorecolor);
        
        scoresNo[i]->setHorizontalAlignment(kCCTextAlignmentLeft);
        Config::setScale(scoresNo[i]);
        scoresNo[i]->setPosition(CCPoint(winSize.width * 0.25, winSize.height*0.7 - scoresNo[i]->boundingBox().size.height*i *1.1));
        
        addChild(scoresNo[i]);
        
        char temp[20];
        sprintf(temp, "high%dst",i);
        sprintf(temp, "%d", CCUserDefault::sharedUserDefault()->getIntegerForKey(temp));
        scores[i] = CCLabelTTF::create(temp, "Marker Felt.ttf", 50);
        if ( i < 3 ) scores[i]->setColor(lbbestcolor);
        else scores[i]->setColor(lbscorecolor);
        Config::setScale(scores[i]);
        scores[i]->setPosition(CCPoint(winSize.width * 0.75, winSize.height*0.7 - scores[i]->boundingBox().size.height*i *1.1));
        
        addChild(scores[i]);
    }
}

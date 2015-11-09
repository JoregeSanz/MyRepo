//
//  ScoreScene.h
//  PacMan
//
//  Created by jyn on 2/3/14.
//
//

#ifndef __PacMan__ScoreScene__
#define __PacMan__ScoreScene__

#include "cocos2d.h"
#include "Config.h"
USING_NS_CC;


class ScoreScene : public CCLayer
{
    public :
    virtual bool init();
    static CCScene* scene();

    CREATE_FUNC(ScoreScene);
    
    
    CCLabelTTF *scores[HIGHSCORE_NUM];
    CCMenu	 *backbutton;
    
    ~ScoreScene();
    
    void    onBack();
    void showHighScores();
};

#endif /* defined(__PacMan__ScoreScene__) */

//
//  Obstacle.h
//  TingTong
//
//  Created by admin on 6/3/14.
//
//

#ifndef __TingTong__Obstacle__
#define __TingTong__Obstacle__

#include <iostream>

#include "CarGame.h"

class Obstacle : public CCSprite
{
public:
    int m_nRoadIndex;//0~3
    int m_nType;//0 barrel
    float time;
    CarGame* m_gamescene;
    
    static Obstacle* create(int Type, int road,int roadst,CCArray* wordarray, int curtarget);
    void crushAnimation();
    void removeObstacle();
    void update();
    void AnimateTrue();
    void AnimateFalse();
    void RemoveLabel();
};

#endif /* defined(__TingTong__Obstacle__) */

//
//  MovingCardGroup.h
//  RealSolitaire
//
//  Created by LightSky on 2/11/15.
//
//

#ifndef __RealSolitaire__MovingCardGroup__
#define __RealSolitaire__MovingCardGroup__

#include "CardGroup.h"

enum RIGHT_DIRECTION
{
    NONE,
    LEFT,
    RIGHT
};

class MovingCardGroup : public CardGroup
{
private:
    float m_fFaceDeltaY;
    RIGHT_DIRECTION m_direction;
    
public:
    MovingCardGroup();
    
    /**
     *  add card.
     **/
    void setCard(int nCardNumber);
    
    /**
     *  get card position in screen coordinate
     **/
    Vec2 getCardPosition(int index);
    
    /**
     *  return rect in screen coordinate
     **/
    Rect getRect();
    
    /**
     *  when cards move to right of original position, cards rotate to right, left, .....
     **/
    void setRightRotate();
    
    /**
     *  when cards move to left of original position, cards rotate ro left, right, .....
     **/
    void setLeftRotate();
    
    /**
     *  set lotation state to NONE
     **/
    void clearRotation();
};

#endif /* defined(__RealSolitaire__MovingCardGroup__) */

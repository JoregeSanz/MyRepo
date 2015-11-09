//
//  MovingCardGroup.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/11/15.
//
//

#include "MovingCardGroup.h"

MovingCardGroup::MovingCardGroup()
{
    m_fFaceDeltaY = Utils::GetFaceCardDistance();
}

void MovingCardGroup::setCard(int nCardNumber)
{
    float dy = 0;
    if( getCardCount() > 0 )
    {
        Card * lastCard = getLastCard();
        dy = m_spBase->getContentSize().height / 2.0 - lastCard->getPositionY();
        
        dy += m_fFaceDeltaY;
    }
    
    Card * card = Card::create();
    card->initWithNoWithoutBg(nCardNumber);
    card->setPosition(Vec2(m_spBase->getContentSize().width / 2, m_spBase->getContentSize().height / 2 - dy ));
    m_spBase->addChild(card);
    
    m_vCards.pushBack(card);
}

Vec2 MovingCardGroup::getCardPosition(int index)
{
    Vec2 pos = m_spBase->getPosition();
    if( getCardCount() > index )
    {
        Card * card = m_vCards.at(index);
        float dy = m_spBase->getContentSize().height / 2.0 - card->getPositionY();
        pos.y -= dy;
    }
    
    return pos;
}

Rect MovingCardGroup::getRect()
{
    float dh = 0;
    if( getCardCount() > 0 )
    {
        Card * lastCard = getLastCard();
        dh = m_spBase->getContentSize().height / 2.0 - lastCard->getPositionY();
    }
    
    Rect rc = m_spBase->getBoundingBox();
    rc.origin.y -= dh;
    rc.size.height += dh;
    
    return rc;
}

#pragma mark - rotation anim

void MovingCardGroup::setRightRotate()
{
    if( m_direction != RIGHT )
    {
        int cnt = getCardCount();
        if( cnt > 0 )
            m_direction = RIGHT;
        
        for( int i = 0; i < cnt; i ++ )
        {
            if( i % 2 == 0 )
                m_vCards.at(i)->onRotateRight();
            else
                m_vCards.at(i)->onRotateLeft();
        }
    }
}

void MovingCardGroup::setLeftRotate()
{
    if( m_direction != LEFT )
    {
        int cnt = getCardCount();
        if( cnt > 0 )
            m_direction = LEFT;
        
        for( int i = 0; i < cnt; i ++ )
        {
            if( i % 2 == 0 )
                m_vCards.at(i)->onRotateLeft();
            else
                m_vCards.at(i)->onRotateRight();
        }
    }
}

void MovingCardGroup::clearRotation()
{
    m_direction = NONE;
}

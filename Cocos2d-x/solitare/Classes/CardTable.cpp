 //
//  CardTable.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/10/15.
//
//

#include "CardTable.h"

#define kCARDTABLE          "sCardTable"

CardTable::CardTable()
{
    m_fDeltaY = 18;		//20
    m_fFaceDeltaY = Utils::GetFaceCardDistance();
}

void CardTable::setCard(int nCardNumber)
{
    float dy = 0;
    if( getCardCount() > 0 )
    {
        Card * lastCard = getLastCard();
        dy = m_spBase->getContentSize().height / 2.0 - lastCard->getPositionY();
        
        if( lastCard->isFace() )
            dy += m_fFaceDeltaY;
        else
            dy += m_fDeltaY;
    }
    
    Card * card = Card::create();
    card->initWithNo(nCardNumber);
    card->setPosition(Vec2(m_spBase->getContentSize().width / 2, m_spBase->getContentSize().height / 2 - dy ));
    m_spBase->addChild(card);
    
    m_vCards.pushBack(card);
}

void CardTable::removeSelectedCards(int st)
{
    int cnt = getCardCount();
    for( int i = cnt -1; i >= st; i -- )
    {
        Card* card = m_vCards.at(i);
        card->removeFromParent();
        
        m_vCards.erase(i);
    }
}

bool CardTable::canMerge(int nCardNumber)
{
    bool res = false;
    
    int cnt = getCardCount();
    if( cnt == 0 )
    {
        if( nCardNumber != 0 && nCardNumber >= 12 && (nCardNumber - 12) % 13 == 0 )     // "K"
            res = true;
    }
    else
    {
        if( getLastCard()->isFace() )
        {
            int nLastCardNumber = getLastCard()->getCardNumber();
            int nLastKindNumber = Card::SGetCardKindNumber(nLastCardNumber);
            int nLastLevelNumber = Card::SGetCardLevelNumber(nLastCardNumber);
            
            int nKindNumber = Card::SGetCardKindNumber(nCardNumber);
            int nLevelNumber = Card::SGetCardLevelNumber(nCardNumber);
            
            if( nLevelNumber == nLastLevelNumber - 1 )
            {
                if( nKindNumber <= 2 && nLastKindNumber >= 3 )
                    res = true;
                else if( nKindNumber >= 3 && nLastKindNumber <= 2 )
                    res = true;
            }
        }
    }
    
    return res;
}

#pragma mark - coordinate

Vec2 CardTable::getLastCardPosition()
{
    Vec2 pos = m_spBase->getPosition();
    if( getCardCount() > 0 )
    {
        Card * lastCard = getLastCard();
        float dy = m_spBase->getContentSize().height / 2.0 - lastCard->getPositionY();
        
        if( lastCard->isFace() )
            dy += m_fFaceDeltaY;
        else
            dy += m_fDeltaY;
        
        pos.y -= dy;
    }
    
    return pos;
}

Vec2 CardTable::getCardPosition(int index)
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

Rect CardTable::getRect()
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

Rect CardTable::getHintRect(int index)
{
    if( index == -1 )   //empty card table
    {
        Rect rc = getBaseSprite()->getBoundingBox();
        rc.origin.x += getBaseSprite()->getContentSize().width / 2.0;
        rc.origin.y += getBaseSprite()->getContentSize().height / 2.0;
        return rc;
    }
    
	Vec2 posIndex = getCardPosition(index);
	Vec2 posLast = getCardPosition(getCardCount()-1);

	Vec2 pos = (posIndex + posLast);
	pos.x /= 2.0;
	pos.y /= 2.0;

	Size size(getBaseSprite()->getContentSize());

	size.height = size.height + posIndex.y - posLast.y;

	Rect rc(pos.x, pos.y, size.width, size.height);

	return rc;
}

int CardTable::getSelectedCardNo(const Vec2& point)
{
    Vec2 pt = m_spBase->convertToNodeSpace(point);
    int no = -1;
    
    int cnt = getCardCount();
    for( int i = cnt -1; i >= 0; i -- )
    {
        Card * card = m_vCards.at(i);
        if( card->isFace() && card->getBoundingBox().containsPoint(pt) )
        {
            no = i;
            break;
        }
    }
    
    return no;
}

#pragma mark - card action

void CardTable::onFlipTopCard()
{
    if( m_vCards.size() > 0 )
    {
        Card* topCard = m_vCards.at(m_vCards.size()-1);
        topCard->onFlip();
    }
}

void CardTable::runFlipAndShimmerAction(float stTime)
{
    if( m_vCards.size() > 0 )
    {
        Card* topCard = m_vCards.at(m_vCards.size()-1);
        topCard->runFlipAndShimmerAction(stTime);
    }
}

void CardTable::beginAllFaceShimmerAction()
{
    //init shimmer area
    int cnt = getCardCount();
    if( cnt == 0 )
        return;
    if( getLastCard()->isFace() == false )  //Now moving
        return;
    if( getLastCard()->getRemark() == REMARK_FLIPING )
    	return;
    
    float dh = 2;
    float w = m_spBase->getContentSize().width;
    float h = m_spBase->getContentSize().height - dh * 2;
    float h0 = h;
    
    int st = cnt - 2;
    if( getLastCard()->isVisible() == false )   //hint moving
        st = cnt - 3;
    for( int i = st; i >= 0; i -- )
        if( m_vCards.at(i)->isFace() )
            h += m_fFaceDeltaY;
        else
            break;
    
    float y = getLastCardPosition().y - m_spBase->getPosition().y + m_fFaceDeltaY;
    
    ClippingRectangleNode* shimmerClipNode = ClippingRectangleNode::create();
    shimmerClipNode->setPosition(0, y + dh);
    shimmerClipNode->setClippingRegion(Rect(0, 0, w, h));
    m_spBase->addChild(shimmerClipNode);
    
    Sprite* spShimmer = Sprite::create("placeBg/Card-Shimmer.png");
    spShimmer->setScale(w / spShimmer->getContentSize().width * SHIMMER_SCALE, h0 / spShimmer->getContentSize().height * SHIMMER_SCALE);
    spShimmer->setPosition(w/2.0, -h0/2.0);
    shimmerClipNode->addChild(spShimmer);
    
    //animation
    auto moveAction = MoveBy::create(SHIMMER_MOVE_DURATION / (2*h0) * (h0 + h), Vec2(0, h0 + h));
    CallFunc * callback = CallFunc::create(CC_CALLBACK_0(CardTable::onFinishedShimmerEffect, this, shimmerClipNode));
    Sequence * seq = Sequence::create(moveAction, callback, NULL);
    
    spShimmer->runAction(seq);
}

void CardTable::runAllFaceShimmerAction(float delay)
{
    DelayTime * delayAction = DelayTime::create(delay);
    CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(CardTable::beginAllFaceShimmerAction, this));
    Sequence * seq = Sequence::create(delayAction, callFunc, NULL);
    m_spBase->runAction(seq);
}

void CardTable::onFinishedShimmerEffect(Node* sender)
{
    sender->removeFromParent();
}

void CardTable::runMaskFadeEffect()
{
    int cnt = getCardCount();
    for( int i = 0; i < cnt; i ++ )
    {
        Card* card = m_vCards.at(i);
        card->showCardMaskFade();
    }
}

#pragma mark - save and restore state

std::string CardTable::getSaveKey()
{
    std::string res(kCARDTABLE);
    res = res + "_" + std::to_string(m_nNo) + "_";
    return res;
}

void CardTable::saveState(int nMove)
{
    CardGroup::saveState(nMove, getSaveKey());
}

void CardTable::restoreState(int nMove)
{
    std::vector<std::string> vCardNo = CardGroup::getState(nMove, getSaveKey());
    
    reset();
    int cnt = vCardNo.size();
    for( int i = 0; i < cnt; i ++ )
    {
        std::string strNo = vCardNo.at(i);
        int nCarNumber = std::stoi(strNo);
        setCard(nCarNumber);
        
        i ++;
        std::string strIsFace = vCardNo.at(i);
        int nIsFace = std::stoi(strIsFace);
        
        if( nIsFace == 1 || i == cnt - 1 )
            showLastCardFace();
    }
}


bool CardTable::isComplete()
{
	bool res = true;
	int cnt = getCardCount();
	for( int i = 0; i < cnt; i ++ )
		if( m_vCards.at(i)->isFace() == false )
		{
			res = false;
			break;
		}

	return res;
}

int CardTable::getFirstFaceIndex()
{
	int res = -1;
	int cnt = getCardCount();
	for( int i = 0; i < cnt; i ++ )
	{
		Card * card = m_vCards.at(i);
		if( card->isFace() )
		{
			res = i;
			break;
		}
	}

	return res;
}

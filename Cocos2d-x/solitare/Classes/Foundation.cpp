//
//  Foundation.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/11/15.
//
//

#include "Foundation.h"

#define kFOUNDATION         "sFoundation"

bool Foundation::canMerge(int nCardNumber)
{
    bool res = false;
    
    int cnt = getCardCount();
    if( cnt == 0 )
    {
        if( nCardNumber == 0 || nCardNumber % 13 == 0 )     // "A"
            res = true;
    }
    else
    {
        int nLastCardNumber = getLastCard()->getCardNumber();
        int nLastKindNumber = Card::SGetCardKindNumber(nLastCardNumber);
        int nLastLevelNumber = Card::SGetCardLevelNumber(nLastCardNumber);
        
        int nKindNumber = Card::SGetCardKindNumber(nCardNumber);
        int nLevelNumber = Card::SGetCardLevelNumber(nCardNumber);
        
        if( nLevelNumber == nLastLevelNumber + 1 )
        {
            if( nKindNumber == nLastKindNumber )
                res = true;
        }
    }
    
    return res;
}

#pragma mark - card action

void Foundation::onFlipTopCard()
{
    if( getCardCount() > 0 )
    {
        Card* topCard = getLastCard();
        topCard->onFlip();
    }
}

void Foundation::setShadowMask()
{
    m_spMask = Sprite::create("placeBg/Card-Mask.png");
    m_spMask->setOpacity(REDEAL_MASK_OPACITY);
    Size size = getBaseSprite()->getContentSize();
    m_spMask->setScale(size.width / m_spMask->getContentSize().width * MASK_SCALE, size.height / m_spMask->getContentSize().height * MASK_SCALE);
    m_spMask->setPosition(size.width / 2.0, size.height / 2.0);
    
    getBaseSprite()->addChild(m_spMask);
}

void Foundation::showShadowMask()
{
    if( m_spMask->isVisible() == false )
        m_spMask->setVisible(true);
}

void Foundation::hideShadowMask()
{
    m_spMask->setVisible(false);
}

#pragma mark - save and restore state

std::string Foundation::getSaveKey()
{
    std::string res(kFOUNDATION);
    res = res + "_" + std::to_string(m_nNo) + "_";
    return res;
}

void Foundation::saveState(int nMove)
{
    CardGroup::saveState(nMove, getSaveKey());
}

void Foundation::restoreState(int nMove)
{
    std::vector<std::string> vCardNo = CardGroup::getState(nMove, getSaveKey());
    
    reset();
    hideShadowMask();
    
    int cnt = vCardNo.size();
    if( cnt == 0 )
    	return;

    for( int i = 0; i < cnt; i += 2 )
    {
        std::string strNo = vCardNo.at(i);
        int nCarNumber = std::stoi(strNo);
        setCard(nCarNumber);
        showLastCardFace();
        getLastCard()->hideCardMask();
    }

    showShadowMask();
}

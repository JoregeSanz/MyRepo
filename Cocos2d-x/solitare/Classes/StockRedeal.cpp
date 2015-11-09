//
//  StockRedeal.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/10/15.
//
//

#include "StockRedeal.h"

#define kSTOCKREDEAL                "sStockRedeal"

void StockRedeal::onRearrangeCards()
{
    int cnt = getCardCount();
    if( cnt <= 1 )
        return;

    std::vector<int> vCards;
    for( int i = 0; i < cnt; i ++ )
    {
        int nCardNumber = getCardNumberFromIndex(i);
        vCards.push_back(nCardNumber);
    }
    
    std::random_shuffle(vCards.begin(), vCards.end());
    reset();
    
    for( int i = 0; i < cnt; i ++ )
    {
        setCard(vCards.at(i));
        getLastCard()->hideCardMask();
    }
}

void StockRedeal::setShadowMask()
{
    m_spMask = Sprite::create("placeBg/Card-Mask.png");
    m_spMask->setOpacity(REDEAL_MASK_OPACITY);
    Size size = getBaseSprite()->getContentSize();
    m_spMask->setScale(size.width / m_spMask->getContentSize().width * MASK_SCALE, size.height / m_spMask->getContentSize().height * MASK_SCALE);
    m_spMask->setPosition(size.width / 2.0, size.height / 2.0);
    
    getBaseSprite()->addChild(m_spMask);
}

void StockRedeal::showShadowMask()
{
    if( m_spMask->isVisible() == false )
        m_spMask->setVisible(true);
}

void StockRedeal::hideShadowMask()
{
    m_spMask->setVisible(false);
}

void StockRedeal::runMaskFadeEffect()
{
    m_spMask->setOpacity(0);
    m_spMask->setVisible(true);
    
    DelayTime* delayAction = DelayTime::create(FADE_DELAY / 2.0);
    auto fadeAction = FadeIn::create(FADE_DURATION);
    Sequence* seq = Sequence::create(delayAction, fadeAction, NULL);
    m_spMask->runAction(seq);
}

#pragma mark - save and restore state

void StockRedeal::saveState(int nMove)
{
    CardGroup::saveState(nMove, kSTOCKREDEAL);
}

void StockRedeal::restoreState(int nMove)
{
    std::vector<std::string> vCardNo = CardGroup::getState(nMove, kSTOCKREDEAL);
    
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
        getLastCard()->hideCardMask();
    }
    
    showShadowMask();
}

void StockRedeal::runHintGlowEffect()
{
    //glow sprite
    float w = m_spBase->getContentSize().width;
    float h = m_spBase->getContentSize().height;

    Sprite* spGlow = Sprite::create("placeBg/glow.png");
    spGlow->setScale(w / spGlow->getContentSize().width * HINT_SCALE, h / spGlow->getContentSize().height * HINT_SCALE);
    spGlow->setPosition(w/2.0, h/2.0);
    spGlow->setLocalZOrder(ZORDER_HINT);

    m_spBase->addChild(spGlow);

    //glow animation
    spGlow->setOpacity(0);
    FadeIn* fade1 = FadeIn::create(HINT_DURATION);
    FadeOut* fade2 = FadeOut::create(HINT_DURATION);
    CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, spGlow));
    Sequence* seq = Sequence::create(fade1, fade2, fade1, fade2, callback, NULL);
    spGlow->runAction(seq);
}

void StockRedeal::setNoRedeal()
{
	getBaseSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("placeBg/StockBk-NoRedeal.png"));
}

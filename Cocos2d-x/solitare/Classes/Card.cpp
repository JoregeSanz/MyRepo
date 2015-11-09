
//
//  Card.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/9/15.
//
//

#include "Card.h"

#define ROTATE_ANGLE            3.0
#define SHAKE_ROTATE_ANGLE		5
#define SHAKE_ROTATE_DURATION	0.1

#define ACTION_TAG_SHAKE		1001

#pragma mark - initialize

Card::Card()
{
    m_nRemark = REMARK_DEFAULT;
}

Card* Card::create()
{
    Card * pCard = new (std::nothrow) Card();
    if( pCard && pCard->initWithFile("placeBg/StockBk-Temp.png") )
    {
        pCard->autorelease();
        
        return pCard;
    }
    
    CC_SAFE_DELETE(pCard);
    
    return NULL;
}

void Card::initWithNo(int nCardNo)
{
    setContentSize(Utils::share()->getCardSize());
    initCardMask();
    
    initCardBackground();
    
    m_nNo = nCardNo;
    setCardKindAndNumber(nCardNo);
    
    initCardFace();
}

void Card::initWithNoWithoutBg(int nCardNo)
{
    setContentSize(Utils::share()->getCardSize());
    initCardMask();

    m_nNo = nCardNo;
    setCardKindAndNumber(nCardNo);

    initCardFace();
}

void Card::initCardMask()
{
    m_spMask = Sprite::create("placeBg/Card-Mask.png");
    m_spMask->setOpacity(MASK_OPACITY);
    m_spMask->setScale(getContentSize().width / m_spMask->getContentSize().width * MASK_SCALE, getContentSize().height / m_spMask->getContentSize().height * MASK_SCALE);
    m_spMask->setPosition(getContentSize().width / 2.0, getContentSize().height / 2.0);

    addChild(m_spMask);
}

void Card::initCardBackground()
{
    m_spBg = Sprite::create("placeBg/StockBk-Temp.png");
    m_spBg->setTexture(Director::getInstance()->getTextureCache()->addImage(getBgPath()));
    m_spBg->setPosition(getContentSize().width / 2.0, getContentSize().height / 2.0);
    addChild(m_spBg);
    
    //    //set scale
    //    Utils::share()->calcCurrentScale(getContentSize());
}

void Card::initCardFace()
{
    m_spFace = Sprite::create(getFacePath());
    m_spFace->setScale(getContentSize().width / m_spFace->getContentSize().width, getContentSize().height / m_spFace->getContentSize().height);
    m_spFace->setPosition(getContentSize().width / 2.0, getContentSize().height / 2.0);

    m_spFace->setVisible(false);
    addChild(m_spFace);
}

void Card::initShimmerEffect()
{
	float dh = 2;
    float w = getContentSize().width;
    float h = getContentSize().height - dh * 2;
    
    m_shimmerClipNode = ClippingRectangleNode::create();
    m_shimmerClipNode->setClippingRegion(Rect(0, 0, w, h));
    m_shimmerClipNode->setPosition(0, dh);
    addChild(m_shimmerClipNode);
    
    m_spShimmer = Sprite::create("placeBg/Card-Shimmer.png");
    m_spShimmer->setScale(getContentSize().width / m_spShimmer->getContentSize().width * SHIMMER_SCALE, getContentSize().height / m_spShimmer->getContentSize().height * SHIMMER_SCALE);
    m_spShimmer->setPosition(w/2.0, -h/2.0);
    m_shimmerClipNode->addChild(m_spShimmer);
}

void Card::runShimmerAction()
{
    initShimmerEffect();
    
    auto moveAction = MoveBy::create(SHIMMER_MOVE_DURATION, Vec2(0, getContentSize().height * 2));
    CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::onFinishedShimmerEffect, this));
    Sequence * seq = Sequence::create(moveAction, callback, NULL);
    
    m_spShimmer->runAction(seq);
}

void Card::runShimmerActionDelay(float delay)
{
    DelayTime * delayAction = DelayTime::create(delay);
    CallFunc * callShimmer = CallFunc::create(CC_CALLBACK_0(Card::runShimmerAction, this));
    Sequence * seq = Sequence::create(delayAction, callShimmer, NULL);
    runAction(seq);
}

void Card::onFinishedShimmerEffect()
{
    m_shimmerClipNode->removeFromParent();
    m_shimmerClipNode = NULL;
}

void Card::runHintGlowEffect()
{
    //glow sprite
    float w = getContentSize().width;
    float h = getContentSize().height;

    Sprite* spGlow = Sprite::create("placeBg/glow.png");
    spGlow->setScale(w / spGlow->getContentSize().width * HINT_SCALE, h / spGlow->getContentSize().height * HINT_SCALE);
    spGlow->setPosition(w/2.0, h/2.0);
    spGlow->setLocalZOrder(ZORDER_HINT);
    
    addChild(spGlow);
    
    //glow animation
    spGlow->setOpacity(0);
    FadeIn* fade1 = FadeIn::create(HINT_DURATION);
    FadeOut* fade2 = FadeOut::create(HINT_DURATION);
    CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, spGlow));
    Sequence* seq = Sequence::create(fade1, fade2, fade1, fade2, callback, NULL);
    spGlow->runAction(seq);
}

void Card::runHintShakeEffect()
{
	auto rotateLeft = RotateTo::create(SHAKE_ROTATE_DURATION, -SHAKE_ROTATE_ANGLE);
	auto rotateRight = RotateTo::create(SHAKE_ROTATE_DURATION, SHAKE_ROTATE_ANGLE);
	auto action = RepeatForever::create(Sequence::create(rotateLeft, rotateRight, NULL));
	action->setTag(ACTION_TAG_SHAKE);
	runAction(action);
}

void Card::stopHintShakeAction()
{
	stopActionByTag(ACTION_TAG_SHAKE);

	//init rotation.
	setRotation(0);
}

void Card::hideCardMask()
{
    m_spMask->setVisible(false);
}

void Card::showCardMask()
{
    m_spMask->setVisible(true);
}

void Card::showCardMaskFade()
{
    m_spMask->setOpacity(0);
    showCardMask();
    DelayTime* delayAction = DelayTime::create(FADE_DELAY);
    auto fadeAction = FadeIn::create(FADE_DURATION);
    Sequence* seq = Sequence::create(delayAction, fadeAction, NULL);
    m_spMask->runAction(seq);
}

int Card::getCardNumber()
{
    return m_nNo;
}

void Card::setCardKindAndNumber(int nCardNo)
{
    m_nKind = SGetCardKindNumber(nCardNo);
    m_nNum = SGetCardLevelNumber(nCardNo);
}

int Card::SGetCardKindNumber(int nCardNumber)
{
    int nKind = nCardNumber / 13;
    nKind ++;
    return nKind;
}

int Card::SGetCardLevelNumber(int nCardNumber)
{
    int nNum = nCardNumber % 13;
    nNum ++;
    
    return nNum;
}

void Card::setRemark(int nRemark)
{
    m_nRemark |= ( 1 << nRemark );
}

int Card::getRemark()
{
    return m_nRemark;
}

bool Card::isFace()
{
    return m_spFace->isVisible();
}

void Card::showFace()
{
    if( !isFace() )
        m_spFace->setVisible(true);
}

#pragma mark - face style

std::string Card::getFacePath()
{
    std::string path = "cards/" + Utils::share()->getCardFaceStyleName() + "/face/" + std::to_string(m_nKind) + "-" + std::to_string(m_nNum) + ".png";
    return path;
}

#pragma mark- set background style

std::string Card::getBgPath()
{
    std::string path = "cards/" + Utils::share()->getCardBgStyleName() + "/back/blue.png";
    return path;
}

#pragma mark - animation

void Card::onFlip()
{
	setRemark(REMARK_FLIPING);
    ScaleTo *act1= ScaleTo::create(FLIP_DURATION, 0, 1);
    CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::onReplaceCardImage, this));
    ScaleTo *act2= ScaleTo::create(FLIP_DURATION, 1, 1);
    
    Sequence* act3 = Sequence::create(act1, callback, act2, NULL);
    
    runAction(act3);
}

void Card::runFlipAndShimmerAction(float stTime)
{
    ScaleTo *act1= ScaleTo::create(FLIP_DURATION, 0, 1);
    CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::onReplaceCardImage, this));
    ScaleTo *act2= ScaleTo::create(FLIP_DURATION, 1, 1);
    CallFunc * callShimmer = CallFunc::create(CC_CALLBACK_0(Card::runShimmerAction, this));
    
    DelayTime * delay = DelayTime::create(stTime);
    
    Sequence* act3 = Sequence::create(delay, act1, callback, act2, callShimmer, NULL);
    
    runAction(act3);
}

void Card::onReplaceCardImage()
{
    if( isFace() )
        m_spFace->setVisible(false);
    else
    {
//    	if( m_spBg )
//    	{
//			m_spBg->removeFromParent();
//			m_spBg = NULL;
//    	}

        m_spFace->setVisible(true);
    }
}

void Card::onRotateLeft()
{
    setRotation(-ROTATE_ANGLE);
}

void Card::onRotateRight()
{
    setRotation(ROTATE_ANGLE);
}

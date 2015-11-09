//
//  StockTemp.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/10/15.
//
//

#include "StockTemp.h"

#define CARD_CNT				52

#define kSTOCKTEMP              "sStockTemp"
#define kCARDARRANGEMENT		"sCardArrangement"

void StockTemp::createCards()
{
    std::vector<int> numbers;
    for( int i = 0; i < CARD_CNT; i ++ )
        numbers.push_back(i);
    
    std::random_shuffle(numbers.begin(), numbers.end());
    std::string sCardArrangement = "";

    for( int i = 0; i < 52; i ++ )
    {
    	int nCardNumber = numbers.at(i);
    	sCardArrangement = sCardArrangement + std::to_string(nCardNumber) + " ";

        Card* card = Card::create();
        card->initWithNo(nCardNumber);
        card->hideCardMask();
        addCard(card);
    }

    Utils::SetData(kCARDARRANGEMENT, sCardArrangement);
}

void StockTemp::restoreArrangement()
{
	std::string sCards = Utils::GetData(kCARDARRANGEMENT);
	std::string buf; // Have a buffer string
	std::stringstream ss(sCards); // Insert the string into a stream

	std::vector<std::string> tokens; // Create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);

	if( tokens.size() != CARD_CNT )		//If get invalid information from some errors, create.
	{
		createCards();
		return;
	}

	std::vector<int> numbers;
	for( int i = 0; i < CARD_CNT; i ++ )
		numbers.push_back(std::stoi(tokens.at(i)));

	for( int i = 0; i < CARD_CNT; i ++ )
	{
		int nCardNumber = numbers.at(i);
		Card* card = Card::create();
		card->initWithNo(nCardNumber);
		card->hideCardMask();
		addCard(card);
	}
}

void StockTemp::setDrawThreeMode()
{
    int cnt = getCardCount();
    if( cnt < 2 )
        return;
    
    Vec2 deltaPos = Vec2(m_spBase->getContentSize().width / 3.0, 0);
    Vec2 pos0 = Vec2(m_spBase->getContentSize().width / 2.0, m_spBase->getContentSize().height / 2.0);
    
    if( cnt >= 3 )
    {
        Vec2 pos = pos0 + deltaPos;
        m_vCards.at(cnt-2)->setPosition(pos);
        
        pos = pos0 + deltaPos + deltaPos;
        m_vCards.at(cnt-1)->setPosition(pos);
        m_vCards.at(cnt-1)->showCardMask();
        
        m_vCards.at(cnt-3)->showCardMask();
    }
    else if( cnt == 2 )
    {
        Vec2 pos = pos0 + deltaPos;
        m_vCards.at(cnt-1)->setPosition(pos);
        m_vCards.at(cnt-2)->showCardMask();
    }
}

void StockTemp::addCard(Card* card)
{
    m_vCards.pushBack(card);
    m_spBase->addChild(card);
    card->setPosition(m_spBase->getContentSize().width / 2.0, m_spBase->getContentSize().height / 2.0);
}

#pragma mark - save and restore state

void StockTemp::saveState(int nMove)
{
    CardGroup::saveState(nMove, kSTOCKTEMP);
}

void StockTemp::restoreState(int nMove)
{
    std::vector<std::string> vCardNo = CardGroup::getState(nMove, kSTOCKTEMP);
    
    reset();
    
    int cnt = vCardNo.size();
    for( int i = 0; i < cnt; i += 2 )
    {
        std::string strNo = vCardNo.at(i);
        int nCarNumber = std::stoi(strNo);
        setCard(nCarNumber);
        showLastCardFace();
        
        if( i != cnt - 2 )
            getLastCard()->hideCardMask();
    }
    
    if( Utils::share()->getDrawMode() == DRAW_MODE_THREE )
        setDrawThreeMode();
}

#pragma mark - fold cards

void StockTemp::foldCards(int nCnt)
{
    float fDeltaX = m_spBase->getContentSize().width / 3.0;
    Vec2 despos = Vec2(m_spBase->getContentSize().width / 2.0, m_spBase->getContentSize().height / 2.0);
    int cnt = getCardCount();
    if( cnt < 3 )
        return;
    
    for( int i = cnt - 2; i < cnt; i ++ )
    {
        Card * card = m_vCards.at(i);
        if( nCnt == 1 && i == cnt - 1 )
            despos = despos + Vec2(fDeltaX, 0);
        auto moveAction = MoveTo::create(FOLD_DURATION, despos);
        card->runAction(moveAction);
    }
}

void StockTemp::unfoldCards()
{
    int cnt = getCardCount();
    if( cnt >= 3 )
    {
        Vec2 pos = Vec2(m_spBase->getContentSize().width / 3.0, 0);
        for( int i = cnt - 1; i >= cnt - 2; i -- )
        {
            auto moveAction = MoveBy::create(FOLD_DURATION, pos);
            Card* card = m_vCards.at(i);
            card->runAction(moveAction);
        }
    }
}

void StockTemp::setThreeCard(int nCardNumber, int nPos)
{
    Vec2 pos = Vec2(m_spBase->getContentSize().width / 2, m_spBase->getContentSize().height / 2);
    float fDelta = m_spBase->getContentSize().width / 3.0;
    pos = pos + Vec2(fDelta * nPos, 0);
    
    Card * card = Card::create();
    card->initWithNo(nCardNumber);
    card->setPosition(pos);
    m_spBase->addChild(card);
    
    m_vCards.pushBack(card);
    
    int cnt = getCardCount();
    if( cnt > 3 )
    {
        int idx = cnt - 4;
        m_vCards.at(idx)->hideCardMask();
    }
}

Vec2 StockTemp::getLastCardPosition()
{
    Vec2 pos = m_spBase->getPosition();
    if( getCardCount() > 0 )
    {
        Card * lastCard = getLastCard();
        float dx = lastCard->getPositionX() - m_spBase->getContentSize().width / 2.0;
        
        pos.x += dx;
    }
    
    return pos;
}

Rect StockTemp::getLastCardBoundingBox()
{
    Rect rc = m_spBase->getBoundingBox();
    
    if( getCardCount() > 0 )
    {
        Card * lastCard = getLastCard();
        float dx = lastCard->getPositionX() - m_spBase->getContentSize().width / 2.0;
        
        rc.origin.x += dx;
    }
    
    return rc;
}

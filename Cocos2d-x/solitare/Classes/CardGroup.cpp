//
//  CardGroup.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/11/15.
//
//

#include "CardGroup.h"

CardGroup::CardGroup()
{
    m_spBase = NULL;
}

void CardGroup::setBaseSprite(Sprite* base)
{
    m_spBase = base;
}

void CardGroup::setNo(int nNo)
{
    m_nNo = nNo;
}

Sprite* CardGroup::getBaseSprite()
{
    return m_spBase;
}

void CardGroup::reset()
{
    for( int i = 0; i < getCardCount(); i ++ )
    {
        Card* card = m_vCards.at(i);
        card->removeFromParent();
    }
    
    m_vCards.clear();
}

int CardGroup::popCard()
{
    if( getCardCount() == 0 )
        return -1;
    
    Card* card = m_vCards.at(m_vCards.size()-1);
    int number = card->getCardNumber();
    card->removeFromParent();
    m_vCards.popBack();
    
    return number;
}

int CardGroup::getCardNumberFromIndex(int index)
{
    if( index >= getCardCount() )
        return -1;
    
    return m_vCards.at(index)->getCardNumber();
}

Card* CardGroup::getLastCard()
{
    Card* card = NULL;
    if( getCardCount() > 0 )
    {
        card = m_vCards.at(m_vCards.size()-1);
    }
    
    return card;
}

void CardGroup::runShimmerAction(float delay)
{
    if( getCardCount() > 0 )
    {
        Card* lastCard = getLastCard();
        if( lastCard->isFace() )
        {
            lastCard->runShimmerActionDelay(delay);
        }
    }
}

int CardGroup::getCardCount()
{
    return (int)m_vCards.size();
}

void CardGroup::setCard(int nCardNumber)
{
    Card * card = Card::create();
    card->initWithNo(nCardNumber);
    card->setPosition(Vec2(m_spBase->getContentSize().width / 2, m_spBase->getContentSize().height / 2));
    m_spBase->addChild(card);
    
    m_vCards.pushBack(card);
}

void CardGroup::showLastCardFace()
{
    Card* lastCard = getLastCard();
    if( lastCard )
    {
        lastCard->showFace();
    }
}

#pragma mark - save and restore data

void CardGroup::saveState(int nMove, std::string key)
{
    std::string data = "";
    for( int i = 0; i < getCardCount(); i ++ )
    {
        data = data + std::to_string(getCardNumberFromIndex(i)) + " ";
        Card * card = m_vCards.at(i);
        if( card->isFace() )
            data = data + "1" + " ";
        else
            data = data + "0" + " ";
    }
    
    Utils::SetData(key + std::to_string(nMove), data);
}

std::vector<std::string> CardGroup::getState(int nMove, std::string key)
{
    std::string data = Utils::GetData(key + std::to_string(nMove));
    
    std::string buf; // Have a buffer string
    std::stringstream ss(data); // Insert the string into a stream
    
    std::vector<std::string> tokens; // Create vector to hold our words
    
    while (ss >> buf)
        tokens.push_back(buf);
    
    return tokens;
}

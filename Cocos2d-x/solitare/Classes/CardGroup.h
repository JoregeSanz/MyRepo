//
//  CardGroup.h
//  RealSolitaire
//
//  Created by LightSky on 2/11/15.
//
//

#ifndef __RealSolitaire__CardGroup__
#define __RealSolitaire__CardGroup__

#include "Utils.h"
#include "Card.h"

class CardGroup
{
public:
    Sprite* m_spBase;
    Vector<Card*> m_vCards;
    int m_nNo;      //number in same groups.
    
public:
    
    CardGroup();
    
    void setBaseSprite(Sprite* base);
    
    void setNo(int nNo);
    
    Sprite* getBaseSprite();

    /**
     *  create card and add.
     **/
    void setCard(int nCardNumber);
    
    /**
     *  return last card.
     **/
    Card* getLastCard();
    
    /**
     *  show shimmer effect
     **/
    void runShimmerAction(float delay);
    
    /**
     *  remove top card, and return card number.
     **/
    int popCard();
    
    /**
     *  return card number at index.
     **/
    int getCardNumberFromIndex(int index);
    
    /**
     *  get the number of cards
     **/
    int getCardCount();
    
    /**
     *  remove all cards
     **/
    void reset();
    
    /**
     *  show last card face.
     **/
    void showLastCardFace();
    
    ////////////// save state ////////////////
    /**
     *  save all cards information.
     **/
    void saveState(int nMove, std::string key);
    
    /**
     *  get card list from memory.
     **/
    std::vector<std::string> getState(int nMove, std::string key);
};

#endif /* defined(__RealSolitaire__CardGroup__) */

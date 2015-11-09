//
//  StockTemp.h
//  RealSolitaire
//
//  Created by LightSky on 2/10/15.
//
//

#ifndef __RealSolitaire__StockTemp__
#define __RealSolitaire__StockTemp__

#include "CardGroup.h"

#define FOLD_DURATION           0.1

class StockTemp : public CardGroup
{
private:
    void setDrawThreeMode();
    
public:
    /**
     *  create cards.
     **/
    void createCards();

    /**
     *	restore card arrangement
     **/
    void restoreArrangement();

    /**
     *  add card.
     **/
    void addCard(Card* card);
    
    ////////////// save state ////////////////
    /**
     *  save all cards information.
     **/
    void saveState(int nMove);
    
    /**
     *  restore state from memory
     **/
    void restoreState(int nMove);
    
    /**
     *  fold cards in draw three mode
     **/
    void foldCards(int nCnt);
    
    /**
     *  unfold cards in draw three mode
     **/
    void unfoldCards();
    
    /**
     *  set card in draw three mode
     **/
    void setThreeCard(int nCardNumber, int nPos);
    
    /**
     *  get last card position in screen coordinate
     **/
    Vec2 getLastCardPosition();
    
    /**
     *  get last card bounding box in screen coordinate
     **/
    Rect getLastCardBoundingBox();
};

#endif /* defined(__RealSolitaire__StockTemp__) */

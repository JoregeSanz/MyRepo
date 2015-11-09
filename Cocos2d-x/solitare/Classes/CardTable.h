//
//  CardTable.h
//  RealSolitaire
//
//  Created by LightSky on 2/10/15.
//
//

#ifndef __RealSolitaire__CardTable__
#define __RealSolitaire__CardTable__

#include "CardGroup.h"

class CardTable : public CardGroup
{
private:
    float m_fDeltaY;
    float m_fFaceDeltaY;
    
    ////// functions //////
    std::string getSaveKey();
    
public:
    
    CardTable();
    
    /**
     *  add card.
     **/
    void setCard(int nCardNumber);
    
    /**
     *  get position of card will be adeed in screen coordinate
     **/
    Vec2 getLastCardPosition();
    
    /**
     *  get card position in screen coordinate
     **/
    Vec2 getCardPosition(int index);
    
    /**
     *  return rect in screen coordinate
     **/
    Rect getRect();

    /**
     *	return hint rect from index card to last card.
     **/
    Rect getHintRect(int index);

    /**
     *  flip the top card
     **/
    void onFlipTopCard();
    
    /**
     *  flip and shimmer effect
     **/
    void runFlipAndShimmerAction(float stTime);
    
    /**
     *  show shimmer effect in all face area
     **/
    void runAllFaceShimmerAction(float delay);
    void beginAllFaceShimmerAction();
    void onFinishedShimmerEffect(Node* sender);
    
    /**
     *  show mask fade effect.
     **/
    void runMaskFadeEffect();
    
    /**
     *  return selected card number.
     **/
    int getSelectedCardNo(const Vec2& point);
    
    /**
     *  remove selected cards( from st card ~ to last card).
     **/
    void removeSelectedCards(int st);
    
    /**
     *  Check merge with moving cards.
     **/
    bool canMerge(int nCardNumber);
    
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
     *	Check if all cards are face.
     **/
    bool isComplete();

    /**
     *	return first face card's index
     **/
    int getFirstFaceIndex();
};

#endif /* defined(__RealSolitaire__CardTable__) */

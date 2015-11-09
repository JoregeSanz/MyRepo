//
//  Foundation.h
//  RealSolitaire
//
//  Created by LightSky on 2/11/15.
//
//

#ifndef __RealSolitaire__Foundation__
#define __RealSolitaire__Foundation__

#include "CardGroup.h"

class Foundation : public CardGroup
{
private:
    Sprite * m_spMask;
    
    ////// functions //////
    std::string getSaveKey();
    
public:
    /**
     *  Check merge with moving cards.
     **/
    bool canMerge(int nCardNumber);
    
    /**
     *  flip the top card
     **/
    void onFlipTopCard();
    
    /**
     *  set background shadow
     *  Note : Before call this function, must call setBaseSprite();
     **/
    void setShadowMask();
    
    /**
     *  show shadow mask
     **/
    void showShadowMask();
    
    /**
     *  hide shadow mask
     **/
    void hideShadowMask();
    
    ////////////// save state ////////////////
    /**
     *  save all cards information.
     **/
    void saveState(int nMove);
    
    /**
     *  restore state from memory
     **/
    void restoreState(int nMove);
};

#endif /* defined(__RealSolitaire__Foundation__) */

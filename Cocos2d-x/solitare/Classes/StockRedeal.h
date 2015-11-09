//
//  StockRedeal.h
//  RealSolitaire
//
//  Created by LightSky on 2/10/15.
//
//

#ifndef __RealSolitaire__StockRedeal__
#define __RealSolitaire__StockRedeal__

#include "CardGroup.h"

class StockRedeal : public CardGroup
{
private:
    Sprite * m_spMask;
    
public:
    
    /**
     *  rearrange cards.
     **/
    void onRearrangeCards();
    
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
    
    /**
     *  show mask fade effect.
     **/
    void runMaskFadeEffect();
    
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
	 *  glow effect
	 **/
	void runHintGlowEffect();

	/**
	 *  set background image to no redeal
	 **/
	void setNoRedeal();
};

#endif /* defined(__RealSolitaire__StockRedeal__) */

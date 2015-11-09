//
//  Card.h
//  RealSolitaire
//
//  Created by LightSky on 2/9/15.
//
//

#ifndef __RealSolitaire__Card__
#define __RealSolitaire__Card__

#include "Utils.h"

#define REMARK_DEFAULT      0
#define REMARK_LAST         1
#define REMARK_BACK   		2
#define REMARK_FLIPING		3

#define REDEAL_MASK_OPACITY     255
#define MASK_OPACITY            210
#define MASK_SCALE              1.4     //1.15
#define HINT_SCALE              1.5
#define SHIMMER_SCALE			0.9

#define SHIMMER_MOVE_DURATION   1.0
#define HINT_DURATION           0.4
#define ZORDER_HINT             -1

#define FLIP_DURATION           0.2

class Card : public Sprite
{
private:
    Sprite * m_spMask;
    Sprite * m_spBg;
    Sprite * m_spFace;
    ClippingRectangleNode * m_shimmerClipNode;
    Sprite * m_spShimmer;
    
    int m_nNo;
    int m_nKind;            //  1~4
    int m_nNum;             //  1~13
    
    int m_nRemark;

    //functions
    /**
     *  get card's kind and set to member variables, number from nCardNo.
     **/
    void setCardKindAndNumber(int nCardNo);

    /**
     *  initialize card background mask
     **/
    void initCardMask();
    
    /**
     *  initialize background image
     **/
    void initCardBackground();

    /**
     *  initialize card face style.
     **/
    void initCardFace();
    
    /**
     *  initialize shimmer effect
     **/
    void initShimmerEffect();
    
    /**
     *  finished shimmer effect
     **/
    void onFinishedShimmerEffect();
    
    /**
     *  get card background resource path
     **/
    std::string getBgPath();
    
    /**
     *  get card face resource path
     **/
    std::string getFacePath();
    
public:
    
    Card();
    static Card * create();
    
    /**
     *  initialize card state with nCardNo(0~51)
     **/
    void initWithNo(int nCardNo);
    
    /**
     * 	initialize card state with nCardNo(0~51) for moving card
     **/
    void initWithNoWithoutBg(int nCardNo);

    /**
     *  return card number
     **/
    int getCardNumber();
    
    /**
     *  return card kind number ( 1 - 4 )
     **/
    static int SGetCardKindNumber(int nCardNumber);
    
    /**
     *  return card level number ( 1 - 13 )
     **/
    static int SGetCardLevelNumber(int nCardNumber);
    
    /**
     *  flip animation
     **/
    void onFlip();
    
    /**
     *  show shimmer effect
     **/
    void runFlipAndShimmerAction(float stTime);

    /**
     *  When flip 90, chage card image.
     **/
    void onReplaceCardImage();
    
    /**
     *  set other information.
     **/
    void setRemark(int nRemark);
    
    /**
     *  get other information setted already.
     **/
    int getRemark();
    
    /**
     *  check if card state is face.
     **/
    bool isFace();
    
    /**
     *  show face
     **/
    void showFace();
    
    /**
     *  hide card mask
     **/
    void hideCardMask();
    
    /**
     *  show card mask
     **/
    void showCardMask();
    
    /**
     *  show card mask with fade effect
     **/
    void showCardMaskFade();
    
    /**
     *  rotate to right.
     **/
    void onRotateRight();
    
    /**
     *  rotate to left
     **/
    void onRotateLeft();
    
    /**
     *  show shimmer effect.
     **/
    void runShimmerAction();
    
    /**
     *  show shimmer effect after delay time
     **/
    void runShimmerActionDelay(float dealy);
    
    /**
     * Hint animation 1
     *  glow effect
     **/
    void runHintGlowEffect();

    /**
     * Hint animation 2
     * shake effect
     **/
    void runHintShakeEffect();

    /**
     *	stop hint shake action.
     **/
    void stopHintShakeAction();
};

#endif /* defined(__RealSolitaire__Card__) */

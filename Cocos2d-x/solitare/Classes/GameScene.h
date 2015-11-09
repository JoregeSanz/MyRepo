//
//  GameScene.h
//  RealSolitaire
//
//  Created by LightSky on 2/9/15.
//
//

#ifndef __RealSolitaire__GameScene__
#define __RealSolitaire__GameScene__

#include "Utils.h"
#include "StockTemp.h"
#include "StockRedeal.h"
#include "Foundation.h"
#include "CardTable.h"
#include "MovingCardGroup.h"

#define CARDTB_CNT          7
#define CARDFD_CNT          4

class GameScene : public Layer
{
private:
    
	Layout* pnlBg;

    //game state
    bool m_bIsRunning;
    bool m_bIsTimeElapse;
    bool m_bHintMoving;
    
    //card arrange
    bool m_bRedeal;		//Redeal is possible?
    bool m_bRedealing;	//When start game, move action from stockTemp to stockRedeal
    bool m_bArranging;

    //check this is first touch move event.
    bool m_bTouchMoving;
    
    //check can touch again.
    bool m_bTouchAgain;
    
    //area began touch.
    int m_nTouchBegin;      //0 : 1
    int m_nfStockRedeal;    //  0 or 1
    int m_nfStockTemp;      //  0 or 1
    int m_nFoundation;      //  0 or 1 - 4
    int m_nCardTable;       //  0 or 1 - 7
        
    //get objects from the design scene
    Layer * pnlBody;
    Layer * pnlMenuBar;
    Layer * pnlScoreBar;
    
    Text * m_lblTime;
    Text * m_lblMoves;
    Text * m_lblScore;
    Text * m_lblElapsedTime;
    ImageView * m_imgGameWon;
    ImageView * m_imgNotWin;
    
    Sprite * spStockNoRedeal;
    
    Button * btnNew;
    Button * btnSettings;
    Button * btnHighScores;
    Button * btnHint;
    Button * btnUndo;
    
    //StockTemp
    StockTemp stockTemp;
    Vec2 m_pRightStockTemp;
    
    //StockRedeal
    StockRedeal stockRedeal;
    int m_nStartNumber;
    

    //Foundation
    Foundation foundations[CARDFD_CNT];
    
    //CardTables
    CardTable cardTables[CARDTB_CNT];
    
    //moving card group
    MovingCardGroup mvCardGroup;
    
    //arrange cards
    int m_nTableNo;
    int m_nNextTable;
    
    //touch
    Vec2 m_pTouchBegin;
    Vec2 m_pTouchEnd;
    Vec2 m_pTouchDelta;     //delta vec2 between anchor point and touch begin point.
    
    //double tap
    long m_lTimeEnd;
    long m_lTimeStart;
    
    Vec2 m_pTouchOld;
    
    //shimmer effect period
    long m_lShimmerTime;
    
    //game was win
    int m_nFDVisitNumber;
    
    //score count
    int m_nMoves;
    int m_nScore;
    int m_nInitScore;
    int m_nElapsedTime;
    int m_nHint;
    
    //setting
    int m_nScoreSetting;	//0 : none, 1 : standard, 2 : vegas, 3 : cumulative
    int m_nDrawMode;        //0 : one, 1 : three
    int m_nRedealCount;		//redeal count 0 ~ 3
    bool m_bGameSolver;
    bool m_bAutoComplete;

    Sprite* m_spHint;
    Vector<Card*> m_vHintCards;

    //functions
    //////////////////////   game control    ///////////////////////
    /**
     *  load scene and get components.
     **/
    void loadScene();
    
    /**
     * 	make UI from cocos studio UI using code.
     */
    Node * initUI();

    /**
     *	get setting configuration and set state.
     **/
    void onGameSetting();

    /**
     *  Clear game board, reset game state.
     **/
    void resetGame();
    
    /**
     *  save game state to local memory
     **/
    void saveGameState();
    
    /**
     *  button click event
     **/
    void touchCallback(Ref* pSender, Widget::TouchEventType type);
    
    /**
     *  clicked "New Game" button.
     **/
    void onClickedNewGame();
    
    /**
     * @param isWon : if isWon is false, collect information only when score mode is vegas.
     * return current game state(draw mode, score, moves, elapsed time)
     **/
    std::string getGameInformation(bool isWon);

    /**
     *	return game result.
     **/
    std::string getGameResult(bool isWon);

    /**
     *  clicked "Hint" button
     **/
    bool onClickHint();
    
    /**
     *	When hint button clicked, show hint layer.
     *	@param rect : transparent rectangle area.
     **/
    void addHintLayer(const Rect& rect, const Rect& rcDes = Rect::ZERO);

    /**
     *	Remove hint layer, and stop hint animation.
     **/
    void stopHintAction();

    /**
     * 	Check if game is winnable.
     **/
    bool isPossibleGameWin();

    /**
     *  undo game state
     **/
    void onClickedUndo();
    
    /**
     *	restart game
     *	isNew , true : quit this game and start new game.
     *			false : restart this game.
     **/
    void onRestartGame(bool isNew);

    /**
     *  arrange cards from StockTemp to CardTables and StockRedeal.
     **/
    void doArrangeCards(float dt);
    
    /**
     *  slide to left stock redeal place.
     **/
    void onSlideToRedealStock(float dt);
    
    /**
     *  arrange move action finished callback function
     **/
    void onFinishedMoveAction(Card* sender);
    
    /**
     *  left slide action finished ( from stockTemp to stockRedeal)
     **/
    void onFinishedLeftSlideAction(Card* sender);
    
    /**
     *  slide and filp action finished ( from stockTemp to stockRedeal by onRedeal event )
     **/
    void onFinishedLeftSlideAndFlipAction(Card* sender);
    
    /**
     *  Arrived merge card to cardTable
     **/
    void onArrivedMergeCardToCardTable(Card* sender);
    
    /**
     *  Arrived merge card to foundation
     **/
    void onArrivedMergeCardToFoundation(Card* sender);

    /**
     *  Arrived merge card to stockTemp
     **/
    void onArrivedMergeCardToStockTemp(Card* sender);
    
    /**
     *  move and flip action from stockRedeal to stockTemp
     **/
    void onFinishedRightSlideFlipAction(Card* sender);
    
    /**
     *  move and flip action from stockRedeal to stockTemp in draw three mode
     **/
    void onFinishedThreeRightSlideFlipAction(Card* sender);
    
    /**
     *  remove sent card from stockTemp
     **/
    void removeSentCardFromStockTemp();
    
    //touch begin
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    //touch end
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    
    //touch end
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    /**
     *  flip and move card from left stockRedeal to right stockTemp
     **/
    void onSlideFlipToTempStock();
    
    /**
     *  flip and move 3 cards from left stockRedeal to right stockTemp in three draw mode
     **/
    void onThreeSlideFlipToTempStock();
    
    /**
     *  check if touch valid area.
     **/
    bool touchValidate();
    
    /**
     *  create moving card group and remove selected cards from cardTable.
     *  @params   nCardTBNumber : cardTable number  , st : start card number in cardTable.
     **/
    void setMovingCards(int nCardTBNumber, int st);
    
    /**
     *  create moving card group including hint card, and invisible that card in cardtable
     *  @param  nCardTBNumber : hit cardtable number. if -1, this is stockTemp
     **/
    void setHintMoveCard(int nCardTBNumber);
    
    /**
     *  create moving card group and remove selected card from foundation.
     *  @params   nFD : foundation number
     **/
    void setMovingCardsFromFoundation(int nFD);

    /**
     *  create moving card group and remove selected card from stockTemp.
     **/
    void setMovingCardsFromStockTemp();
    
    /**
     *  create card group moving by touch and invisible selected card from stockTemp.
     **/
    void setTouchMovingCardsFromStockTemp();
    
    /**
     *  merge moving cards with cardTable
     **/
    void onMergeCardsToCardTable(int nCardTB, bool isBack = false);
    
    /**
     * merge moving card with foundation
     **/
    void onMergeCardToFoundation(int nFD, bool isBack = false);
    
    /**
     *  move hint card to foundation and come back to original point(cardtable or cardtemp)
     *  @param :   nFD, destination(foundation) number
     *  @param :   nDestCardTBNo, destination(catdTable) number When nFD == -1
     *  @param :   nCardTBNo, cardtable number. If value is -1, this is stockTemp
     **/
    void onMoveHintCard(int nFD, int nDestCardTBNo, int nCardTBNo);
    
    void onFinishedHintCardMoving(Card* sender);
    
    /**
     * merge moving card with stockTemp
     **/
    void onMergeCardToStockTemp();
    
    /**
     *  flip new card in cardTable.
     **/
    void onFlipNewCard();
    
    /**
     * 	check if game is completed.
     **/
    void checkGameComplete(float dt);

    /**
     *	check auto complete and start auto complete action
     **/
    void onAutoComplete();

    /**
     *  check double tap event
     **/
    bool isDoubleTap();
    
    /**
     *  when double tapped, check which card is selected and proccess that.
     *  The selected card go to proper place.(foundation or card table)
     **/
    void onDoubleTapped();
    
    /**
     *  set enable touble event again
     **/
    void setTouchEnable(int nRemark);
    
    /**
     *  return foundation number possible to merge
     **/
    int getCanMergeFoundationNo(int nCardNumber);
    
    /**
     *  return card table number possible to merge
     **/
    int getCanMergeCardTableNo(int nCardNumber);
    
    /**
     *  redeal cards.
     *  cards slide and flip with animation from stockTemp to stockRedeal.
     **/
    void onRedeal();
    
    /**
     *	In vegas mode count = 3, redeal disable.
     **/
    void setRedealState();

    /**
     *  when game was win, start congratulate animation
     **/
    void gameWasWin();
    
    /**
     * 	Deprecated.
     *  card move down from foundation to screen bottom. This is iphone version style.
     **/
    void onAnimateCard(float dt);
    
    /**
     *	cards move down from foundation to screen bottom and disperse to random top direction.
     *	This is ipad version style.
     **/
    void playGameWonAction(float dt);

    /**
     *  card leaved from foundation arrived at screen bottom
     **/
    void onArrivedAtBottom(Card* sender);
    
    /**
     * 	Deprecated.
     *  create random cards and throw to random directions. This is iphone version style.
     **/
    void onThrowCards(int nCardNumber, Vec2 pos);
    
    /**
     *	throw to random direction with rotate animation. This is ipad version style.
     **/
    void onThrowCard(int nCardNumber, Vec2 pos);

    /**
     *  show congratulation message box.
     **/
    void onCongratulate(float dt);
    
    /**
     * show shimmer effect of the top card on stockTemp, cardTables, foundations.
     **/
    void showShimmerEffect();
    
    /**
     *  increase moves and score.
     *
     *  When flip top card on stockTable, +5                *
     *  When card move from stockTable to foundation, + 10  *
     *  When card move from foundation to stockTable, -15   *
     *  When card move from stockTemp to stockTable, +5     *
     *  When card move from stockTemp to foundation, + 10   *
     *  When undo, score / 2
     *  When redeal, -100                                   *
     **/
    void increaseMovesAndScore(int nScore);
    
    /**
     *  decrease moves and score.
     **/
    void decreaseMovesAndScore(int nScore);
    
    void setOnlyScore(int nScore);
    
    /**
     *  set moves and score to labels of screen.
     **/
    void setScoreData();
    
    /**
     *  set hint button title
     **/
    void setHint();
    
    /**
     *  set current time.
     **/
    void setCurrentTime(float dt);
    
    /**
     * 	set screen state to right hand mode
     **/
    void setRightHandMode();

    /**
     * 	auto complete action
     **/
    void doAutoComplete(float dt);

    /**
     *	when click hint button, mask effect.
     **/
    Sprite* maskedSpriteWithSprite(Sprite* pTextureSprite, Sprite* pMaskSprite, float x, float y, Sprite* pDestMaskSprite = NULL, float xdes = 0, float ydes = 0);

    /**
     * set enable action buttons( hint button, undo button)
     **/
    void setEnableActionButtons(bool bEnable);

public:
    //there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();
    
    //Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    //implement the "static create()" method manually
    CREATE_FUNC(GameScene);

    void setWallpaper();
};

#endif /* defined(__RealSolitaire__GameScene__) */

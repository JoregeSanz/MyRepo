//
//  GameScene.h
//  PacMan
//
//  Created by jyn on 1/31/14.
//
//

#ifndef __PacMan__GameScene__
#define __PacMan__GameScene__

#include "cocos2d.h"
#include "Config.h"
//#include "XTLayer.h"

enum XTDirection {NONE, LEFT,RIGHT,UP,DOWN };

using namespace cocos2d;
typedef enum{
	GhostStateNormal,
	GhostStateBlue,
	GhostStateSkel,
    GhostStateFadeIn,
	GhostStateAngel
}GhostStateEnum;

typedef enum{
    PandaStateReady,
    PandaStateNormal,
    PandaStateDie
}PandaStateEnum;

class GameSprite : public cocos2d::CCSprite
{
    
    
public:
    CCPoint startPoint;
    CCPoint endPoint;
    float speed;
    GameMoveEnum moveEnum;
    CCTexture2D* oriTexture;

    int index;//ghost, bonus
	bool isBig;

    //Panda
    void setPandaState(int n);
    void pandaReady();
    int pandaState;

    
    //Angel Ghost Var
    char *map;
    bool *vis;
    int *pathx, *pathy;//old point x,y
    int *step;
    int *qx;
    int *qy;
    int rows, cols;
    int stx, sty; //start point
    int zx, zy; //z point
    bool bFinedPath;
    bool bExited;
    //added by dukce.pak
    bool bGhostDied;
    
    int *rltx;
    int *rlty;
    int nStep;
    int nIdx;
    void goHome();
    
    void moveToNextPoint();
    void moveToPoint(CCPoint p);
    bool canMove();

    int ghostState;
    // returns a pointer to new Ball instance.
    static GameSprite* create(const char *pszFileName);
    void setSpeed(float f);
    void setEndPoint(CCPoint pt);
    void setGhostState(int n);
    
    void findPath();
    void findShortestPath(char* map, int startx, int starty, int endx, int endy, int rows, int cols);
    void solve();
    void outpath(int curx, int cury);
    
    void onRecovery();
};
//added by dukce.pak 2014.02.12.15.05
typedef enum {
    BonusNone,
    BonusAppear,
    BonusOut,
} BonusItemState;

typedef struct _OutDoor{
    int xx;
    int yy;
    GameMoveEnum enterDirection;
}OutDoor;

//------
class GameScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    //    void menuCloseCallback(CCObject* pSender);
    void setLevel(int nLevel);
    
    int m_nLevel;
    char *m_map;
    int m_nHighScores[HIGHSCORE_NUM];
    int m_nScore;
//    int m_nLife;
    int m_nItem;
    int m_nRow;
    int m_nCol;
    bool m_isPlaying;
    bool m_bColorMaze;
//    bool m_bItemAppear;
    
    
    float manEatGhostTime;
    
    //modified by dukce.pak
    int manAteGhostNum;
private:
    static int m_nLives;
    static float m_itemXPos;
public:
    static float itemXPos() {return GameScene::m_itemXPos;}
    static void setItemXPos(float xpos) { GameScene::m_itemXPos = xpos;}
    static int Lives(){return GameScene::m_nLives;}
    static void SetLives(int lives){GameScene::m_nLives = lives;}
    float itemAppearTime;
    BonusItemState itemState;
    OutDoor m_outDoor[2];
    void panda_visible_true();
    //------
    
    GameMoveEnum manMoveEnum;

//    CCPoint m_ptExit[2];
    CCPoint m_ptGhostHome;
    
    GameSprite *man;
    CCArray *m_arrayPea;
    CCArray *m_arrayGhost;
    CCArray *m_arrayLife;
//    CCArray *m_arrayBonus;
    GameSprite* m_spBonus;
    CCArray *m_arrayEatBonus;
//added by dukce.pak
    
    CCSprite * m_spHighScore;
    CCSprite * m_spScore;
    CCArray * m_arrHighScore;
    CCArray * m_arrScore;
    bool m_nowPlayingLevel;     //if true then background music is level song, else is power pallet song
    
//    CCLabelTTF* m_pHighScoreLabel;
//    CCLabelTTF* m_pScoreLabel;

    ~GameScene();
    CCSprite*   m_spReady;
    CCSprite*   m_spReadyNum;
    int         m_nReadyNum;
    
    CCTexture2D* m_teBlueGhost;
    CCTexture2D* m_teSkelGhost;
    CCTexture2D* m_teAngelGhost;
    
    CCSprite* m_spLoseSkel;
    CCSprite* m_spDyingPanda;
    
    CCSprite* m_spPause;
    CCSprite* m_spPauseScreen;
    
    CCPoint m_ptPrev;
    bool m_bDrag;
    
    void deleteArray();
    
    void getReady();
    void onResume();
    void onRestart();
//added by dukce.pak
    void onQuit();
    
    void pandaDied();
    void preloadSound();
    void playBGSound(bool bPlay);
    
    virtual void draw(void);
    virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
    virtual void ccTouchesBegan(cocos2d::CCSet* _touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* _touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* _touches, cocos2d::CCEvent* event);
    
    void updateInterval();
    void updateHighScore();
    void updateScore();
    
    void layoutGameSprites(void);
    // modified by dukce.pak 
    void drawWallPath(int x, int y, float margins, XTDirection d);
    void addPeaWithPoint(int x, int y, bool bBig);
    void addManWithPoint(int x, int y);
    void addGhostWithPoint(int x, int y);
    void addPeace(int x, int y, char s);
    void addItem();
    void addScore(int nScore, CCPoint pt, bool bPopup);
    
    void startGame();
    void stopGame();
    
    void updateEatItems();
    
    void win();
    void gameOver();
    void addLife();
    void finishLoseAnimation();
    
    void loadHighScores();
    void saveHighScores();
    
    bool isCrossPointWithSprite(GameSprite* spirit);
    bool isTopPointWithSprite(GameSprite* spirit);
    
    CCPoint getCenterWithPoint(float px, float py);
    CCPoint getGridPointWithPoint(CCPoint p);
    GameRoleEnum getRoleEnumByPoint(float px, float py);
    bool moveSpiritWidthOrientation(GameSprite* spirit, GameMoveEnum move, bool trans);
    
//    virtual void xtTapGesture(CCPoint position);
//    virtual void xtSwipeGesture(XTTouchDirection direction, float distance, float speed) ;
    //virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* event);
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameScene);
    

};

#endif /* defined(__PacMan__GameScene__) */

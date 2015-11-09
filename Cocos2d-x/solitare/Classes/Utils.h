//
//  Utils.h
//  RealSolitaire
//
//  Created by LightSky on 2/9/15.
//
//

#ifndef __RealSolitaire__Utils__
#define __RealSolitaire__Utils__

#include "SimpleAudioEngine.h"

#include "cocos2d.h"
#include "CocosGUI.h"

#include "cocostudio/CocoStudio.h"

using namespace cocos2d;
using namespace cocos2d::ui;


#define DESIGN_WIDTH                        768.0
#define DESIGN_HEIGHT                       1024.0

#define DESIGN_WIDTH_LANDSCAPE				1136
#define DESIGN_HEIGHT_LANDSCAPE				640

#define FADE_DELAY                          2
#define FADE_DURATION                       1

#define HINT_COUNT                          10

#define DRAW_MODE_ONE                       0
#define DRAW_MODE_THREE                     1

class Utils
{
private:
    
    std::vector<std::string> m_vSettingCardFace;
    std::vector<std::string> m_vSettingCardBack;
    std::vector<std::string> m_vSettingWallpaper;
    
    Size cardSize;
    Size realCardSize;
    float cardScaleX;
    float cardScaleY;
    
    std::string sCardBackStyle;
    std::string sCardFaceStyle;
    
    std::string m_sfilepath;

public:
    
    std::map<std::string, std::string> mapState;

    static Utils * share();
    
    Utils();
    
    /**
     *  set current card place size in design.
     **/
    void setCardSize(Size size);
    
    /**
     *  set card size
     **/
    Size getCardSize();
    
    /**
     *  caculate scale current card size to design size.
     **/
    void calcCurrentScale(Size curSize);
    
    //------------------------------
    //  save to memory
    //------------------------------
    /**
     *  save card background style
     **/
    void saveCardBg(int nStyle);
    
    /**
     *  get card background style.
     **/
    std::string getCardBgStyleName();

    /**
     *  save card face style
     **/
    void saveCardFace(int nStyle);
    
    /**
     *  get card face style
     **/
    std::string getCardFaceStyleName();
    
    /**
     * 	save wallpaper image number
     **/
    void saveWallpaperNumber(int nNumber);

    /**
     *	get wallpaper image number
     **/
    int getWallpaperNumber();

    /**
     *	get wallpaper image name
     **/
    std::string getWallpaperName(int nNumber);

    /**
     * 	get wallpaper full path
     **/
    std::string getWallpaperPath();

    //setting information
	/**
	 *	nGeneral :	Draw, Scroing, Status
	 *	nWallpaper :	wallpaper image number
	 *	nCards :	face style, back style
	 */
	void setSettings(int nGeneral, int nWallpaper, int nCards, int nAdvanced);
	void setRedealCount(int nCount);
	int getRedealCount();
	int getSettingsGeneral();
	int getSettingsCardStyle();
	bool isPlaySound();
	bool isTimedGame();
	bool isShowMoves();
	int getSettingScoring();
    int getDrawMode();
    int getAdvancedSettings();
    bool getRightHandMode();
    bool getAutoComplete();
    bool getGameSolver();
    bool isRedealPossible();

    void saveGameState();

    //sound effect
    static void preloadSounds();
    static void playCardFlip();
    static void playCardSlide();
    static void playFoundation();
    static void playGameWon();
    static void playRedeal();
    static void playShuffle();
    
    //save data to local memeory
    static void SetFaceCardDistance(float fDistance);
    static float GetFaceCardDistance();
    
    ///////////////////// save game state   ///////////////////////////
    //save score data
    static void SetScoreData(int nMoves, int nScore, int nElapsedTime);
    static int GetMoves();
    static int GetScore();
    static void SetInitialScore(int nInitScore);
    static int GetInitialScore();
    static int GetElapsedTime();
    static void SetHintCount(int nHintCnt);
    static int GetHintCount();
    
    static void SetData(std::string key, std::string data);
    static std::string GetData(std::string key);

    //new game information
    static void SetNewGame(bool isNew);
    static bool GetNewGame();

    //game state keep
    static void SetKeepGame(bool bKeep);
    static bool GetKeepGame();
    
    //shimmer time
    static void SetShimmerTime(long lTime);
    static long GetShimmerTime();

    //orientation event
    static void SetOrientation(bool isPortrait);
    static bool GetOrientation();
};

#endif /* defined(__RealSolitaire__Utils__) */

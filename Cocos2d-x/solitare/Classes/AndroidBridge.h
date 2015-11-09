/**
 * AndroidBridge header file
 *  author : LightSky
 *  since : 2015/02/18
 **/

#include "cocos2d.h"
#include "GameScene.h"

using namespace cocos2d;


class AndroidBridge : public Node{

private:

	GameScene * m_GameScene;

	void onSetGame();

	void replaceScene();

public:

	static AndroidBridge* getInstance();

	AndroidBridge(){m_GameScene = NULL;};

	void setGameScene(GameScene* gameScene);

	//call
	void onNewGame(std::string info, std::string result);
	void onSetting(std::string filepath);
	void onHighScores();
	void notifyNoWinnable();
	void didGameWon(std::string info, std::string result);

	//call back
	void onQuitAndStartGame();
	void onRestartGame();
	void onSetGame(int nGeneral, int nWallpaper, int nCardStyle, int nAdvanced, int nNewGameStart);
	void onChangedOrientation(bool isPortrait, float w, float h);
	void saveGameState();
};


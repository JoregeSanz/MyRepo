//
//  Config.h
//  QuillysRun
//
//  Created by Ry on 2013.9.5
//
//

#ifndef __Config_h__
#define __Config_h__

#include "cocos2d.h"

USING_NS_CC;

#define SND_BIGCHOMP "big chomp.mp3"
#define SND_COINDROP "coin drop.mp3"
#define SND_DEATH   "death.mp3"
#define SND_GATE    "heavens gates.mp3"
#define SND_COMPLETE "level complete.mp3"
#define SND_NEON    "neon humming.mp3"
#define SND_SMALLPELLET "small pellet.mp3"

//added by dukce.pak
#define Init_LifeNum   3
#define SND_LEVEL(a) CCString::createWithFormat("LEVEL%d.mp3",a)->getCString()
#define SND_POWER(a) CCString::createWithFormat("POWER%d.mp3",a)->getCString()


#define SCORE_PADDING_RATE 0.2


void addGlowEffect(CCSprite* sprite,  const ccColor3B& colour, const CCSize& size);

typedef enum{
	GameRoleNoneEnum,
	GameRoleSmallPeaEnum,
	GameRoleBigPeaEnum,
	GameRoleManEnum,
	GameRoleGhostEnum,
	GameRoleWallEnum,
    GameRoleExitEnum,
    GameRoleCrossEnum,
    GameRoleTopWayEnum,
    GameRoleHomeEnum
}GameRoleEnum;
typedef enum{
	GameMoveNoneEnum,
	GameMoveLeftEnum,
	GameMoveTopEnum,
	GameMoveRightEnum,
	GameMoveBottomEnum
}GameMoveEnum;

typedef enum{
    zOrderPel,
    zOrderGhost,
    zOrderMan,
    zOrderBigItem
}GameZOrder;
#define MAX_LEVEL   8
#define HIGHSCORE_NUM   8

#define DESIGN_RESOLUTION_WIDTH     640.f
#define DESIGN_RESOLUTION_HEIGHT    1136.f
extern ccColor4F g_drawColor[MAX_LEVEL];
extern ccColor4F g_borderColor;
extern ccColor4F g_drawLevelLineColor[MAX_LEVEL];

GameRoleEnum getRoleEnum(char role);

class Config
{
public:
    static void setScaleFactor(CCSize size);
    static void setScale(CCNode *node);
    static void setScale(CCNode *node, float factorX, float factorY);
      
    static float getX(float x);
    static float getY(float y);
    static float getyScale();
    static float getxScale();
    
};
static float __attribute__((unused)) _scaleX;
static float __attribute__((unused)) _scaleY;

#endif /* defined(__Config_h__) */

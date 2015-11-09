//
//  Define.h
//  TingTong
//
//  Created by jyn on 4/10/14.
//
//

#ifndef TingTong_Define_h
#define TingTong_Define_h

#define kDefaultActionTag 100
#define kZoomActionTag 101

#define MENU_SOUND_TAG      10
#define MENU_EFFECT_TAG     11
#define MENU_HELP_TAG       12
#define MENU_RETURN_TAG     13

#define DESIGN_RESOLUTION_WIDTH     1920.f
#define DESIGN_RESOLUTION_HEIGHT    1080.f

#define HEART_TAG   90
#define LEVEL "level"
#define SOUNDONOFF "soundstatus"
extern float g_fResScale;


#define PTM_RATIO 32

#define WinWidth     CCDirector::sharedDirector()->getWinSize().width
#define WinHeight     CCDirector::sharedDirector()->getWinSize().height

#define IsPad (WinWidth/WinHeight < 1.5)
#define BodyScale (IsPad?1.0:0.5)

struct LevelInfo {
    float   Time;
    float   Speed;
    int     Moves;
    int     Lifes;
    int     Points1;
    int     Points2;
    int     Points3;
    int     Ticks;
    int     Levelnum;
    int     Score;
    
};

#endif

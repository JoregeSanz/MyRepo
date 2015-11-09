//
//  CarGame.h
//  TingTong
//
//  Created by jyn on 4/12/14.
//
//

#ifndef __TingTong__CarGame__
#define __TingTong__CarGame__

#include "cocos2d.h"
#include "BaseLayer.h"
#include <spine/spine-cocos2dx.h>
#include "CCSvParse.h"

USING_NS_CC;



#define ROAD_TEX_NUM    17

class CarGame : public BaseLayer
{
private:
	cocos2d::extension::CCSkeletonAnimation* m_car;

    float m_sx, m_sy;
    int m_nRoadLine;
    int m_nPrevline;
    float time_Temp;
    int number_Level;
    int str_Line;
    int line_number_EqualToLevel;
    int m_nTicknum;
    float obstacleinterval;
    bool startstream;
    
    CCArray* m_aryRoadTexture;
    CCArray* m_aryRoadRight;
    CCArray* m_aryRoadLeft;
    CCArray* m_aryObstacle;
        
    CCArray* m_aryStreamLoad;
    CCSprite* road;
    CCLabelTTF* lbl_levelNum;
    CCLabelTTF* lbl_Score;
    CCLabelTTF* lbl_Time;
    CCLabelTTF* lbl_Target;
    //int m_Score;
    
    
    CCLayer* m_roadLayer;
    
    
    
    CCPoint touchpoint_;
    void createRoadLayer();
    void setRoadLine(int n);
    void landcar();
    CCSprite* animSprite;
    
    //method to read words and its translation
    void readWordInfo();
    void updatelbltarget();
    void stoppowerspeed();    
    void goGameSelect();
    void goGameOver();
    
    LevelInfo m_levelInfo;
public:
    CCArray* wordinfo;
    CCSVParse* levelinfo;
    
    float m_fRoadScale[ROAD_TEX_NUM][2];
    float m_fRoadY[ROAD_TEX_NUM];
    
    bool iscrashing;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    void update(float delta);
    void roadStream();
    void moveRoad(float delta);
    // a selector callback
    void menuCallback(CCObject* pSender);
    void checkCollision();
    void ccTouchesBegan(cocos2d::CCSet* _touches, cocos2d::CCEvent* event);
    
    void stopRunning();
    void continueRunning();
    // implement the "static node()" method manually
    CREATE_FUNC(CarGame);
    
    
};
#endif /* defined(__TingTong__CarGame__) */

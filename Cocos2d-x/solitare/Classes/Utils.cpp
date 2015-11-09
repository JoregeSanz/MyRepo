//
//  Utils.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/9/15.
//
//

#include "Utils.h"

#define kSETTING_WAllPAPER					"nWallPaper"
#define kSETTING_CARD_FACE                  "nSettingCardFace"
#define kSETTING_CARD_BACK                  "nSettingCardBack"
#define kCARD_FACE_DIS                      "fCardFaceDistance"

#define kMOVES                              "nMoves"
#define kSCORE                              "nScore"
#define kINITSCORE							"nInitScore"
#define kELAPSEDTIME                        "nElapsedTime"
#define kHINTCNT                            "nHintCnt"
#define kREDEALCOUNT						"nRedealCount"

#define kADVANCED							"nAdvancedSettings"
#define kBITHNADMODE						0
#define kBITAUTOCOMPLETE					1
#define kBITGAMESOLVER						2

#define kNEWGAME							"bNewGame"
#define kKEEPGAME							"bKeepGame"

#define kSHIMMERTIME                        "lShimmerTime"

#define kORIENTATION						"bIsPortrait"

#define CARD_FACE_DISTANCE                  25	//30

#define INDEX_SOUND							8
#define INDEX_SHOW_MOVES					7
#define INDEX_TIMED_GAME					6
#define	INDEX_CUMULATIVE					5
#define INDEX_VEGAS							4
#define INDEX_STANDARD						3
#define INDEX_NONE							2

static Utils * s_gc;

Utils* Utils::share()
{
    if( !s_gc )
    {
        s_gc = new Utils();
    }
    
    return s_gc;
}

Utils::Utils()
{
    //initialize setting / card / file path strings
    
    m_vSettingCardFace.push_back("standard");
    m_vSettingCardFace.push_back("classic");
    
    sCardFaceStyle = m_vSettingCardFace.at(0);
    
    m_vSettingCardBack.push_back("standard");
    m_vSettingCardBack.push_back("classic");
    
    sCardBackStyle = m_vSettingCardBack.at(0);

    const int nNum = 19;
    std::string sWallpaperNames[] = {"wp_green", "wp_blue", "wp_red", "wp_wood", "wp_peacock", "wp_red_sea_bannerfish",
    		"wp_rainforest", "wp_spring_flowers", "wp_tropical", "wp_penguins",
    		"borealis", "lava", "mirrorlake", "newyorkcity",
    		"raindrops", "santamonica", "skydive",
    		"custom"};
    for( int i = 0; i < nNum; i ++ )
    	m_vSettingWallpaper.push_back(sWallpaperNames[i]);
    
    m_sfilepath = FileUtils::getInstance()->getWritablePath();
    m_sfilepath = m_sfilepath + "gamestate";
    
    //get game state
    __Dictionary * dict = __Dictionary::createWithContentsOfFile(m_sfilepath.c_str());
    __Array * keys = dict->allKeys();
    if( keys)
    {
        for( int i = 0; i < keys->count(); i ++ )
        {
            std::string key = ((__String*)keys->getObjectAtIndex(i))->getCString();
            const __String * value = dict->valueForKey(key);
            mapState[key] = value->getCString();
        }
    }
}

void Utils::saveGameState()
{
	log(" ******  saveGameState  *****\n");
//	dictState->writeToFile(kDICTFILENAME);
    
    __Dictionary * dict = __Dictionary::create();
    std::map<std::string, std::string>::iterator itr;
    for( itr = mapState.begin(); itr != mapState.end(); itr ++ )
    {
        dict->setObject(__String::create(itr->second), itr->first);
    }

    dict->writeToFile(m_sfilepath.c_str());
}

#pragma mark - card size and scale

void Utils::setCardSize(Size size)
{
    cardSize = size;
}

Size Utils::getCardSize()
{
    return cardSize;
}

void Utils::calcCurrentScale(Size curSize)
{
    cardScaleX = cardSize.width / curSize.width;
    cardScaleY = cardSize.height / curSize.height;
}

#pragma mark - sound

void Utils::preloadSounds()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/CardFlip.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/CardSlide.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/Foundation.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/GameWon.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/Redealt.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/Shuffle.mp3");
}

void Utils::playCardFlip()
{
	if(Utils::share()->isPlaySound())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/CardFlip.mp3", false);
}

void Utils::playCardSlide()
{
	if(Utils::share()->isPlaySound())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/CardSlide.mp3", false);
}

void Utils::playFoundation()
{
	if(Utils::share()->isPlaySound())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/Foundation.mp3", false);
}

void Utils::playGameWon()
{
	if(Utils::share()->isPlaySound())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/GameWon.mp3", false);
}

void Utils::playRedeal()
{
	if(Utils::share()->isPlaySound())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/Redealt.mp3", false);
}

void Utils::playShuffle()
{
	if(Utils::share()->isPlaySound())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/Shuffle.mp3", false);
}

#pragma mark - set and get data

void Utils::SetFaceCardDistance(float fDistance)
{
    UserDefault::getInstance()->setFloatForKey(kCARD_FACE_DIS, fDistance);
}

float Utils::GetFaceCardDistance()
{
    return UserDefault::getInstance()->getFloatForKey(kCARD_FACE_DIS, CARD_FACE_DISTANCE);
}

#pragma mark - card background style

void Utils::saveCardBg(int nStyle)
{
    UserDefault::getInstance()->setIntegerForKey(kSETTING_CARD_BACK, nStyle);
}

std::string Utils::getCardBgStyleName()
{
	int nStyle = UserDefault::getInstance()->getIntegerForKey(kSETTING_CARD_BACK);
	std::string styleName = m_vSettingCardBack.at(nStyle);
	sCardBackStyle = styleName;

    return sCardBackStyle;
}

#pragma mark - card face style

void Utils::saveCardFace(int nStyle)
{
    UserDefault::getInstance()->setIntegerForKey(kSETTING_CARD_FACE, nStyle);
}

std::string Utils::getCardFaceStyleName()
{
	int nStyle = UserDefault::getInstance()->getIntegerForKey(kSETTING_CARD_FACE);
	std::string styleName = m_vSettingCardFace.at(nStyle);
	sCardFaceStyle = styleName;
    return sCardFaceStyle;
}

void Utils::saveWallpaperNumber(int nNumber)
{
    UserDefault::getInstance()->setIntegerForKey(kSETTING_WAllPAPER, nNumber);
}

int Utils::getWallpaperNumber()
{
	return UserDefault::getInstance()->getIntegerForKey(kSETTING_WAllPAPER, 3);
}

std::string Utils::getWallpaperName(int nNumber)
{
	return m_vSettingWallpaper.at(nNumber);
}

std::string Utils::getWallpaperPath()
{
	std::string res;

	std::string name = getWallpaperName(getWallpaperNumber());
	log("## Utils::getWallpaperPath() : %s ##\n", name.c_str());

	if( name == "custom" )
	{
		res = FileUtils::getInstance()->getWritablePath() + "custom.png";
		return res;
	}

	if( GetOrientation() == false ) //landscape
		name = name + "_landscape";
	res = "bg/" + name + ".png";
	log("##  --- %s --- ##\n", res.c_str());
	return res;
}

#pragma mark - save game state

void Utils::SetScoreData(int nMoves, int nScore, int nElapsedTime)
{
    UserDefault::getInstance()->setIntegerForKey(kMOVES, nMoves);
    UserDefault::getInstance()->setIntegerForKey(kSCORE, nScore);
    UserDefault::getInstance()->setIntegerForKey(kELAPSEDTIME, nElapsedTime);
}

int Utils::GetMoves()
{
    return UserDefault::getInstance()->getIntegerForKey(kMOVES);
}

void Utils::SetHintCount(int nHintCnt)
{
    UserDefault::getInstance()->setIntegerForKey(kHINTCNT, nHintCnt);
}

int Utils::GetHintCount()
{
    return UserDefault::getInstance()->getIntegerForKey(kHINTCNT, HINT_COUNT);
}

int Utils::GetScore()
{
    return UserDefault::getInstance()->getIntegerForKey(kSCORE);
}

void Utils::SetInitialScore(int nInitScore)
{
	UserDefault::getInstance()->setIntegerForKey(kINITSCORE, nInitScore);
}

int Utils::GetInitialScore()
{
    return UserDefault::getInstance()->getIntegerForKey(kINITSCORE);
}

int Utils::GetElapsedTime()
{
    return UserDefault::getInstance()->getIntegerForKey(kELAPSEDTIME);
}

void Utils::SetData(std::string key, std::string data)
{
//    UserDefault::getInstance()->setStringForKey(key.c_str(), data);
    Utils::share()->mapState[key] = data;
    
//	Utils::share()->saveGameState();
}

std::string Utils::GetData(std::string key)
{
//    return UserDefault::getInstance()->getStringForKey(key.c_str());
    return Utils::share()->mapState[key];
}

#pragma mark - save new_game setting

void Utils::SetNewGame(bool isNew)
{
	UserDefault::getInstance()->setBoolForKey(kNEWGAME, isNew);
}

bool Utils::GetNewGame()
{
	return UserDefault::getInstance()->getBoolForKey(kNEWGAME, true);
}

void Utils::setRedealCount(int nCount)
{
	UserDefault::getInstance()->setIntegerForKey(kREDEALCOUNT, nCount);
}

int Utils::getRedealCount()
{
	return UserDefault::getInstance()->getIntegerForKey(kREDEALCOUNT);
}

void Utils::setSettings(int nGeneral, int nWallpaper, int nCards, int nAdvanced)
{
	const int keyNum = 9;
	std::string sKeys[] = {"kSettingDrawOne", "kSettingDrawThree", "kSettingNone", "kSettingStandard", "kSettingVegas",
			"kSettingCumulative", "kSettingTimedGame", "kSettingShowMoves", "kSettingGameSounds" };

	//save general information
	for( int i = 0; i < keyNum; i ++ )
	{
		bool bValue = false;
		if( nGeneral & (1<<i) )
			bValue = true;

		UserDefault::getInstance()->setBoolForKey(sKeys[i].c_str(), bValue);
	}

	//wallpaper
	saveWallpaperNumber(nWallpaper);

	//card style : face style
	int nStyle = 0;			//standard
	if( nCards & (1 << 1))	//classic
			nStyle = 1;
	saveCardFace(nStyle);

	//card style : back style
	nStyle = 0;
	if( nCards & (1 << 3) )
		nStyle = 1;		//classic

	saveCardBg(nStyle);

	//advanced settins
	UserDefault::getInstance()->setIntegerForKey(kADVANCED, nAdvanced);
}

int Utils::getAdvancedSettings()
{
	return UserDefault::getInstance()->getIntegerForKey(kADVANCED);
}

bool Utils::getRightHandMode()
{
	int nAdvanced = getAdvancedSettings();
	bool res = false;
	if( nAdvanced & (1<<kBITHNADMODE) )
		res = true;

	return res;
}

bool Utils::getAutoComplete()
{
	int nAdvanced = getAdvancedSettings();
	bool res = false;
	if( nAdvanced & (1<<kBITAUTOCOMPLETE) )
		res = true;

	return res;
}

bool Utils::getGameSolver()
{
	int nAdvanced = getAdvancedSettings();
	bool res = false;
	if( nAdvanced & (1<<kBITGAMESOLVER) )
		res = true;

	return res;
}

bool Utils::isRedealPossible()
{
	int nSettingScoring = getSettingScoring();
	return nSettingScoring <= 1 || ( nSettingScoring > 1 && getRedealCount() > 0 );
}

int Utils::getSettingsGeneral()
{
	const int keyNum = 9;
	std::string sKeys[] = {"kSettingDrawOne", "kSettingDrawThree", "kSettingNone", "kSettingStandard", "kSettingVegas",
			"kSettingCumulative", "kSettingTimedGame", "kSettingShowMoves", "kSettingGameSounds" };

	int res = 0;
	for( int i = 0; i < keyNum; i ++ )
	{
		bool defaultValue = false;
		if( i == 0 )	//drawone
			defaultValue = true;
		else if( i == 3 )	//standard
			defaultValue = true;
		else if( i == 7 ) //show moves
			defaultValue = true;
		else if( i == 8 ) //game sounds
			defaultValue = true;

		bool bValue = UserDefault::getInstance()->getBoolForKey(sKeys[i].c_str(), defaultValue);
		if( bValue )
			res |= (1<<i);
	}

	return res;
}

int Utils::getSettingsCardStyle()
{
	int res = 0;
	int nStyle = UserDefault::getInstance()->getIntegerForKey(kSETTING_CARD_FACE);
	if( nStyle == 1 )
		res |= (1<<1);

	nStyle = UserDefault::getInstance()->getIntegerForKey(kSETTING_CARD_BACK);
	if( nStyle == 1 )
		res |= (1<<3);

	return res;
}

bool Utils::isPlaySound()
{
	int nGeneral = getSettingsGeneral();
	bool res = false;
	if( nGeneral & (1<<INDEX_SOUND) )
		res = true;
	return res;
}

int Utils::getDrawMode()
{
    bool isOne = UserDefault::getInstance()->getBoolForKey("kSettingDrawOne", true);
    int res = isOne ? DRAW_MODE_ONE : DRAW_MODE_THREE;
    return res;
}

bool Utils::isTimedGame()
{
	int nGeneral = getSettingsGeneral();
	bool res = false;
	if( nGeneral & (1<<INDEX_TIMED_GAME) )
		res = true;
	return res;
}

bool Utils::isShowMoves()
{
	int nGeneral = getSettingsGeneral();
	bool res = false;
	if( nGeneral & (1<<INDEX_SHOW_MOVES) )
		res = true;

	return res;
}

int Utils::getSettingScoring()
{
	int nGeneral = getSettingsGeneral();
	int res = 0;
	if( nGeneral & (1<<INDEX_NONE ) )
		res = 0;
	else if( nGeneral & (1<<INDEX_STANDARD))
		res = 1;
	else if( nGeneral & (1<<INDEX_VEGAS))
	{
		if( nGeneral & (1<<INDEX_CUMULATIVE) )
			res = 3;
		else
			res = 2;
	}

	return res;
}

void Utils::SetKeepGame(bool bKeep)
{
	if( bKeep == false )
	{
		int nCount = 3;
		if( Utils::share()->getDrawMode() == DRAW_MODE_ONE )
			nCount = 1;

		Utils::share()->setRedealCount(nCount);
	}

	UserDefault::getInstance()->setBoolForKey(kKEEPGAME, bKeep);
}

bool Utils::GetKeepGame()
{
	return UserDefault::getInstance()->getBoolForKey(kKEEPGAME);
}

#pragma mark - shimmer effect time

void Utils::SetShimmerTime(long lTime)
{
    UserDefault::getInstance()->setIntegerForKey(kSHIMMERTIME, (int)lTime);
}

long Utils::GetShimmerTime()
{
    return (long)UserDefault::getInstance()->getIntegerForKey(kSHIMMERTIME);
}

#pragma mark - orientation change event

void Utils::SetOrientation(bool isPortrait)
{
	UserDefault::getInstance()->setBoolForKey(kORIENTATION, isPortrait);
}

bool Utils::GetOrientation()
{
	return UserDefault::getInstance()->getBoolForKey(kORIENTATION, false);	//true portrait, false landscape
}

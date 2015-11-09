//
//  GameScene.cpp
//  RealSolitaire
//
//  Created by LightSky on 2/9/15.
//
//

#include "GameScene.h"
#include "AndroidBridge.h"

/***************************
 Check platform
 ****************************/

// define supported target platform macro which CC uses.
#define CC_PLATFORM_UNKNOWN            0
#define CC_PLATFORM_IOS                1
#define CC_PLATFORM_ANDROID            2
#define CC_PLATFORM_WIN32              3
#define CC_PLATFORM_MARMALADE          4
#define CC_PLATFORM_LINUX              5
#define CC_PLATFORM_BADA               6
#define CC_PLATFORM_QNX                7

// Determine tartet platform by compile environment macro.
#define CC_TARGET_PLATFORM             CC_PLATFORM_UNKNOWN

// iphone
#if ! CC_TARGET_PLATFORM && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_IOS
#define CC_SUPPORT_PVRTC
#endif

// android
#if ! CC_TARGET_PLATFORM && defined(ANDROID)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_ANDROID
#endif

// win32
#if ! CC_TARGET_PLATFORM && (defined(WIN32) && defined(_WINDOWS))
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_WIN32
#endif

// linux
#if ! CC_TARGET_PLATFORM && defined(LINUX)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_LINUX
#endif

// marmalade
#if ! CC_TARGET_PLATFORM && defined(MARMALADE)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_MARMALADE
#endif

// bada
#if ! CC_TARGET_PLATFORM && defined(SHP)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_BADA
#endif

// qnx
#if ! CC_TARGET_PLATFORM && defined(__QNX__)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM     CC_PLATFORM_QNX
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! CC_TARGET_PLATFORM
//#error  "Can not recognize the target platform, compling under a unsupported platform?"
#endif

///////////////////////////////////////////////////////////////////////////

#define ARRANGE_DELAY           0.05         //0.1
#define MOVE_DURATION           0.20        //0.25

#define SLIDE_LEFT_DELAY        0.01
#define SLIDE_LEFT_DURATION     0.1

#define SLIDE_FLIP_DURATION     0.3

#define MOVE_TARGET_DURATION    0.2
#define MOVE_TARGET_DELAY       0.04

#define ZORDER_TEMP             1
#define ZORDER_MOVE             10
#define ZORDER_WON				100

#define TIME_ACTIVATE           1       //second
#define DISTANCE_ACTIVATE       20      //for double tap

#define GAMEWIN_DURATION        0.9
#define GAMEWIN_THROW_DURATION  1.0

//ipad version style
#define GAMEWON_FD_DURATION		1.0		//each foundation delay that start move down action.
#define GAMEWON_MOVEDOWN_DELAY	0.04	//52 cards
#define GAMEWON_END_DURATION	3.0		//until show congratulation view, delay time.

#define CARDS_NUM               20

#define SHIMMER_DEALY           0.1
#define SHIMMER_DURATION        (5 * 60)    //5min

#define SCALE_HINTMASK			1.35

Scene* GameScene::createScene()
{
    // 'scene' is autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    //add layer as a child to scene
    scene->addChild(layer);
    
    //return the scene
    return scene;
}

//on "init" you need to initialize your instance
bool GameScene::init()
{
    // super init first
    if( !Layer::init() )
        return false;
    
    //add touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    m_bIsRunning = false;
    m_bIsTimeElapse = false;
    m_bArranging = false;
    m_bRedealing = false;

    loadScene();
    
    //double tap
    m_lTimeEnd = 0;
    m_pTouchOld = Vec2(-100, -100);
    
    return true;
}

#pragma mark - game control

void GameScene::setWallpaper()
{
	TextureCache::getInstance()->removeTextureForKey(Utils::share()->getWallpaperPath());
	pnlBg->setBackGroundImage(Utils::share()->getWallpaperPath());
}

Node * GameScene::initUI()
{
	Node * scene = Node::create();

	//PnlBg
	Layout * pnlBg = Layout::create();
	pnlBg->setName("PnlBg");
	pnlBg->setContentSize(Size(DESIGN_WIDTH_LANDSCAPE, DESIGN_HEIGHT_LANDSCAPE));
	scene->addChild(pnlBg);

	//PnlBody
	Layout * pnlBody = Layout::create();
	pnlBody->setName("PnlBody");
	pnlBody->setContentSize(Size(DESIGN_WIDTH_LANDSCAPE, DESIGN_HEIGHT_LANDSCAPE));
	scene->addChild(pnlBody);

	Sprite * spStockNoRedeal = Sprite::create("placeBg/StockBk-NoRedeal.png");
	spStockNoRedeal->setName("SpStockNoRedeal");
	spStockNoRedeal->setPosition(170, 527);
	spStockNoRedeal->setVisible(false);
	pnlBody->addChild(spStockNoRedeal);

	Sprite * spStockRedeal = Sprite::create("placeBg/Stock-Redeal.png");
	spStockRedeal->setName("SpStockRedeal");
	spStockRedeal->setPosition(170, 527);
	pnlBody->addChild(spStockRedeal);

	Sprite * spStockTemp = Sprite::create("placeBg/StockBk-Temp.png");
	spStockTemp->setName("SpStockTemp");
	spStockTemp->setPosition(302, 527);
	pnlBody->addChild(spStockTemp);

		//foundation
	Sprite * spFoundation0 = Sprite::create("placeBg/FoundationBk.png");
	spFoundation0->setName("SpFoundation0");
	spFoundation0->setPosition(564, 527);
	pnlBody->addChild(spFoundation0);

	Sprite * spFoundation1 = Sprite::create("placeBg/FoundationBk.png");
	spFoundation1->setName("SpFoundation1");
	spFoundation1->setPosition(699, 527);
	pnlBody->addChild(spFoundation1);

	Sprite * spFoundation2 = Sprite::create("placeBg/FoundationBk.png");
	spFoundation2->setName("SpFoundation2");
	spFoundation2->setPosition(833, 527);
	pnlBody->addChild(spFoundation2);

	Sprite * spFoundation3 = Sprite::create("placeBg/FoundationBk.png");
	spFoundation3->setName("SpFoundation3");
	spFoundation3->setPosition(970, 527);
	pnlBody->addChild(spFoundation3);

		//card table
	Sprite * spTable0 = Sprite::create("placeBg/TableauBk.png");
	spTable0->setName("SpTable0");
	spTable0->setPosition(170, 373);
	pnlBody->addChild(spTable0);

	Sprite * spTable1 = Sprite::create("placeBg/TableauBk.png");
	spTable1->setName("SpTable1");
	spTable1->setPosition(302, 373);
	pnlBody->addChild(spTable1);

	Sprite * spTable2 = Sprite::create("placeBg/TableauBk.png");
	spTable2->setName("SpTable2");
	spTable2->setPosition(433, 373);
	pnlBody->addChild(spTable2);

	Sprite * spTable3 = Sprite::create("placeBg/TableauBk.png");
	spTable3->setName("SpTable3");
	spTable3->setPosition(564, 373);
	pnlBody->addChild(spTable3);

	Sprite * spTable4 = Sprite::create("placeBg/TableauBk.png");
	spTable4->setName("SpTable4");
	spTable4->setPosition(699, 373);
	pnlBody->addChild(spTable4);

	Sprite * spTable5 = Sprite::create("placeBg/TableauBk.png");
	spTable5->setName("SpTable5");
	spTable5->setPosition(833, 373);
	pnlBody->addChild(spTable5);

	Sprite * spTable6 = Sprite::create("placeBg/TableauBk.png");
	spTable6->setName("SpTable6");
	spTable6->setPosition(970, 373);
	pnlBody->addChild(spTable6);

	//pnl menu bar
	Layout * PnlMenuBar = Layout::create();
	PnlMenuBar->setName("PnlMenuBar");
	PnlMenuBar->setContentSize(Size(DESIGN_WIDTH_LANDSCAPE, 80));
	scene->addChild(PnlMenuBar);

		//BtnNew
	Button * btnNew = Button::create("btn/btn_new_xx.png", "btn/btn_new_xx_highlighted.png", "", TextureResType::LOCAL);
	btnNew->setName("BtnNew");
	btnNew->setAnchorPoint(Vec2(0, 0.5));
	btnNew->setScale(0.46);
	btnNew->setPosition(Vec2(80, 40));
	PnlMenuBar->addChild(btnNew);

	Text * textNew = Text::create("New Game", "", 48);
	textNew->setScale(0.8717, 1);
	textNew->setPosition(Vec2(191.08, 54));
	btnNew->addChild(textNew);

		//BtnHighScore
	Button * btnHighScore = Button::create("btn/btn_highscores_xx.png", "btn/btn_highscores_xx_highlighted.png", "", TextureResType::LOCAL);
	btnHighScore->setName("BtnHighScore");
	btnHighScore->setAnchorPoint(Vec2(0, 0.5));
	btnHighScore->setScale9Enabled(true);
	btnHighScore->setCapInsets(Rect(90, 15, 15, 15));
	btnHighScore->setContentSize(Size(351, 108));
	btnHighScore->setScale(0.46);
	btnHighScore->setPosition(Vec2(240, 40));
	PnlMenuBar->addChild(btnHighScore);

	Text * textHighScore = Text::create("High Scores", "", 48);
	textHighScore->setScale(0.8717, 1);
	textHighScore->setPosition(Vec2(222.33, 56.97));
	btnHighScore->addChild(textHighScore);

		//BtnSettings
	Button * btnSettings = Button::create("btn/btn_setttings_xx.png", "btn/btn_settings_xx_highlighted.png", "", TextureResType::LOCAL);
	btnSettings->setName("BtnSettings");
	btnSettings->setAnchorPoint(Vec2(0.5, 0.5));
	btnSettings->setScale(0.46);
	btnSettings->setPosition(Vec2(479, 40));
	PnlMenuBar->addChild(btnSettings);

	Text * textSettings = Text::create("Settings", "", 48);
	textSettings->setScale(0.8717, 1);
	textSettings->setPosition(Vec2(177.58, 54));
	btnSettings->addChild(textSettings);

		//BtnUndo
	Button * btnUndo = Button::create("btn/btn_undo_xx.png", "btn/btn_undo_xx_highlighted.png", "", TextureResType::LOCAL);
	btnUndo->setName("BtnUndo");
	btnUndo->setAnchorPoint(Vec2(1.0, 0.5));
	btnUndo->setScale(0.49, 0.46);
	btnUndo->setPosition(Vec2(970, 39.73));
	PnlMenuBar->addChild(btnUndo);

	ImageView * imgUndo = ImageView::create("btn/Undo-Normal.png", TextureResType::LOCAL);
	imgUndo->setPosition(Vec2(53.88, 54.89));
	btnUndo->addChild(imgUndo);

	Text * textUndo = Text::create("Undo", "", 48);
	textUndo->setScale(0.8717, 1);
	textUndo->setPosition(Vec2(148.48, 54));
	btnUndo->addChild(textUndo);

		//BtnHint
	Button * btnHint = Button::create("btn/btn_hint_xx.png", "btn/btn_hint_xx_highlighted.png", "", TextureResType::LOCAL);
	btnHint->setName("BtnHint");
	btnHint->setAnchorPoint(Vec2(1.0, 0.5));
	btnHint->setScale(0.46, 0.46);
	btnHint->setPosition(Vec2(1062.00, 39.2));
	PnlMenuBar->addChild(btnHint);

	Text * textHint = Text::create("Hint", "", 48);
	textHint->setScale(0.8717, 1);
	textHint->setPosition(Vec2(114.90, 54));
	btnHint->addChild(textHint);

	//Pnl Score Bar
	Layout * PnlScoreBar = Layout::create();
	PnlScoreBar->setName("PnlScoreBar");
	PnlScoreBar->setPosition(Vec2(0, 590));
	PnlScoreBar->setContentSize(Size(DESIGN_WIDTH_LANDSCAPE, 50));
	scene->addChild(PnlScoreBar);

		//text time
		Text * textTime = Text::create("09:00", "", 48);
		textTime->setName("TxtTime");
		textTime->setScale(0.46);
		textTime->setAnchorPoint(Vec2(0, 0.5));
		textTime->setPosition(Vec2(80, 25));
		PnlScoreBar->addChild(textTime);

		//text time
		Text * textMoves = Text::create("Moves: 57", "", 48);
		textMoves->setName("TxtMoves");
		textMoves->setScale(0.46);
		textMoves->setAnchorPoint(Vec2(1.0, 0.5));
		textMoves->setPosition(Vec2(754.46, 25));
		PnlScoreBar->addChild(textMoves);

		//text score
		Text * textScore = Text::create("Score: -250$", "", 48);
		textScore->setName("TxtScore");
		textScore->setScale(0.46);
		textScore->setAnchorPoint(Vec2(1.0, 0.5));
		textScore->setPosition(Vec2(1062, 25));
		PnlScoreBar->addChild(textScore);

		//text score
		Text * textElapsedTime = Text::create("Time: 1000", "", 48);
		textElapsedTime->setName("TxtElapsedTime");
		textElapsedTime->setScale(0.46);
		textElapsedTime->setAnchorPoint(Vec2(1.0, 0.5));
		textElapsedTime->setPosition(Vec2(900.48, 25));
		PnlScoreBar->addChild(textElapsedTime);

		//game won image
		ImageView *imgWon = ImageView::create("placeBg/GameWon.png", TextureResType::LOCAL);
		imgWon->setName("ImgGameWon");
		imgWon->setVisible(false);
		imgWon->setPosition(Vec2(530, 25));
		PnlScoreBar->addChild(imgWon);

		//game not winnable image
		ImageView *imgNotWin = ImageView::create("placeBg/NotWinnable.png", TextureResType::LOCAL);
		imgNotWin->setName("ImgNoWin");
		imgNotWin->setVisible(false);
		imgNotWin->setPosition(Vec2(530, 25));
		PnlScoreBar->addChild(imgNotWin);

	return scene;
}

void GameScene::loadScene()
{
/*
	std::string sceneName = "MainSceneLandscape.csb";
	if( Utils::GetOrientation() )	//portrait
		sceneName = "MainScene.csb";

	log(" ************   load scene   ****************\n");
	log(sceneName.c_str());

    auto scene = CSLoader::createNode(sceneName);
    addChild(scene);
*/
	/**
	 *	@since 2015/05/04
	 *	convert cocos studio UI to code.
	 */

	auto scene = initUI();
	addChild(scene);

    pnlBg = (Layout*)scene->getChildByName("PnlBg");
    TextureCache::getInstance()->removeTextureForKey(Utils::share()->getWallpaperPath());
    pnlBg->setBackGroundImage(Utils::share()->getWallpaperPath());

    pnlScoreBar = (Layer*)scene->getChildByName("PnlScoreBar");
    pnlBody = (Layer*)scene->getChildByName("PnlBody");
    pnlMenuBar = (Layer*)scene->getChildByName("PnlMenuBar");

    //in the score bar
    m_lblTime = (Text*)pnlScoreBar->getChildByName("TxtTime");
    m_lblTime->setVisible(false);
    m_lblMoves = (Text*)pnlScoreBar->getChildByName("TxtMoves");
    m_lblScore = (Text*)pnlScoreBar->getChildByName("TxtScore");
    m_lblElapsedTime = (Text*)pnlScoreBar->getChildByName("TxtElapsedTime");
    m_imgGameWon = (ImageView*)pnlScoreBar->getChildByName("ImgGameWon");
    m_imgNotWin = (ImageView*)pnlScoreBar->getChildByName("ImgNoWin");
    
    //in the PnlBody
    spStockNoRedeal = (Sprite*)pnlBody->getChildByName("SpStockNoRedeal");
    Sprite* spStockRedeal = (Sprite*)pnlBody->getChildByName("SpStockRedeal");
    Sprite* spStockTemp = (Sprite*)pnlBody->getChildByName("SpStockTemp");
    spStockTemp->setLocalZOrder(ZORDER_TEMP);
    
    Utils::share()->setCardSize(spStockTemp->getContentSize());
    
    stockRedeal.setBaseSprite(spStockRedeal);
    stockRedeal.setShadowMask();
    
    stockTemp.setBaseSprite(spStockTemp);
    
    int i;
    for( i = 0; i < CARDFD_CNT; i ++ )
    {
        std::string name = "SpFoundation" + std::to_string(i);
        Sprite* spFoundation = (Sprite*)pnlBody->getChildByName(name);
        
        //init card foundations
        foundations[i].setBaseSprite(spFoundation);
        foundations[i].setNo(i);
        foundations[i].setShadowMask();
    }
    
    for( i = 0; i < CARDTB_CNT; i ++ )
    {
        std::string name = "SpTable" + std::to_string(i);
        Sprite* spTable = (Sprite*)pnlBody->getChildByName(name);
        
        //create cardTable
        cardTables[i].setBaseSprite(spTable);
        cardTables[i].setNo(i);
     }
    
    //moving card group
    Sprite* spMoving = Sprite::create("placeBg/StockBk-Temp.png");
    spMoving->setLocalZOrder(ZORDER_MOVE);
    pnlBody->addChild(spMoving);
    mvCardGroup.setBaseSprite(spMoving);
    
    //in the PnlMenuBar
    btnNew = (Button*)pnlMenuBar->getChildByName("BtnNew");
    btnSettings = (Button*)pnlMenuBar->getChildByName("BtnSettings");
    btnHighScores = (Button*)pnlMenuBar->getChildByName("BtnHighScore");
    btnHint = (Button*)pnlMenuBar->getChildByName("BtnHint");
    btnUndo = (Button*)pnlMenuBar->getChildByName("BtnUndo");
    
//    btnNew->setPressedActionEnabled(true);
//    btnSettings->setPressedActionEnabled(true);
//    btnHighScores->setPressedActionEnabled(true);
//    btnHint->setPressedActionEnabled(true);
//    btnUndo->setPressedActionEnabled(true);
    
    btnNew->addTouchEventListener(CC_CALLBACK_2(GameScene::touchCallback, this));
    btnSettings->addTouchEventListener(CC_CALLBACK_2(GameScene::touchCallback, this));
    btnHighScores->addTouchEventListener(CC_CALLBACK_2(GameScene::touchCallback, this));
    btnHint->addTouchEventListener(CC_CALLBACK_2(GameScene::touchCallback, this));
    btnUndo->addTouchEventListener(CC_CALLBACK_2(GameScene::touchCallback, this));
    
    onGameSetting();
    if( Utils::share()->getRightHandMode() )
    	setRightHandMode();

    if( Utils::GetKeepGame() )
    {
    	m_nMoves = Utils::GetMoves();
        m_nHint = Utils::GetHintCount();
    	m_nScore = Utils::GetScore();
    	m_nInitScore = Utils::GetInitialScore();
    	m_nElapsedTime = Utils::GetElapsedTime();

    	setScoreData();
        setHint();

    	//undo cardTables
		for( int i = 0; i < CARDTB_CNT; i ++ )
			cardTables[i].restoreState(m_nMoves);

		//undo foundations
		for( int i = 0; i < CARDFD_CNT; i ++ )
			foundations[i].restoreState(m_nMoves);

		stockTemp.restoreState(m_nMoves);

		stockRedeal.restoreState(m_nMoves);

		m_bIsRunning = true;
		m_bTouchAgain = true;
    }
    else
    	onRestartGame(Utils::GetNewGame());

    //show current time
	setCurrentTime(1.0);
	schedule(schedule_selector(GameScene::setCurrentTime), 1.0);

    Utils::SetKeepGame(true);
}

void GameScene::onGameSetting()
{
    m_lShimmerTime = Utils::GetShimmerTime();
    
    m_nDrawMode = Utils::share()->getDrawMode();
	m_nScoreSetting = Utils::share()->getSettingScoring();
	m_bRedeal = Utils::share()->isRedealPossible();
	m_nRedealCount = Utils::share()->getRedealCount();

	if( m_bRedeal == false )
		stockRedeal.setNoRedeal();

	m_bGameSolver = Utils::share()->getGameSolver();
	m_bAutoComplete = Utils::share()->getAutoComplete();

	//status bar
	Utils::SetInitialScore(0);
	if( m_nScoreSetting == 0 )	 //none
	{
		m_lblScore->setVisible(false);
		m_lblMoves->setPositionX(m_lblElapsedTime->getPositionX());
		m_lblElapsedTime->setPositionX(m_lblScore->getPositionX());
	}
	else if( m_nScoreSetting == 1 )		//standard
		Utils::SetInitialScore(0);
	else if( m_nScoreSetting == 2 )		//vegas
		Utils::SetInitialScore(-52);
	else if( m_nScoreSetting == 3 )		//cumulative
		Utils::SetInitialScore(Utils::GetInitialScore()-52);

	if( Utils::share()->isTimedGame() == false )
	{
		m_lblElapsedTime->setVisible(false);
		m_lblMoves->setPositionX(m_lblElapsedTime->getPositionX());
	}

	if( Utils::share()->isShowMoves() == false )
		m_lblMoves->setVisible(false);
}

void GameScene::resetGame()
{
    m_bIsRunning = false;
    m_bIsTimeElapse = false;
    
    for( int i = 0; i < CARDTB_CNT; i ++ )
    {
        cardTables[i].reset();
        cardTables[i].getBaseSprite()->removeAllChildren();
    }
    
    for( int i = 0; i < CARDFD_CNT; i ++ )
    {
        foundations[i].reset();
        foundations[i].hideShadowMask();
    }
    
    stockTemp.reset();
    
    stockRedeal.reset();
    stockRedeal.hideShadowMask();
    if( m_nDrawMode == DRAW_MODE_ONE )
    	m_nRedealCount = 1;
    else
       	m_nRedealCount = 3;

    Utils::share()->setRedealCount(m_nRedealCount);

    m_bRedeal = Utils::share()->isRedealPossible();
    
    //reset score values
    m_nMoves = 0;
    m_nHint = HINT_COUNT;
    m_nInitScore = Utils::GetInitialScore();
    m_nScore = m_nInitScore;
    m_nElapsedTime = 0;
    
    setScoreData();
    setHint();
}

void GameScene::gameWasWin()
{
    m_bIsRunning = false;
    m_bIsTimeElapse = false;
    
    m_nFDVisitNumber = 0;

    schedule(schedule_selector(GameScene::playGameWonAction), GAMEWON_FD_DURATION);		//ipad version style
    //schedule(schedule_selector(GameScene::onAnimateCard), GAMEWIN_DURATION);	//iphone version style
}

void GameScene::onCongratulate(float dt)
{
    Utils::playGameWon();

    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	{
    	Utils::share()->saveGameState();
		AndroidBridge::getInstance()->didGameWon(getGameInformation(true), getGameResult(true));
	}
}

void GameScene::saveGameState()
{
    //save score data
    Utils::SetScoreData(m_nMoves, m_nScore, m_nElapsedTime);
    
    int i;
    for( i = 0; i < CARDTB_CNT; i ++ )
        cardTables[i].saveState(m_nMoves);
    
    for( i = 0; i < CARDFD_CNT; i ++ )
        foundations[i].saveState(m_nMoves);
    
    stockTemp.saveState(m_nMoves);
    
    stockRedeal.saveState(m_nMoves);
}

#pragma mark - card action

void GameScene::playGameWonAction(float dt)
{
	int i = 0;
	while( foundations[m_nFDVisitNumber].getCardCount() > 0 )
	{
		int nCardNumber = foundations[m_nFDVisitNumber].getLastCard()->getCardNumber();
		foundations[m_nFDVisitNumber].popCard();

		Card * card = Card::create();
		card->initWithNo(nCardNumber);
		card->setPosition(foundations[m_nFDVisitNumber].getBaseSprite()->getPosition());
		card->showFace();
		card->setLocalZOrder(ZORDER_WON - i);
		pnlBody->addChild(card);

		int range = int(cardTables[CARDTB_CNT-1].getBaseSprite()->getPosition().x - cardTables[0].getBaseSprite()->getPosition().x);
		int xpos = rand() % range;
		float x = cardTables[0].getBaseSprite()->getPosition().x + xpos;

		Vec2 desPos = Vec2( x, card->getContentSize().height / 2.0);

		//move action
		DelayTime * delayAction = DelayTime::create(GAMEWON_MOVEDOWN_DELAY*i);
		auto moveAction = MoveTo::create(GAMEWIN_DURATION, desPos);
		CallFunc * callback = CallFunc::create(CC_CALLBACK_0(GameScene::onArrivedAtBottom, this, card));
		Sequence * seq = Sequence::create(delayAction, moveAction, callback, NULL);
		card->runAction(seq);

		i ++;
	}

	foundations[m_nFDVisitNumber].hideShadowMask();

	m_nFDVisitNumber ++;
	if( m_nFDVisitNumber == 4 )
	{
		unschedule(schedule_selector(GameScene::playGameWonAction));
		scheduleOnce(schedule_selector(GameScene::onCongratulate), GAMEWON_END_DURATION);
	}
}

void GameScene::onAnimateCard(float dt)
{
    if( foundations[m_nFDVisitNumber].getCardCount() > 0 )  //FOR TEST
    {
        int nCardNumber = foundations[m_nFDVisitNumber].getLastCard()->getCardNumber();
        foundations[m_nFDVisitNumber].popCard();
        if( foundations[m_nFDVisitNumber].getCardCount() == 0 )
            foundations[m_nFDVisitNumber].hideShadowMask();
        
        Card * card = Card::create();
        card->initWithNo(nCardNumber);
        card->setPosition(foundations[m_nFDVisitNumber].getBaseSprite()->getPosition());
        card->showFace();
        pnlBody->addChild(card);

        int range = int(cardTables[CARDTB_CNT-1].getBaseSprite()->getPosition().x - cardTables[0].getBaseSprite()->getPosition().x);
        int xpos = rand() % range;
        float x = cardTables[0].getBaseSprite()->getPosition().x + xpos;
        
        Vec2 desPos = Vec2( x, card->getContentSize().height / 2.0);
        
        //move action
        auto moveAction = MoveTo::create(GAMEWIN_DURATION, desPos);
        CallFunc * callback = CallFunc::create(CC_CALLBACK_0(GameScene::onArrivedAtBottom, this, card));
        Sequence * seq = Sequence::create(moveAction, callback, NULL);
        card->runAction(seq);
    }
    else
    {
        unschedule(schedule_selector(GameScene::onAnimateCard));
        onCongratulate(0);
    }
    
    m_nFDVisitNumber ++;
    if( m_nFDVisitNumber == 4 )
        m_nFDVisitNumber = 0;
}

void GameScene::onArrivedAtBottom(Card* sender)
{
    onThrowCard(sender->getCardNumber(), sender->getPosition());
    sender->removeFromParent();
}

void GameScene::onThrowCard(int nCardNumber, Vec2 pos)
{
    int angleMax = 160;
    //float diagonalDis = sqrtf(DESIGN_HEIGHT * DESIGN_HEIGHT + DESIGN_WIDTH * DESIGN_WIDTH) + 100;	//portrait
    float diagonalDis = sqrtf(DESIGN_HEIGHT_LANDSCAPE * DESIGN_HEIGHT_LANDSCAPE + DESIGN_WIDTH_LANDSCAPE * DESIGN_WIDTH_LANDSCAPE) + 100;	//landscape
    Vec2 desPos;
    
	Card* card = Card::create();
	card->initWithNo(nCardNumber);
	card->showFace();
	card->hideCardMask();
	card->setPosition(pos);
	pnlBody->addChild(card);

	float angle = (rand() % angleMax + 10.0) * M_PI / 180.0;
	float x = pos.x + cos(angle) * diagonalDis;
	float y = diagonalDis * sin(angle);

	desPos = Vec2(x, y);

	//animation
	auto moveAction = MoveTo::create(GAMEWIN_THROW_DURATION, desPos);
	float rotateAngle = 90 + rand() % 270;
	auto spawnAction = Spawn::create(moveAction, RotateBy::create(GAMEWIN_THROW_DURATION, rotateAngle), NULL);
	DelayTime* delay = DelayTime::create( (rand() % 80) / 100.0 * 0.1 );

	CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::removeFromParent, card));
	Sequence * seq = Sequence::create(delay, spawnAction, callback, NULL);
	auto easeAction = EaseIn::create(seq, GAMEWIN_THROW_DURATION);

	card->runAction(easeAction);
}

void GameScene::onThrowCards(int nCardNumber, Vec2 pos)
{
    int angleMax = 160;
    float diagonalDis = sqrtf(DESIGN_HEIGHT * DESIGN_HEIGHT + DESIGN_WIDTH * DESIGN_WIDTH) + 100;
    Vec2 desPos;

    for( int i = 0; i < CARDS_NUM; i ++ )
    {
        Card* card = Card::create();
        card->initWithNo(nCardNumber);
        card->showFace();
        card->hideCardMask();
        card->setPosition(pos);
        pnlBody->addChild(card);

        if( i % 3 == 0 )
        {
            float angle = (rand() % angleMax + 10.0) * M_PI / 180.0;
            float x = pos.x + cos(angle) * diagonalDis;
            float y = diagonalDis * sin(angle);

            desPos = Vec2(x, y);
        }

        //animation
        auto moveAction = MoveTo::create(GAMEWIN_THROW_DURATION, desPos);
        float rotateAngle = 90 + rand() % 270;
        auto spawnAction = Spawn::create(moveAction, RotateBy::create(GAMEWIN_THROW_DURATION, rotateAngle), NULL);
        DelayTime* delay = DelayTime::create( (rand() % 80) / 100.0 * 0.1 );

        CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::removeFromParent, card));
        Sequence * seq = Sequence::create(delay, spawnAction, callback, NULL);
        auto easeAction = EaseIn::create(seq, GAMEWIN_THROW_DURATION);

        card->runAction(easeAction);
    }
}

void GameScene::doArrangeCards(float dt)
{
    if( m_nNextTable < CARDTB_CNT )
    {
        int nCardNumber = stockTemp.popCard();
        if( nCardNumber == -1 )
            return;
        
        if( m_nTableNo == CARDTB_CNT )
        {
            m_nTableNo = m_nNextTable;
            m_nNextTable ++;
        }
        
        Vec2 desPos = cardTables[m_nTableNo].getLastCardPosition();
        
        Card * card = Card::create();
        card->initWithNo(nCardNumber);
        card->setPosition(stockTemp.getBaseSprite()->getPosition());
        card->setLocalZOrder(ZORDER_MOVE);
        pnlBody->addChild(card);
        
        //set last card flag
        if( m_nNextTable == CARDTB_CNT )    //last card
            card->setRemark(REMARK_LAST);
        
        //move action
        //DelayTime * delayTime = DelayTime::create(MOVE_DELAY);
        auto moveAction = MoveTo::create(MOVE_DURATION, desPos);
        CallFunc * callback = CallFunc::create(CC_CALLBACK_0(GameScene::onFinishedMoveAction, this, card));
        Sequence * seq = Sequence::create(moveAction, callback, NULL);
        card->setTag(m_nTableNo);
        card->runAction(seq);
        
        m_nTableNo ++;
    }
    else
    {
        //arrangement to card tables is finished.
        unschedule(schedule_selector(GameScene::doArrangeCards));
        
        //slide animation to stock redeal place.
        m_nStartNumber = 0;
        schedule(schedule_selector(GameScene::onSlideToRedealStock), SLIDE_LEFT_DELAY);
    }
}

void GameScene::onSlideToRedealStock(float dt)
{
    if( stockTemp.getCardCount() )
    {
        int nCardNumber = stockTemp.popCard();
        Vec2 desPos = stockRedeal.getBaseSprite()->getPosition();
        
        Card * card = Card::create();
        card->initWithNo(nCardNumber);
        card->setPosition(stockTemp.getBaseSprite()->getPosition());
        pnlBody->addChild(card);
        
        if( stockTemp.getCardCount() == 0 )
            card->setRemark(REMARK_LAST);
        
        //move action
        float LIM = 35;
        float rate = (LIM - m_nStartNumber) / LIM;
        auto moveAction = MoveTo::create(SLIDE_LEFT_DURATION / rate, desPos);
        CallFunc * callback = CallFunc::create(CC_CALLBACK_0(GameScene::onFinishedLeftSlideAction, this, card));
        Sequence * seq = Sequence::create(moveAction, callback, NULL);
        card->runAction(seq);
        
        m_nStartNumber ++;
    }
    else
    {
        //slide cards from stockTemp to stockRedeal
        unschedule(schedule_selector(GameScene::onSlideToRedealStock));
    }
}

void GameScene::onSlideFlipToTempStock()
{
    int nCardNumber = stockRedeal.popCard();
    if( stockRedeal.getCardCount() == 0 )
        stockRedeal.hideShadowMask();
    
    Card * card = Card::create();
    card->initWithNo(nCardNumber);
    card->setPosition(stockRedeal.getBaseSprite()->getPositionX(), stockRedeal.getBaseSprite()->getPositionY());
    card->setLocalZOrder(ZORDER_MOVE);
    pnlBody->addChild(card);
    card->setRemark(REMARK_LAST);
    
    //move and flip action.
    Vec2 desPos = Vec2(stockTemp.getBaseSprite()->getPositionX(), stockTemp.getBaseSprite()->getPositionY());
    auto moveAction = MoveTo::create(SLIDE_FLIP_DURATION, desPos);
    
    ScaleTo *act1= ScaleTo::create(SLIDE_FLIP_DURATION / 2.0, 0, 1);
    CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::onReplaceCardImage, card));
    ScaleTo *act2= ScaleTo::create(SLIDE_FLIP_DURATION / 2.0, 1, 1);
    Sequence* flipAction = Sequence::create(act1, callback, act2, NULL);
    
    auto * spwan = Spawn::create(moveAction, flipAction, NULL);
    
    CallFunc * actionFinished = CallFunc::create(CC_CALLBACK_0(GameScene::onFinishedRightSlideFlipAction, this, card));
    Sequence * seq = Sequence::create(spwan, actionFinished, NULL);
    
    card->runAction(seq);
}

void GameScene::onThreeSlideFlipToTempStock()
{
    int nCardsToSend = stockRedeal.getCardCount();
    if( nCardsToSend > 3 )
        nCardsToSend = 3;
    
    int nStockTempCardCnt = stockTemp.getCardCount();
    int nDesPosition[3] = {0, 1, 2};    //nCardsToSend == 3
    int nFold = 2;
    if( nCardsToSend == 2 )
    {
        if( nStockTempCardCnt >= 3 )
        {
            nDesPosition[0] = 1;
            nDesPosition[1] = 2;
        }
    }
    else if( nCardsToSend == 1 )
    {
        if( nStockTempCardCnt >= 3 )
        {
            nDesPosition[0] = 2;
            
            nFold = 1;
        }
    }

    stockTemp.foldCards(nFold);
    
    Vec2 desPos0 = Vec2(stockTemp.getBaseSprite()->getPositionX(), stockTemp.getBaseSprite()->getPositionY());
    float deltaX = stockTemp.getBaseSprite()->getContentSize().width / 3.0;
    Vec2 desPositions[3] = { desPos0, desPos0 + Vec2(deltaX, 0), desPos0 + Vec2(deltaX * 2, 0) };
    
    float fRatio[3] = {0.4, 0.7, 0.8};	//iphone {0.4, 0.6, 0.65};
    for( int i = 0; i < nCardsToSend; i ++ )
    {
        int nCardNumber = stockRedeal.popCard();
        if( stockRedeal.getCardCount() == 0 )
            stockRedeal.hideShadowMask();
        
        Card * card = Card::create();
        card->initWithNo(nCardNumber);
        card->setPosition(stockRedeal.getBaseSprite()->getPositionX(), stockRedeal.getBaseSprite()->getPositionY());
        card->setLocalZOrder(ZORDER_MOVE);
        pnlBody->addChild(card);
        if( i == nCardsToSend - 1 )
            card->setRemark(REMARK_LAST);
        card->setTag(nDesPosition[i]);
        
        float fDuration = SLIDE_FLIP_DURATION; // * fRatio[i];
        
        //move and flip action.
        Vec2 desPos = desPositions[i];
        auto moveAction = MoveTo::create(fDuration, desPos);
        
        ScaleTo *act1= ScaleTo::create(fDuration / 2.0, 0, 1);
        CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::onReplaceCardImage, card));
        ScaleTo *act2= ScaleTo::create(fDuration / 2.0, 1, 1);
        Sequence* flipAction = Sequence::create(act1, callback, act2, NULL);
        
        auto * spwan = Spawn::create(moveAction, flipAction, NULL);
        
        CallFunc * actionFinished = CallFunc::create(CC_CALLBACK_0(GameScene::onFinishedThreeRightSlideFlipAction, this, card));
        Sequence * seq = Sequence::create(spwan, actionFinished, NULL);
        
        card->runAction(seq);
    }
}

void GameScene::setRedealState()
{
	if( m_bRedeal && m_nScoreSetting >= 2 )	//vegas mode
	{
		m_nRedealCount --;
		if( m_nRedealCount == 0 )
		{
			m_bRedeal = false;
			//change background
			stockRedeal.setNoRedeal();
		}

		Utils::share()->setRedealCount(m_nRedealCount);
	}
}

void GameScene::onRedeal()
{
    for( int i = 0; stockTemp.getCardCount() > 0; i ++ )
    {
        int nCardNumber = stockTemp.popCard();
        
        Card * card = Card::create();
        card->initWithNo(nCardNumber);
        card->setPosition(stockTemp.getBaseSprite()->getPosition());
        card->setLocalZOrder(ZORDER_MOVE);
        card->showFace();
        card->hideCardMask();
        pnlBody->addChild(card);
        if( stockTemp.getCardCount() == 0 )
            card->setRemark(REMARK_LAST);
        
        //move and flip action.
        Vec2 desPos = Vec2(stockRedeal.getBaseSprite()->getPosition());
        auto moveAction = MoveTo::create(SLIDE_FLIP_DURATION, desPos);
        
        ScaleTo *act1= ScaleTo::create(SLIDE_FLIP_DURATION / 2.0, 0, 1);
        CallFunc * callback = CallFunc::create(CC_CALLBACK_0(Card::onReplaceCardImage, card));
        ScaleTo *act2= ScaleTo::create(SLIDE_FLIP_DURATION / 2.0, 1, 1);
        Sequence* flipAction = Sequence::create(act1, callback, act2, NULL);
        
        auto * spwan = Spawn::create(moveAction, flipAction, NULL);
        
        CallFunc * showMask = CallFunc::create(CC_CALLBACK_0(Card::showCardMask, card));
        CallFunc * actionFinished = CallFunc::create(CC_CALLBACK_0(GameScene::onFinishedLeftSlideAndFlipAction, this, card));
        DelayTime * delayTime = DelayTime::create(MOVE_TARGET_DELAY * i / 5.0);
        Sequence * seq = Sequence::create(delayTime, showMask, spwan, actionFinished, NULL);
        
        card->runAction(seq);
    }
}

void GameScene::onMergeCardsToCardTable(int nCardTB, bool isBack)
{
    int cnt = mvCardGroup.getCardCount();
    int i;
    Vec2 lastPosition = cardTables[nCardTB].getLastCardPosition();
    float fDis = Utils::GetFaceCardDistance();
    
    for( i = 0; i < cnt; i ++ )
    {
        int nCardNumber = mvCardGroup.getCardNumberFromIndex(i);
        
        //create moving card
        Card * card = Card::create();
        card->initWithNo(nCardNumber);
        card->showFace();
        card->setLocalZOrder(ZORDER_MOVE);
        card->setPosition(mvCardGroup.getCardPosition(i));
        card->setTag(nCardTB);
        
        if( i == cnt - 1 )
        {
            card->setRemark(REMARK_LAST);
            if( isBack )
                card->setRemark(REMARK_BACK);
        }
        
        pnlBody->addChild(card);
        
        Vec2 desPos = lastPosition - Vec2(0, fDis * i);
        auto moveAction = MoveTo::create(MOVE_TARGET_DURATION, desPos);
        
        auto * spwan = Spawn::create(moveAction, NULL);
        CallFunc * actionFinished = CallFunc::create(CC_CALLBACK_0(GameScene::onArrivedMergeCardToCardTable, this, card));
        
        DelayTime * delayTime = DelayTime::create(MOVE_TARGET_DELAY * i);
        Sequence * seq = Sequence::create(delayTime, spwan, actionFinished, NULL);
        
        card->runAction(seq);
    }
    
    mvCardGroup.reset();
    mvCardGroup.clearRotation();
}

void GameScene::onMergeCardToFoundation(int nFD, bool isBack)
{
    int i = 0;
    int nCardNumber = mvCardGroup.getCardNumberFromIndex(i);
    
    //create moving card
    Card * card = Card::create();
    card->initWithNo(nCardNumber);
    card->showFace();
    card->setLocalZOrder(ZORDER_MOVE);
    card->setPosition(mvCardGroup.getCardPosition(i));
    card->setTag(nFD);
    pnlBody->addChild(card);
    card->setRemark(REMARK_LAST);
    if( isBack )
        card->setRemark(REMARK_BACK);
    
    Vec2 desPos = foundations[nFD].getBaseSprite()->getPosition();
    auto moveAction = MoveTo::create(MOVE_TARGET_DURATION, desPos);
    
    CallFunc * actionFinished = CallFunc::create(CC_CALLBACK_0(GameScene::onArrivedMergeCardToFoundation, this, card));
    
    Sequence * seq = Sequence::create(moveAction, actionFinished, NULL);
    
    card->runAction(seq);
    
    mvCardGroup.reset();
    mvCardGroup.clearRotation();
}

void GameScene::onMoveHintCard(int nFD, int nDestCardTBNo, int nCardTBNo)
{
    int i = 0;
    int nCardNumber = mvCardGroup.getCardNumberFromIndex(i);
    
    //create moving card
    Card * card = Card::create();
    card->initWithNo(nCardNumber);
    card->showFace();
    card->setLocalZOrder(ZORDER_MOVE);
    card->setPosition(mvCardGroup.getCardPosition(i));
    card->setTag(nCardTBNo);
    pnlBody->addChild(card);
    
    Vec2 desPos;
    if( nFD != -1 ) //to foundation
        desPos = foundations[nFD].getBaseSprite()->getPosition();
    else    //to cardTable
        desPos = cardTables[nDestCardTBNo].getLastCardPosition();
    
    auto moveAction = MoveTo::create(MOVE_TARGET_DURATION, desPos);
    CallFunc * glowAction = CallFunc::create(CC_CALLBACK_0(Card::runHintGlowEffect, card));
    DelayTime* delayAction = DelayTime::create(HINT_DURATION*4);
    auto comebackAction = MoveTo::create(MOVE_TARGET_DURATION, mvCardGroup.getCardPosition(i));
    CallFunc * actionFinished = CallFunc::create(CC_CALLBACK_0(GameScene::onFinishedHintCardMoving, this, card));
    
    Sequence * seq = Sequence::create(moveAction, glowAction, delayAction,comebackAction, actionFinished, NULL);
    
    card->runAction(seq);
    
    mvCardGroup.reset();
    mvCardGroup.clearRotation();
}

void GameScene::onFinishedHintCardMoving(Card* sender)
{
    int nCardTBNo = sender->getTag();
    sender->removeFromParent();
    
    if( nCardTBNo == -1 ) //stockTemp
        stockTemp.getLastCard()->setVisible(true);
    else
        cardTables[nCardTBNo].getLastCard()->setVisible(true);
    m_bHintMoving = false;
}

void GameScene::onMergeCardToStockTemp()
{
    int i = 0;
    int nCardNumber = mvCardGroup.getCardNumberFromIndex(i);
    
    //create moving card
    Card * card = Card::create();
    card->initWithNo(nCardNumber);
    card->showFace();
    card->setLocalZOrder(ZORDER_MOVE);
    card->setPosition(mvCardGroup.getCardPosition(i));
    pnlBody->addChild(card);
    card->setRemark(REMARK_LAST);
    
    Vec2 desPos = stockTemp.getLastCardPosition();
    auto moveAction = MoveTo::create(MOVE_TARGET_DURATION, desPos);
    
    auto * spwan = Spawn::create(moveAction, NULL);
    CallFunc * actionFinished = CallFunc::create(CC_CALLBACK_0(GameScene::onArrivedMergeCardToStockTemp, this, card));
    
    Sequence * seq = Sequence::create(spwan, actionFinished, NULL);
    
    card->runAction(seq);
    
    mvCardGroup.reset();
    mvCardGroup.clearRotation();
}

void GameScene::onFinishedMoveAction(Card* sender)
{
    int nTableNo = sender->getTag();
    cardTables[nTableNo].setCard(sender->getCardNumber());
    cardTables[nTableNo].getLastCard()->hideCardMask();
    sender->removeFromParent();
    
    //flip cards on top of stockTables.
    if( sender->getRemark() & ( 1 << REMARK_LAST ) )
    {
        saveGameState();
        
        for( int i = 0; i < CARDTB_CNT; i ++ )
        {
            int idx = CARDTB_CNT - i - 1;
            cardTables[idx].runFlipAndShimmerAction(SHIMMER_DEALY * (i + 1));
            cardTables[idx].runMaskFadeEffect();
        }
        
        m_bIsRunning = true;
        m_bTouchAgain = true;
        m_bArranging = false;
    }
}

void GameScene::onFinishedLeftSlideAction(Card* sender)
{
    stockRedeal.setCard(sender->getCardNumber());
    stockRedeal.getLastCard()->hideCardMask();
    sender->removeFromParent();
    
    if( sender->getRemark() & ( 1 << REMARK_LAST ) )
    {
        m_bRedealing = false;
        stockTemp.saveState(m_nMoves);
        stockRedeal.saveState(m_nMoves);
        stockRedeal.runMaskFadeEffect();
    }
    
    setTouchEnable(sender->getRemark());
}

void GameScene::onFinishedRightSlideFlipAction(Card* sender)
{
	int nRemark = sender->getRemark();
	int nCardNumber = sender->getCardNumber();
	sender->removeFromParent();

	if( stockTemp.getCardCount() > 0 )
        stockTemp.getLastCard()->hideCardMask();
    
    stockTemp.setCard(nCardNumber);
    stockTemp.showLastCardFace();
    
    increaseMovesAndScore(0);
    setTouchEnable(nRemark);

    if( stockRedeal.getCardCount() == 0 )
    	setRedealState();
}

void GameScene::onFinishedThreeRightSlideFlipAction(Card* sender)
{
    stockTemp.setThreeCard(sender->getCardNumber(), sender->getTag());
    stockTemp.showLastCardFace();
    
    sender->removeFromParent();
    
    int nRemark = sender->getRemark();

    if( nRemark & ( 1 << REMARK_LAST ))
    {
    	if( stockRedeal.getCardCount() == 0 )
    	    setRedealState();
    	increaseMovesAndScore(0);
    }

    setTouchEnable(nRemark);
}

void GameScene::removeSentCardFromStockTemp()
{
    Card * lastcard = stockTemp.getLastCard();
	if(  lastcard && lastcard->isVisible() == false )
	{
		stockTemp.popCard();
		if( m_nDrawMode == DRAW_MODE_THREE )
		{
			stockTemp.unfoldCards();
		}
		else
		{
			if( stockTemp.getCardCount() > 0 )
				stockTemp.getLastCard()->showCardMask();
		}
	}
}

void GameScene::onFinishedLeftSlideAndFlipAction(Card* sender)
{
    stockRedeal.setCard(sender->getCardNumber());
    stockRedeal.showShadowMask();
    stockRedeal.getLastCard()->hideCardMask();
    sender->removeFromParent();
    
    //rearrange cards on stockRedeal.
    if( sender->getRemark() & ( 1 << REMARK_LAST ) )
    {
        stockRedeal.onRearrangeCards();
        increaseMovesAndScore(-100);
    }
    
    setTouchEnable(sender->getRemark());
}

void GameScene::onArrivedMergeCardToCardTable(Card* sender)
{
    int nCardTable = sender->getTag();
    int nCardNumber = sender->getCardNumber();
    
    sender->removeFromParent();
    
    cardTables[nCardTable].setCard(nCardNumber);
    cardTables[nCardTable].showLastCardFace();
    
    int nRemark = sender->getRemark();
    if( (nRemark & (1<< REMARK_LAST)) && !(nRemark & (1<<REMARK_BACK)) )
    {
        if( m_nFoundation )
        {
        	if( m_nScoreSetting == 1 )	//standard
        		increaseMovesAndScore(-15);
        	else if( m_nScoreSetting >= 2 ) //vegas
        		increaseMovesAndScore(-5);
        }
        else if( m_nfStockTemp )
            increaseMovesAndScore(5);
        else if( m_nCardTable )
            increaseMovesAndScore(0);       //when top card is flip, don't increase moves.

        checkGameComplete(0);
    }
    
    setTouchEnable(nRemark);
}

void GameScene::onArrivedMergeCardToFoundation(Card* sender)
{
    int nFD = sender->getTag();
    int nCardNumber = sender->getCardNumber();
    
    sender->removeFromParent();
    
    if( foundations[nFD].getCardCount() > 0 )
    	foundations[nFD].getLastCard()->hideCardMask();

    foundations[nFD].setCard(nCardNumber);
    foundations[nFD].showLastCardFace();
    
    int nRemark = sender->getRemark();
    if( !(nRemark & (1<<REMARK_BACK)))
    {
        if( m_nFoundation )
            increaseMovesAndScore(0);
        else
            increaseMovesAndScore(10);
    }
    
    setTouchEnable(nRemark);
    
    checkGameComplete(0);

	int i;
	for( i = 0; i < CARDFD_CNT; i ++ )
		if( foundations[i].getCardCount() != 13 )
			break;

	if( i == CARDFD_CNT )
		gameWasWin();
}

void GameScene::onArrivedMergeCardToStockTemp(Card* sender)
{
//    int nCardNumber = sender->getCardNumber();
    
    sender->removeFromParent();
    
//    if( stockTemp.getCardCount() > 0 )
//        stockTemp.getLastCard()->hideCardMask();
//    stockTemp.setCard(nCardNumber);
//    stockTemp.showLastCardFace();
  
    stockTemp.getLastCard()->setVisible(true);
    
    int nRemark = sender->getRemark();
    
    setTouchEnable(nRemark);
}

void GameScene::setTouchEnable(int nRemark)
{
    if( nRemark & ( 1 << REMARK_LAST ) )
        m_bTouchAgain = true;
}

void GameScene::setMovingCards(int nCardTBNumber, int st)
{
    //set moving cards to mvCardGroup
    Vec2 position = cardTables[nCardTBNumber].getCardPosition(st);
    mvCardGroup.getBaseSprite()->setPosition(position);
    
    int cnt = cardTables[nCardTBNumber].getCardCount();
    int i;
    for( i = st; i < cnt; i ++ )
    {
        int cardNumber = cardTables[nCardTBNumber].getCardNumberFromIndex(i);
        mvCardGroup.setCard(cardNumber);
        mvCardGroup.showLastCardFace();
    }
    
    //remove selected cards from cardTable
    cardTables[nCardTBNumber].removeSelectedCards(st);
}

void GameScene::setHintMoveCard(int nCardTBNumber)
{
    //set moving cards to mvCardGroup
    Vec2 position;
    if( nCardTBNumber == -1) //stockTemp
        position = stockTemp.getLastCardPosition();
    else    //card table
    {
        int idx = cardTables[nCardTBNumber].getCardCount()-1;
        position = cardTables[nCardTBNumber].getCardPosition(idx);
    }
    mvCardGroup.getBaseSprite()->setPosition(position);
    
    int cardNumber;
    if( nCardTBNumber == -1 ) //stockTemp
        cardNumber = stockTemp.getLastCard()->getCardNumber();
    else    //card table
        cardNumber = cardTables[nCardTBNumber].getCardNumberFromIndex(cardTables[nCardTBNumber].getCardCount()-1);
    mvCardGroup.setCard(cardNumber);
    mvCardGroup.showLastCardFace();
    
    m_bHintMoving = true;
    if( nCardTBNumber == -1 ) //stockTemp
        stockTemp.getLastCard()->setVisible(false);
    else
        cardTables[nCardTBNumber].getLastCard()->setVisible(false);
}

void GameScene::setMovingCardsFromFoundation(int nFD)
{
    Vec2 position = foundations[nFD].getBaseSprite()->getPosition();
    mvCardGroup.getBaseSprite()->setPosition(position);
    
    int cardNumber = foundations[nFD].getLastCard()->getCardNumber();
    mvCardGroup.setCard(cardNumber);
    mvCardGroup.showLastCardFace();
    
    //remove selected card from current foundation.
    foundations[nFD].popCard();
    if( foundations[nFD].getCardCount() == 0 )
        foundations[nFD].hideShadowMask();
}

void GameScene::setMovingCardsFromStockTemp()
{
    Vec2 position = stockTemp.getLastCardPosition();
    mvCardGroup.getBaseSprite()->setPosition(position);
    
    int cardNumber = stockTemp.getLastCard()->getCardNumber();
    mvCardGroup.setCard(cardNumber);
    mvCardGroup.showLastCardFace();
    
    //remove selected card from current stockTemp
    stockTemp.popCard();
    if( stockTemp.getCardCount() > 0 )
    	stockTemp.getLastCard()->showCardMask();
}

void GameScene::setTouchMovingCardsFromStockTemp()
{
    Vec2 position = stockTemp.getLastCardPosition();
    m_pTouchDelta = position - m_pTouchBegin;
    mvCardGroup.getBaseSprite()->setPosition(position);
    
    int cardNumber = stockTemp.getLastCard()->getCardNumber();
    mvCardGroup.setCard(cardNumber);
    mvCardGroup.showLastCardFace();
    
    //invisible selected card from current stockTemp
    stockTemp.getLastCard()->setVisible(false);
}

void GameScene::onFlipNewCard()
{
    //flip last card on previous card table.
    if( m_nCardTable != 0 &&
       cardTables[m_nCardTable-1].getCardCount() > 0
       && cardTables[m_nCardTable-1].getLastCard()->isFace() == false )
    {
        cardTables[m_nCardTable-1].onFlipTopCard();
        
        setOnlyScore(5);  //don't increase moves

        scheduleOnce(schedule_selector(GameScene::checkGameComplete), FLIP_DURATION * 3);
    }
}

void GameScene::checkGameComplete(float dt)
{
	bool flag = true;
	if( m_bAutoComplete && m_imgGameWon->isVisible() == false )
	{
		if( stockRedeal.getCardCount() == 0 && stockTemp.getCardCount() == 0 )
		{
			for( int i = 0; i < CARDTB_CNT; i ++ )
				if( cardTables[i].isComplete() == false )
				{
					flag = false;
					break;
				}

			//If completed.
			if( flag ) {
				flag = false;
				m_imgGameWon->setVisible(true);
				setEnableActionButtons(false);
				onAutoComplete();
			}
		}
	}

    if( m_bGameSolver && flag )
	{
		if( isPossibleGameWin() == false )
		{
			m_imgNotWin->setVisible(true);
            if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
            {
            	Utils::share()->saveGameState();
                AndroidBridge::getInstance()->notifyNoWinnable();
            }
		}
	}
}

void GameScene::onAutoComplete()
{
	schedule(schedule_selector(GameScene::doAutoComplete), MOVE_TARGET_DURATION * 1.5);
}

void GameScene::showShimmerEffect()
{
    int i = 0;
    int nDelayCnt = 3;
    for( i = CARDTB_CNT - 1; i >= 0; i -- )
        cardTables[i].runAllFaceShimmerAction(SHIMMER_DEALY * nDelayCnt *(CARDTB_CNT -1 - i));
    
    for( i = CARDFD_CNT - 1; i >= 0; i -- )
        foundations[i].runShimmerAction(SHIMMER_DEALY * nDelayCnt *(CARDTB_CNT -1 - i));
    
    stockTemp.runShimmerAction(SHIMMER_DEALY * nDelayCnt  * CARDTB_CNT);
}

#pragma mark - button touch event

void GameScene::touchCallback(Ref * pSender , Widget::TouchEventType type)
{
    Button * btn = dynamic_cast<Button*>(pSender);
    
    if( !btn )
        return;
    
    switch (type) {
        case Widget::TouchEventType::ENDED:
            
            if( btn == btnNew )
            {
                if( m_bArranging == false && m_bRedealing == false )
                    onClickedNewGame();
            }
            else if( btn == btnSettings )
            {
                if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
                {
                	Utils::share()->saveGameState();
                    AndroidBridge::getInstance()->onSetting(FileUtils::getInstance()->getWritablePath());
                }
            }
            else if( btn == btnHighScores )
            {
            	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
				{
					Utils::share()->saveGameState();
					AndroidBridge::getInstance()->onHighScores();
				}
            }
            else if( btn == btnHint )
            {
                if( onClickHint() )
                {
                	if( m_nScoreSetting == 1 )	//standard
						increaseMovesAndScore(-30);
					else if( m_nScoreSetting >= 2 ) //vegas
						increaseMovesAndScore(-5);
                }
            }
            else if( btn == btnUndo )
            {
                onClickedUndo();
            }
            
            break;
            
        default:
            break;
    }
}

void GameScene::onRestartGame(bool isNew)
{
	log(" *************  %d  *************\n", isNew);

	resetGame();

    //init shimmer time.
    time_t ltime;
    time(&ltime);
    m_lShimmerTime = ltime;
    
	m_bArranging = true;
	m_bRedealing = true;

	if( isNew )
		stockTemp.createCards();
	else
		stockTemp.restoreArrangement();

	//arrange cards with animation
	m_nTableNo = 0;
	m_nNextTable = 1;
	schedule(schedule_selector(GameScene::doArrangeCards), ARRANGE_DELAY);

	Utils::playShuffle();
}

void GameScene::onClickedNewGame()
{
    if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    {
    	if( m_nMoves == 0 )	//If did not move any card, restart new game.
    	{
    		onRestartGame(true);
    	}
    	else
    	{
        	Utils::share()->saveGameState();
        	AndroidBridge::getInstance()->onNewGame(getGameInformation(false), getGameResult(false));
    	}
    }
    else  //for test in ios
        onRestartGame(true);
}

std::string GameScene::getGameInformation(bool isWon)
{
	std::string info = "";
	if( m_nMoves > 0 )
	{
		if( isWon == false && !( m_nScoreSetting == 2 || m_nScoreSetting == 3 ) )
			return info;

		if( isWon == false )
			info = "Vegas, ";
		else
		{
			if( m_nScoreSetting == 0 ) //none
				info = "Non scoring, ";
			else if( m_nScoreSetting == 1 ) //standard
				info = "Standard, ";
			else
				info = "Vegas, ";
		}

		if( m_nDrawMode == DRAW_MODE_ONE )
			info = info + "Draw One";
		else
			info = info + "Draw Three";

		int nTime = -1;
		if( m_lblElapsedTime->isVisible() )
			nTime = m_nElapsedTime;

		info = info + "_" + std::to_string(m_nScore) + "_" + std::to_string(m_nMoves) + "_" + std::to_string(nTime);
	}

	return info;
}

std::string GameScene::getGameResult(bool isWon)
{
	//format : scoresetting_drawmode_timed_won_score

	//scoresetting
	std::string result = std::to_string(m_nScoreSetting);

	//drawmode
	if( m_nDrawMode == DRAW_MODE_ONE )
		result = result + "_1";
	else
		result = result + "_3";

	//timed
	 if( Utils::share()->isTimedGame() )
		 result = result + "_1";
	 else
		 result = result + "_0";

	 //won
	 if( isWon )
		 result = result + "_1";
	 else
		 result = result + "_0";

	 //score
	 result = result + "_" + std::to_string(m_nScore);

	 return result;
}

Sprite* GameScene::maskedSpriteWithSprite(Sprite* pTextureSprite, Sprite* pMaskSprite, float x, float y, Sprite* pDestMaskSprite, float xdes, float ydes)
{
	// store the original positions of both sprites
	Point textureSpriteOrigPosition(pTextureSprite->getPosition().x, pTextureSprite->getPosition().y);
	Point maskSpriteOrigPosition(pMaskSprite->getPosition().x, pMaskSprite->getPosition().y);
	Point maskSpriteDesPosition;

	// convert the texture sprite position into mask sprite coordinate system
	pTextureSprite->setPosition( x, y);

	// position the mask sprite so that the bottom left corner lies on the (o,o) coordinates
	pMaskSprite->setPosition(Vec2(pMaskSprite->getContentSize().width/2, pMaskSprite->getContentSize().height/2));
	if( pDestMaskSprite )
		pDestMaskSprite->setPosition(xdes, ydes);

	RenderTexture* rt = RenderTexture::create((int)pMaskSprite->getContentSize().width, (int)pMaskSprite->getContentSize().height);

	BlendFunc bfMask = BlendFunc();
	bfMask.src = GL_ONE;	//GL_ONE
	bfMask.dst = GL_ZERO;	//GL_ZERO
	pMaskSprite->setBlendFunc(bfMask);

	// turn off anti-aliasing around the mask sprite
	pMaskSprite->getTexture()->setAliasTexParameters();

	BlendFunc bfTexture = BlendFunc();
	bfTexture.src = GL_DST_ALPHA;
	bfTexture.dst = GL_ZERO;		//GL_ZERO;
	pTextureSprite->setBlendFunc(bfTexture);

	if( pDestMaskSprite )
	{
		BlendFunc bfDestMask = BlendFunc();
		bfDestMask.src = GL_DST_ALPHA;
		bfDestMask.dst = GL_ZERO;
		pDestMaskSprite->setBlendFunc(bfDestMask);
	}

	rt->begin();
	pMaskSprite->visit();
	if( pDestMaskSprite )
		pDestMaskSprite->visit();
	pTextureSprite->visit();
	rt->end();

	// generate the resulting sprite
	Sprite* pOutcome = Sprite::createWithTexture(rt->getSprite()->getTexture());
	pOutcome->setFlippedY(true);

	// restore the original sprite positions
	pTextureSprite->setPosition(textureSpriteOrigPosition);
//	pMaskSprite->setPosition(maskSpriteOrigPosition);
	pOutcome->setPosition(maskSpriteOrigPosition);

	return pOutcome;
}

void GameScene::addHintLayer(const Rect& rect, const Rect& rcDes)
{
	if( m_spHint )
		return;

	std::string filepath = "placeBg/hintbg.png";
	if( Utils::GetOrientation() == false )
		filepath = "placeBg/hintbg_landscape.png";

	Sprite* bg =   Sprite::create(filepath);
	if( Utils::GetOrientation() )	//portrait
		bg->setPosition(DESIGN_WIDTH / 2.0, DESIGN_HEIGHT / 2.0);
	else	//landscape
		bg->setPosition(DESIGN_WIDTH_LANDSCAPE / 2.0, DESIGN_HEIGHT_LANDSCAPE / 2.0);


	std::string maskname = "placeBg/mask.png";
	float baseHeight = stockTemp.getBaseSprite()->getContentSize().height;
	if( rect.size.height > baseHeight * 1.3 && rect.size.height < baseHeight * 2.3 )
		maskname = "placeBg/mask_2h.png";
	else if( rect.size.height > baseHeight * 2.3 )
		maskname = "placeBg/mask_3h.png";

	Sprite* mask =   Sprite::create(maskname);
    
    float delta = rect.size.width * (SCALE_HINTMASK - 1.0);
	mask->setScale( (rect.size.width + delta) / mask->getContentSize().width, (rect.size.height + delta) / mask->getContentSize().height);

	Sprite* maskDes = NULL;
	if( rcDes.size.width != 0.01  )	// != Rect::ZERO
	{
		maskDes = Sprite::create("placeBg/mask.png");
		maskDes->setScale((rcDes.size.width + delta) / maskDes->getContentSize().width, (rcDes.size.height + delta) / maskDes->getContentSize().height);
	}

	if( maskDes )
		m_spHint  =   maskedSpriteWithSprite(mask, bg, rect.origin.x, rect.origin.y, maskDes, rcDes.origin.x, rcDes.origin.y);
	else
		m_spHint  =   maskedSpriteWithSprite(mask, bg, rect.origin.x, rect.origin.y);

	addChild(m_spHint);
}

void GameScene::stopHintAction()
{
	if( m_spHint)
	{
		m_spHint->removeFromParent();
		m_spHint = NULL;

		//stop shake actions.
		for( int i = 0; i < m_vHintCards.size(); i ++ )
		{
			m_vHintCards.at(i)->stopHintShakeAction();
		}
		m_vHintCards.clear();
	}
}

bool GameScene::onClickHint()
{
    if( m_nHint > 0 && m_bHintMoving == false )
    {
        //table card
        for(int i = 0; i < CARDTB_CNT; i ++ )      //card tables
        {
        	int cnt = cardTables[i].getCardCount();
            if( cnt > 0 )
            {
            	int nFirstFaceCardIndex = cardTables[i].getFirstFaceIndex();
            	if( nFirstFaceCardIndex != -1 )
            	{
            		for( int j = nFirstFaceCardIndex; j < cnt; j ++ )
            		{
            			int nCardNumber = cardTables[i].getCardNumberFromIndex(j);

						//foundation
						int nFD = getCanMergeFoundationNo(nCardNumber);

						if( nFD != -1 )
						{
							//new style
							Rect rc = foundations[nFD].getBaseSprite()->getBoundingBox();
							rc.origin.x += foundations[nFD].getBaseSprite()->getContentSize().width / 2.0;
							rc.origin.y += foundations[nFD].getBaseSprite()->getContentSize().height / 2.0;

							addHintLayer(cardTables[i].getHintRect(j), rc);
							for( int k = j; k < cnt; k ++ )
							{
								Card * card = cardTables[i].m_vCards.at(k);
								m_vHintCards.pushBack(card);
								card->runHintShakeEffect();
							}

							//old style
							//setHintMoveCard(i);
							//onMoveHintCard(nFD, -1, i); //to foundation

							//m_nHint--;
							setHint();
							return true;
						}

						//other card table
						int nCardTB = getCanMergeCardTableNo(nCardNumber);

						if( nCardTB != -1 )
						{
							//new style
							addHintLayer(cardTables[i].getHintRect(j), cardTables[nCardTB].getHintRect(cardTables[nCardTB].getCardCount()-1));
							for( int k = j; k < cnt; k ++ )
							{
								Card * card = cardTables[i].m_vCards.at(k);
								m_vHintCards.pushBack(card);
								card->runHintShakeEffect();
							}

							//old style
							//setHintMoveCard(i);
							//onMoveHintCard(-1, nCardTB, i); //to cardTable

							//m_nHint--;
							setHint();
							return true;
						}
            		}
            	}
            }
        }
        
        //stockTemp
        if( stockTemp.getCardCount() > 0 )
        {
            int nCardNumber = stockTemp.getLastCard()->getCardNumber();
            
            //foundation
            int nFD = getCanMergeFoundationNo(nCardNumber);
            
            if( nFD != -1 )
            {
            	//new style. This is ipad version style.
                Rect rc = stockTemp.getBaseSprite()->getBoundingBox();
                rc.origin.x += stockTemp.getBaseSprite()->getContentSize().width / 2.0;
                rc.origin.y += stockTemp.getBaseSprite()->getContentSize().height / 2.0;
                
                Rect rcDes = foundations[nFD].getBaseSprite()->getBoundingBox();
                rcDes.origin.x += foundations[nFD].getBaseSprite()->getContentSize().width / 2.0;
                rcDes.origin.y += foundations[nFD].getBaseSprite()->getContentSize().height / 2.0;
                
            	addHintLayer(rc, rcDes);
            	stockTemp.getLastCard()->runHintShakeEffect();
            	m_vHintCards.pushBack(stockTemp.getLastCard());

            	//old style
                //setHintMoveCard(-1);
                //onMoveHintCard(nFD, -1, -1); //to foundation
                
                //m_nHint--;
                setHint();
                return true;
            }
            
            //other card table
            int nCardTB = getCanMergeCardTableNo(nCardNumber);
            
            if( nCardTB != -1 )
            {
            	//new style
                Rect rc = stockTemp.getBaseSprite()->getBoundingBox();
                rc.origin.x += stockTemp.getBaseSprite()->getContentSize().width / 2.0;
                rc.origin.y += stockTemp.getBaseSprite()->getContentSize().height / 2.0;
                
            	addHintLayer(rc, cardTables[nCardTB].getHintRect(cardTables[nCardTB].getCardCount()-1));
            	stockTemp.getLastCard()->runHintShakeEffect();
            	m_vHintCards.pushBack(stockTemp.getLastCard());

            	//old style
                //setHintMoveCard(-1);
                //onMoveHintCard(-1, nCardTB, -1); //to cardTable
                
                //m_nHint--;
                setHint();
                return true;
            }
        }
        
        //redeal
        if( stockRedeal.getCardCount() > 0 )
        {
        	//new style
            Rect rc = stockRedeal.getBaseSprite()->getBoundingBox();
            rc.origin.x += stockRedeal.getBaseSprite()->getContentSize().width / 2.0;
            rc.origin.y += stockRedeal.getBaseSprite()->getContentSize().height / 2.0;
            
        	addHintLayer(rc);

        	//old style
            //stockRedeal.getLastCard()->runHintGlowEffect();
            m_nHint--;
            setHint();

            return true;
        }
        else
        {
        	if( m_bRedeal  && stockTemp.getCardCount() > 1 )
        	{
        		//new style
                Rect rc = stockRedeal.getBaseSprite()->getBoundingBox();
                rc.origin.x += stockRedeal.getBaseSprite()->getContentSize().width / 2.0;
                rc.origin.y += stockRedeal.getBaseSprite()->getContentSize().height / 2.0;
                
        		addHintLayer(rc);

        		//old style
        		//stockRedeal.runHintGlowEffect();
        		return true;
        	}
        }

        //btnHint->setBright(false);
		btnHint->setOpacity(150);
		btnHint->setEnabled(false);
    }

    return false;
}

bool GameScene::isPossibleGameWin()
{
	if( m_nHint > 0 && m_bHintMoving == false )
	{
		//table card
		for(int i = 0; i < CARDTB_CNT; i ++ )      //card tables
		{
			if( cardTables[i].getCardCount() > 0 )
			{
				int nCardNumber = cardTables[i].getLastCard()->getCardNumber();

				//foundation
				int nFD = getCanMergeFoundationNo(nCardNumber);

				if( nFD != -1 )
				{
					return true;
				}

				//other card table
				int nCardTB = getCanMergeCardTableNo(nCardNumber);

				if( nCardTB != -1 )
				{
					return true;
				}
			}
		}

		//stockTemp
		if( stockTemp.getCardCount() > 0 )
		{
			int nCardNumber = stockTemp.getLastCard()->getCardNumber();

			//foundation
			int nFD = getCanMergeFoundationNo(nCardNumber);

			if( nFD != -1 )
			{
				return true;
			}

			//other card table
			int nCardTB = getCanMergeCardTableNo(nCardNumber);

			if( nCardTB != -1 )
			{
				return true;
			}
		}

		//redeal
		if( stockRedeal.getCardCount() > 0 )
		{
			return true;
		}
		else
		{
			if( m_bRedeal && stockTemp.getCardCount() > 1 )
			{
				return true;
			}
		}
	}

	return false;
}

void GameScene::onClickedUndo()
{
    if( m_nMoves > 0 && m_bHintMoving == false )
    {
        m_nMoves --;
        if( m_nScoreSetting == 1 ) 			//standard
        	//m_nScore /= 2;	//iphone rule
        	m_nScore -= 30;				//ipad rule
        else if( m_nScoreSetting == 2 ) 	//vegas
        	//m_nScore -= (m_nScore - m_nInitScore) / 2;	//iphone rule
        	m_nScore -= 5;
        else if( m_nScoreSetting == 3 )		//cumulative
        	//m_nScore -= (m_nScore - m_nInitScore) / 2;
        	m_nScore -= 5;
        
        if( m_nScore < m_nInitScore )
        	m_nScore = m_nInitScore;

        setScoreData();
        
        //undo cardTables
        for( int i = 0; i < CARDTB_CNT; i ++ )
            cardTables[i].restoreState(m_nMoves);
        
        //undo foundations
        for( int i = 0; i < CARDFD_CNT; i ++ )
            foundations[i].restoreState(m_nMoves);
        
        stockTemp.restoreState(m_nMoves);
        
        stockRedeal.restoreState(m_nMoves);

        saveGameState();
    }
}

#pragma mark - touch event

bool GameScene::onTouchBegan(Touch * touch, Event * event)
{
	stopHintAction();

    if( m_bIsRunning == false )
        return false;
    
    if( m_bHintMoving )
        return false;
    
    if( m_bIsTimeElapse == false )
    	m_bIsTimeElapse = true;

    m_pTouchBegin = touch->getLocation();
    //m_bTouchMoving = false;
    
    if(m_bTouchAgain && m_bTouchMoving == false && isDoubleTap() )
    {
        onDoubleTapped();
    }
    
    m_nfStockRedeal = 0;
    if( m_bTouchAgain )
    {
        if( stockRedeal.getBaseSprite()->getBoundingBox().containsPoint(m_pTouchBegin) )
        {
            m_nfStockRedeal = 1;      //stockRedeal
            return true;
        }
    }
    
    return m_bTouchAgain;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    Vec2 ps = touch->getLocation();
    int cnt = mvCardGroup.getCardCount();
    
    if( cnt > 0 )    //moving mvCardGroup
    {
        Vec2 position = ps + m_pTouchDelta;
        mvCardGroup.getBaseSprite()->setPosition(position);
        
        if( position.x < m_pTouchBegin.x )  //left
        {
            if( cnt == 1 && ps.y < position.y )     //under anchor
                mvCardGroup.setRightRotate();
            else
                mvCardGroup.setLeftRotate();
        }
        else    //right
        {
            if( cnt == 1 && ps.y < position.y )
                mvCardGroup.setLeftRotate();
            else
                mvCardGroup.setRightRotate();
        }
    }
    else
    {
    	if( m_bTouchMoving == false && ps.distance(m_pTouchBegin) > DISTANCE_ACTIVATE )
        {
            m_bTouchMoving = touchValidate();
            
            if( m_bTouchMoving )
            {
                m_bTouchAgain = false;
                log(" touchAgain : onTouchMoved");
            }
        }
    }
}

void GameScene::onTouchEnded(Touch * touch, Event * event)
{
    m_bTouchMoving = false;
    
    m_pTouchEnd = touch->getLocation();
    
    int nMovingCards = mvCardGroup.getCardCount();
    
    if( m_nfStockRedeal == 1 &&
       stockRedeal.getBaseSprite()->getBoundingBox().containsPoint(m_pTouchEnd) )
    {
        if( stockRedeal.getCardCount() > 0 )
        {
            m_bTouchAgain = false;
            //flip and move animation from stock redeal to stock temp.
            if( m_nDrawMode == DRAW_MODE_ONE )
                onSlideFlipToTempStock();
            else
                onThreeSlideFlipToTempStock();
        }
        else
        {
            //redeal
            if( stockRedeal.getBaseSprite()->isVisible() && stockTemp.getCardCount() > 0 )
            {
            	if( m_bRedeal )
            	{
            		if( m_nDrawMode == DRAW_MODE_THREE )
					{
						stockTemp.foldCards(2);
						DelayTime* delayAction = DelayTime::create(FOLD_DURATION);
						CallFunc * func = CallFunc::create(CC_CALLBACK_0(GameScene::onRedeal, this));
						Sequence * seq = Sequence::create(delayAction, func, NULL);
						runAction(seq);
					}
					else
						onRedeal();

					Utils::playRedeal();
            	}
            }
        }
    }
    else if( nMovingCards > 0 )   //moving mvCardGroup
    {
        bool isMerge = false;
        Rect rcMVCG = mvCardGroup.getRect();
        int nCardNumber = mvCardGroup.getCardNumberFromIndex(0);
        
        if( nMovingCards == 1 )         //foundation
        {
            for( int i = 0; i < CARDFD_CNT; i ++ )
            {
                Rect rc = foundations[i].getBaseSprite()->getBoundingBox();
                if( rc.intersectsRect(rcMVCG) && foundations[i].canMerge(nCardNumber) )
                {
                    onMergeCardToFoundation(i);
                    if( m_nFoundation == 0 )
                        Utils::playFoundation();
                    else
                        Utils::playCardSlide();
                    isMerge = true;
                    
                    break;
                }
            }
        }
        
        if( isMerge == false )      //card table
        {
            for( int i = 0; i < CARDTB_CNT; i ++ )
            {
                Rect rc = cardTables[i].getRect();
                if( i + 1 != m_nCardTable && rcMVCG.intersectsRect(rc) && cardTables[i].canMerge(nCardNumber) )
                {
                    onMergeCardsToCardTable(i);
                    Utils::playCardSlide();
                    isMerge = true;
                    
                    break;
                }
            }
        }
        
        if( isMerge == false )  //go back
        {
            if( m_nCardTable != 0 )
                onMergeCardsToCardTable(m_nCardTable-1, true);
            else if( m_nFoundation )
                onMergeCardToFoundation(m_nFoundation-1, true);
            else if( m_nfStockTemp )
                onMergeCardToStockTemp();
        }
        else
        {
            onFlipNewCard();
            
            if( m_nfStockTemp )
                removeSentCardFromStockTemp();
        }
    }
}

bool GameScene::touchValidate()
{
    m_nTouchBegin = 0;
//    m_nfStockRedeal = 0;
    m_nfStockTemp = 0;
    m_nFoundation = 0;
    m_nCardTable = 0;
    
//    if( stockRedeal.getBaseSprite()->getBoundingBox().containsPoint(m_pTouchBegin) &&
//       stockRedeal.getCardCount() > 0 )
//    {
//        m_nfStockRedeal = 1;      //stockRedeal
//        return true;
//    }
    
    if( stockTemp.getLastCardBoundingBox().containsPoint(m_pTouchBegin) &&
        stockTemp.getCardCount() > 0 )
    {
        m_nfStockTemp = 1;      //stockTemp
        setTouchMovingCardsFromStockTemp();
        
        return true;
    }

    int i;
    for( i = 0; i < CARDFD_CNT; i ++ )      //card foundations
    {
        if( foundations[i].getCardCount() > 0 && foundations[i].getBaseSprite()->getBoundingBox().containsPoint(m_pTouchBegin) )
        {
            m_pTouchDelta = foundations[i].getBaseSprite()->getPosition() - m_pTouchBegin;
            m_nFoundation = i + 1;
            setMovingCardsFromFoundation(i);
            
            return true;
        }
    }
    
    for( i = 0; i < CARDTB_CNT; i ++ )      //card tables
    {
        int cardNo = cardTables[i].getSelectedCardNo(m_pTouchBegin);
        if( cardNo != -1 )
        {
            m_pTouchDelta = cardTables[i].getCardPosition(cardNo) - m_pTouchBegin;
            m_nCardTable = i + 1;
            setMovingCards(i, cardNo);
            
            return true;
        }
    }
    
    m_nTouchBegin = m_nfStockTemp + m_nFoundation + m_nCardTable;
    
    return m_nTouchBegin != 0;
}

bool GameScene::isDoubleTap()
{
    time_t rawtime;
    time (&rawtime);
    
    //checking
    m_lTimeStart = m_lTimeEnd;
    m_lTimeEnd = rawtime;
    
    long timeDifference = m_lTimeEnd - m_lTimeStart;    //second
    
    bool res = false;
    
    log(" ##  isDoubleTap() %f, %ld   ##\n", m_pTouchOld.distance(m_pTouchBegin), timeDifference);

    if( m_pTouchOld.distance(m_pTouchBegin) < DISTANCE_ACTIVATE && timeDifference <= TIME_ACTIVATE )
        res = true;
    
    m_pTouchOld = m_pTouchBegin;
    
    return res;
}

void GameScene::onDoubleTapped()
{
    //stock temp
    if( stockTemp.getLastCardBoundingBox().containsPoint(m_pTouchBegin) &&
       stockTemp.getCardCount() > 0 )
    {
        int nCardNumber = stockTemp.getLastCard()->getCardNumber();
        int nFD = getCanMergeFoundationNo(nCardNumber);
        
        if( nFD != -1 )
        {
            log(" touchAgain : onDoubleTapped");
            m_bTouchAgain = false;
            m_nfStockTemp = 1;      //stockTemp
            
            if( m_nDrawMode == DRAW_MODE_ONE )
                setMovingCardsFromStockTemp();
            else
			{
                setTouchMovingCardsFromStockTemp();
				removeSentCardFromStockTemp();
			}
            
            onMergeCardToFoundation(nFD);
            Utils::playFoundation();
            
            return;
        }
        
        int nCardTB = getCanMergeCardTableNo(nCardNumber);
        
        if( nCardTB != -1 )
        {
            log(" touchAgain : onDoubleTapped / 1");
            m_bTouchAgain = false;
            m_nfStockTemp = 1;      //stockTemp
            
            if( m_nDrawMode == DRAW_MODE_ONE )
                setMovingCardsFromStockTemp();
            else
			{
                setTouchMovingCardsFromStockTemp();
				removeSentCardFromStockTemp();
			}
            
            onMergeCardsToCardTable(nCardTB);
            Utils::playCardSlide();
            
            return;
        }
    }

    int i;
    int nCardNumber = -1;
    //foundation
    int nFromFD = -1;
    for( i = 0; i < CARDFD_CNT; i ++ )      //card
    {
        if( foundations[i].getCardCount() > 0 && foundations[i].getBaseSprite()->getBoundingBox().containsPoint(m_pTouchBegin) )
        {
            nCardNumber = foundations[i].getLastCard()->getCardNumber();
            nFromFD = i;
            break;
        }
    }
    
    if( nCardNumber != -1 )
    {
        int nFD = getCanMergeFoundationNo(nCardNumber);
        
        if( nFD != -1 )
        {
            log(" touchAgain : onDoubleTapped / 2");
            m_bTouchAgain = false;
            m_nFoundation = nFromFD + 1;
            
            setMovingCardsFromFoundation(nFromFD);
            onMergeCardToFoundation(nFD);
            Utils::playCardSlide();
            
            onFlipNewCard();
            
            return;
        }
        
        int nCardTB = getCanMergeCardTableNo(nCardNumber);
        
        if( nCardTB != -1 )
        {
            log(" touchAgain : onDoubleTapped / 3");
            m_bTouchAgain = false;
            m_nFoundation = nFromFD + 1;
            
            setMovingCardsFromFoundation(nFromFD);
            onMergeCardsToCardTable(nCardTB);
            Utils::playCardSlide();
            
            onFlipNewCard();
            
            return;
        }
    }
    
    //table card
    nCardNumber = -1;
    int nFromCardTB = -1;
    for( i = 0; i < CARDTB_CNT; i ++ )      //card tables
    {
        int cardNo = cardTables[i].getSelectedCardNo(m_pTouchBegin);
        if( cardNo != -1 && cardNo == cardTables[i].getCardCount() - 1 )
        {
            nFromCardTB = i;
            nCardNumber = cardTables[i].getLastCard()->getCardNumber();
            
            break;
        }
    }
    
    if( nCardNumber != -1 )
    {
        int nFD = getCanMergeFoundationNo(nCardNumber);
        
        if( nFD != -1 )
        {
            log(" touchAgain : onDoubleTapped / 4");
            m_bTouchAgain = false;
            m_nCardTable = nFromCardTB + 1;
            
            setMovingCards(nFromCardTB, cardTables[nFromCardTB].getCardCount()-1 );
            onMergeCardToFoundation(nFD);
            Utils::playFoundation();
            
            onFlipNewCard();
            
            return;
        }
        
        int nCardTB = getCanMergeCardTableNo(nCardNumber);
        
        if( nCardTB != -1 )
        {
            log(" touchAgain : onDoubleTapped / 5");
            m_bTouchAgain = false;
            m_nCardTable = nFromCardTB + 1;
            
            setMovingCards(nFromCardTB, cardTables[nFromCardTB].getCardCount()-1 );
            onMergeCardsToCardTable(nCardTB);
            Utils::playCardSlide();
            
            onFlipNewCard();
            
            return;
        }
    }
}

int GameScene::getCanMergeFoundationNo(int nCardNumber)
{
    int res = -1;
    
    for( int i = 0; i < CARDFD_CNT; i ++ )
    {
        if( foundations[i].canMerge(nCardNumber) )
        {
            res = i;
            break;
        }
    }
    
    return res;
}

int GameScene::getCanMergeCardTableNo(int nCardNumber)
{
    int res = -1;
    
    for( int i = 0; i < CARDTB_CNT; i ++ )
    {
        if( cardTables[i].canMerge(nCardNumber) )
        {
            res = i;
            break;
        }
    }
    
    return res;
}

#pragma mark - score management

void GameScene::setOnlyScore(int nScore)
{
	if( m_nScoreSetting == 2 || m_nScoreSetting == 3 )	//vegas or cumulative
		return;

    m_nScore += nScore;
    if( m_nScore < 0 )
        m_nScore = 0;
    
    Utils::SetScoreData(m_nMoves, m_nScore, m_nElapsedTime);
    
    setScoreData();
}

void GameScene::increaseMovesAndScore(int nScore)
{
    m_nMoves ++;
    if( m_nScoreSetting == 2 || m_nScoreSetting == 3 )
    {
    	if(nScore == 10 )
    		m_nScore += 5;
    }
    else
    	m_nScore += nScore;

    if( m_nScore < m_nInitScore )
        m_nScore = m_nInitScore;
    
    setScoreData();
    saveGameState();
}

void GameScene::doAutoComplete(float dt)
{
	log(" ******* 4 ******\n");

	m_bIsRunning = false;
	m_bIsTimeElapse = false;

	//move cards from cardTables to foundations


	int nCardNumber = -1;
	int nFromCardTB = -1;
	for(int i = 0; i < CARDTB_CNT; i ++ )      //card tables
	{
		if( cardTables[i].getCardCount() > 0 )
		{
			nFromCardTB = i;
			nCardNumber = cardTables[i].getLastCard()->getCardNumber();

			int nFD = getCanMergeFoundationNo(nCardNumber);

			if( nFD != -1 )
			{
				m_bTouchAgain = false;
				m_nCardTable = nFromCardTB + 1;

				setMovingCards(nFromCardTB, cardTables[nFromCardTB].getCardCount()-1 );
				onMergeCardToFoundation(nFD);
				Utils::playFoundation();

				return;
			}
		}
	}

	unschedule(schedule_selector(GameScene::doAutoComplete));
}

void GameScene::decreaseMovesAndScore(int nScore)
{
    m_nMoves --;

    m_nScore -= nScore;
    if( m_nScore < 0 )
        m_nScore = 0;

    setScoreData();
    saveGameState();
}

void GameScene::setScoreData()
{
    m_lblMoves->setString("Moves: " + std::to_string(m_nMoves));
    if( m_nScoreSetting == 2 || m_nScoreSetting == 3 )
    	m_lblScore->setString("Score: " + std::to_string(m_nScore) + "$");
    else
    	m_lblScore->setString("Score: " + std::to_string(m_nScore));
    m_lblElapsedTime->setString("Time: " + std::to_string(m_nElapsedTime));
    
    if( m_nMoves == 0 )   //disable undo button
    {
        //btnUndo->setBright(false);
        btnUndo->setOpacity(150);
        btnUndo->setEnabled(false);
    }
    else    //enable undo button
    {
        //btnUndo->setBright(true);
        btnUndo->setOpacity(255);
        btnUndo->setEnabled(true);
    }
}

void GameScene::setEnableActionButtons(bool bEnable)
{
	if( bEnable == false )  //disable button
	{
		btnUndo->setOpacity(150);
		btnUndo->setEnabled(false);

		btnHint->setOpacity(150);
		btnHint->setEnabled(false);
	}
	else    //enable
	{
		btnUndo->setOpacity(255);
		btnUndo->setEnabled(true);

		btnHint->setOpacity(255);
		btnHint->setEnabled(true);
	}
}

void GameScene::setHint()
{
//    btnHint->setTitleText("Hint(" + std::to_string(m_nHint) + ")");
    Utils::SetHintCount(m_nHint);
    
    if( m_nHint == 0 )  //disable button
    {
        //btnHint->setBright(false);
        btnHint->setOpacity(150);
        btnHint->setEnabled(false);
    }
    else    //enable
    {
        //btnHint->setBright(true);
        btnHint->setOpacity(255);
        btnHint->setEnabled(true);
    }
}

void GameScene::setCurrentTime(float dt)
{
    time_t ltime;
    struct tm * timeinfo;
    time(&ltime);
    
    //shimmer effect
    long deltaTime = ltime - m_lShimmerTime;
    if( deltaTime > SHIMMER_DURATION && m_bIsRunning == true && m_bTouchMoving == false && m_bHintMoving == false )
    {
        m_lShimmerTime = ltime;
        Utils::SetShimmerTime(m_lShimmerTime);
        showShimmerEffect();
    }
    
    timeinfo = localtime (&ltime);
    
    /*
    log("year->%d",timeinfo->tm_year+1900);
    log("month->%d",timeinfo->tm_mon+1);
    log("date->%d",timeinfo->tm_mday);
    log("hour->%d",timeinfo->tm_hour);
    log("minutes->%d",timeinfo->tm_min);
    log("seconds->%d",timeinfo->tm_sec);
     */
 
    int nHour = timeinfo->tm_hour;
    std::string sHour = std::to_string(nHour);
    if( nHour < 10 )
        sHour = "0" + sHour;
    
    int nMin = timeinfo->tm_min;
    std::string sMin = std::to_string(nMin);
    if( nMin < 10 )
        sMin = "0" + sMin;
    
    std::string curTime = sHour + ":" + sMin;
    m_lblTime->setString(curTime);

    if( m_bIsTimeElapse )
    {
    	m_nElapsedTime ++;
    	m_lblElapsedTime->setString("Time: " + std::to_string(m_nElapsedTime));
    }
}

#pragma mark - advanced settings

void GameScene::setRightHandMode()
{
	//get positions of card seats.
	Vec2 pRedeal = stockRedeal.getBaseSprite()->getPosition();
	Vec2 pFirstFoundation = foundations[0].getBaseSprite()->getPosition();
	Vec2 pMoveFoundation = pFirstFoundation - pRedeal;
	Vec2 pLastFoundation = foundations[CARDFD_CNT-1].getBaseSprite()->getPosition();

	m_pRightStockTemp = foundations[CARDFD_CNT-2].getBaseSprite()->getPosition();
	if( m_nDrawMode == DRAW_MODE_THREE )
	{
		m_pRightStockTemp.x -= (stockRedeal.getBaseSprite()->getContentSize().width * 2 / 3.0 );
	}

	//set positions
	stockRedeal.getBaseSprite()->setPosition(pLastFoundation);
	stockTemp.getBaseSprite()->setPosition(m_pRightStockTemp);
	for( int i = 0; i < CARDFD_CNT; i ++ )
	{
		Vec2 pos = foundations[i].getBaseSprite()->getPosition();
		pos -= pMoveFoundation;
		foundations[i].getBaseSprite()->setPosition(pos);
	}
}

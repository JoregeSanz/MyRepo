//
//  NinjaScene.cpp
//  TingTong
//
//  Created by lion on 6/24/14.
//
//

#include "NinjaScene.h"
#include "GameSelectScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

const char* objectname1[14] = {"woodenb","watermelon","keyboard","pillow","sack","shoe","dynamite","box","money","tv","tomato","candy","bottle","glass"};


CCScene* NinjaScene::scene()
{
    CCScene* scene = CCScene::create();
    NinjaScene* layer = NinjaScene::create();
    scene->addChild(layer);
    return scene;
}

bool NinjaScene::init()
{
    if (!BaseLayer::init()) 
        return false;
    
    setTouchEnabled(true);
    setBackground(18);
    
    objary = new CCArray;
    objary->initWithCapacity(100);
		
        b2Vec2 gravity;
        gravity.Set(0.0f, -3.0f);
        world = new b2World(gravity);
        
        
        // Do we want to let bodies sleep?
        world->SetAllowSleeping(true);
        world->SetContinuousPhysics(true);
        
//        m_debugDraw = new GLESDebugDraw( PTM_RATIO );
//        world->SetDebugDraw(m_debugDraw);
//        uint32 flags = 0;
//        flags += b2Draw::e_shapeBit;
//        flags += b2Draw::e_jointBit;
//        flags += b2Draw::e_pairBit;
//        flags += b2Draw::e_centerOfMassBit;
//        flags += b2Draw::e_aabbBit;
//        m_debugDraw->SetFlags(flags);
    
    schedule(schedule_selector(NinjaScene::update));
    
    
    levelinfo = new CCSVParse;
    levelinfo->openFile("ninja1.csv");
    
    m_levelInfo.Levelnum = CCUserDefault::sharedUserDefault()->getIntegerForKey(LEVEL);
    m_levelInfo.Time = atoi(levelinfo->getData(0, 1).c_str());
    m_levelInfo.Lifes = atoi(levelinfo->getData(3, 1).c_str());
    m_levelInfo.Speed = atoi(levelinfo->getData(1, 1).c_str());
    m_levelInfo.Ticks = atoi(levelinfo->getData(7, 1).c_str());
    m_levelInfo.Points1 =atoi(levelinfo->getData(4, 1).c_str());
    m_levelInfo.Points2 =atoi(levelinfo->getData(5, 1).c_str());
    m_levelInfo.Points3 =atoi(levelinfo->getData(6, 1).c_str());
    
    m_nTicknum = 10;
    
	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
    
    char m_lev[10];
    sprintf(m_lev, "Level:%d", m_levelInfo.Levelnum);
    lbl_levelNum = CCLabelTTF::create(m_lev, "LuckiestGuyFixed", 80 * g_fResScale);
    lbl_levelNum->setAnchorPoint(ccp(0.5f,0.0f));
    lbl_levelNum->setPosition(ccp(winSize.width*0.1f, winSize.height*0.8f));
    lbl_levelNum->setColor(ccBLUE);
    this->addChild(lbl_levelNum);
    
    
    
    for (int i = 1; i <= m_levelInfo.Lifes; i++)
    {
        CCSprite* heart = CCSprite::create("heart.png");
        heart->setTag(i+HEART_TAG);
        heart->setPosition(ccp(winSize.width*0.6f + i*winSize.width/16, winSize.height*0.8f));
        heart->setAnchorPoint(ccp(0.0f, 0.0f));
        heart->setScale(g_fResScale);
        this->addChild(heart);
    }
    
    char m_Score[20];
    //m_Score = 0;
    sprintf(m_Score, "Score:0");
    lbl_Score = CCLabelTTF::create(m_Score, "LuckiestGuyFixed", 80 * g_fResScale);
    lbl_Score->setAnchorPoint(ccp(0.5f,0.0f));
    lbl_Score->setPosition(ccp(winSize.width*0.3f, winSize.height*0.8f));
    lbl_Score->setColor(ccYELLOW);
    this->addChild(lbl_Score);
    
    lbl_Time = CCLabelTTF::create("time:", "LuckiestGuyFixed", 80 * g_fResScale);
    lbl_Time->setAnchorPoint(ccp(0.5f,0.0f));
    lbl_Time->setPosition(ccp(winSize.width*0.5f, winSize.height*0.8f));
    lbl_Time->setColor(ccGREEN);
    this->addChild(lbl_Time);
    
    lbl_Target = CCLabelTTF::create("", "BlackChancery-Regular.ttf", 120 * g_fResScale);
    lbl_Target->setPosition(ccp(winSize.width*0.5f, winSize.height*0.7f));
    lbl_Target->setColor(ccBLACK);
    this->addChild(lbl_Target);
    
    readWordInfo();
    
    CCTextureCache* cache = CCTextureCache::sharedTextureCache();
    for (int i = 0; i < 14; i++) {
        CCImage* img = new CCImage;
        char name[20] = {0};
        sprintf(name, "%s.png",objectname1[i]);
        img->initWithImageFile(name);
        cache->addUIImage(img, name);
    }
    return true;

}

void NinjaScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
        CCTouch *touch = (CCTouch *)*it;
		CCBlade *blade = CCBlade::create("streak.png", 4, 50);
        _map[touch] = blade;
		addChild(blade);
        
        blade->setColor(ccc3(255,0,0));
        blade->setOpacity(100);
        blade->setDrainInterval(1.0/40);
        
        CCPoint point = convertTouchToNodeSpace(touch);
		blade->push(point);
        startpos = point;
	}
    CCTouch* touch = (CCTouch*)pTouches->anyObject();
    startpos = convertToNodeSpace(touch->getLocationInView());
    
}

void NinjaScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
        CCTouch *touch = (CCTouch *)*it;
        if (_map.find(touch) == _map.end()) continue;
        
        CCBlade *blade = _map[touch];
        CCPoint point = convertTouchToNodeSpace(touch);
        point = ccpAdd(ccpMult(point, 0.6f), ccpMult(touch->getPreviousLocation(), 0.4f));
		blade->push(point);
        endpos = point;
    }
    
    CCPoint sub = ccpSub(endpos, startpos);
    b2Vec2 vel = b2Vec2(sub.x/PTM_RATIO,-sub.y/PTM_RATIO);

    
    if (ccpLengthSQ(ccpSub(startpos, endpos)) > 25)
    {
        for(int i = 0;i<objary->count();i++)
        {
        Object* obj = (Object*)objary->objectAtIndex(i);
        if(obj->sprite->boundingBox().containsPoint(endpos))
        {
            CCSize spritesize = obj->sprite->getContentSize();
            
            char name[20] = {0};
            sprintf(name, "%s.png",objectname1[obj->sprite->getTag()]);
            
            CCTextureCache* cache = CCTextureCache::sharedTextureCache();
            CCTexture2D* spritetexture = cache->textureForKey(name);
            PhysicsSprite* splitsp1 = new PhysicsSprite;
            splitsp1->initWithTexture(spritetexture, CCRectMake(0, 0, spritesize.width/2, spritesize.height));

            
            PhysicsSprite* splitsp2 = new PhysicsSprite;
            splitsp2->initWithTexture(spritetexture, CCRectMake(spritesize.width/2, 0, spritesize.width/2, spritesize.height));
            
            float bodyangle = obj->body->GetAngle();
            b2Vec2 bodypos = obj->body->GetPosition();
            
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.angle = bodyangle;
            bd.linearVelocity = vel;
            bd.angularVelocity = obj->body->GetAngularVelocity();
            
            b2CircleShape circle;
            circle.m_radius = 1.0 * BodyScale;
            b2FixtureDef fd;
            fd.shape = &circle;
            fd.density = 1.0f;
            fd.filter.maskBits = 0;
            
            
            bd.position.Set(bodypos.x - spritesize.width/4.0f*cos(bodyangle)/PTM_RATIO*g_fResScale, bodypos.y - spritesize.height/4.0f*sin(bodyangle)/PTM_RATIO*g_fResScale);
            b2Vec2 force1 = bd.position - b2Vec2(startpos.x/PTM_RATIO, startpos.y/PTM_RATIO);
            b2Body* body1 = world->CreateBody(&bd);
            body1->CreateFixture(&fd);
            body1->ApplyForce(3*force1, obj->body->GetPosition());
            splitsp1->setPhysicsBody(body1);
            
            bd.position.Set(bodypos.x + spritesize.width/4.0f*cos(bodyangle)/PTM_RATIO*g_fResScale, bodypos.y + spritesize.height/4.0f*sin(bodyangle)/PTM_RATIO*g_fResScale);
            b2Vec2 force2 = bd.position - b2Vec2(startpos.x/PTM_RATIO, startpos.y/PTM_RATIO);
            b2Body* body2 = world->CreateBody(&bd);
            body2->CreateFixture(&fd);
            body2->ApplyForce(3*force2, obj->body->GetPosition());
            splitsp2->setPhysicsBody(body2);
            
            
            int spritetag = obj->sprite->getTag();
            if (spritetag == 6)
            {
                CCSprite* ex = CCSprite::create("dynamite_dest.png");
                ex->setScale(0.05);
                CCSequence* seq = CCSequence::create(CCScaleTo::create(0.5, g_fResScale),CCFadeOut::create(0.2),NULL);
                ex->runAction(seq);
                ex->setPosition(obj->sprite->getPosition());
                this->addChild(ex);
                splitsp1->setVisible(false);
                splitsp2->setVisible(false);
            }
            else if(spritetag < 6)
            {
                char name[20] = {0};
                sprintf(name, "%s_piece.png",objectname1[spritetag]);
                for (int i = 0; i < 7; i++)
                {
                    b2BodyDef bd;
                    bd.type = b2_dynamicBody;
                    bd.angle = arc4random();
                    bd.linearVelocity = b2Vec2(arc4random()%10/10.0f,arc4random()%10/10.0f);
                    bd.angularVelocity = arc4random() % 10;
                    bd.position = b2Vec2(bodypos.x+arc4random()%3/10.0f,bodypos.y+arc4random()%3/10.0f);
                    b2CircleShape circle;
                    circle.m_radius = 0.1f * BodyScale;
                    b2FixtureDef fd;
                    fd.shape = &circle;
                    fd.density = 1.0f;
                    fd.filter.maskBits = 0;
                    
                    b2Body* piece = world->CreateBody(&bd);
                    piece->CreateFixture(&fd);
                
                    PhysicsSprite* sp = new PhysicsSprite;
                    sp->init(name);
                    sp->setPhysicsBody(piece);
                    this->addChild(sp);
                    
                }
            }
            else if (spritetag > 10)
            {
                char name[20] = {0};
                for (int i = 0; i < 7; i++)
                {
                    sprintf(name, "%s_piece%d.png",objectname1[spritetag],arc4random()%3+1);

                    b2BodyDef bd;
                    bd.type = b2_dynamicBody;
                    bd.angle = arc4random();
                    bd.linearVelocity = b2Vec2(arc4random()%10/10.0f,arc4random()%10/10.0f);
                    bd.angularVelocity = arc4random() % 10;
                    bd.position = b2Vec2(bodypos.x+arc4random()%3/10.0f,bodypos.y+arc4random()%3/10.0f);
                    b2CircleShape circle;
                    circle.m_radius = 0.1f * BodyScale;
                    b2FixtureDef fd;
                    fd.shape = &circle;
                    fd.density = 1.0f;
                    fd.filter.maskBits = 0;
                    
                    b2Body* piece = world->CreateBody(&bd);
                    piece->CreateFixture(&fd);
                    
                    PhysicsSprite* sp = new PhysicsSprite;
                    sp->init(name);
                    sp->setPhysicsBody(piece);
                    this->addChild(sp);
                    
                }
            }
            else if (spritetag == 10)
            {
                
                CCSprite* ex = CCSprite::create("tomato_piece1.png");
                ex->setScale(0.05);
                CCSequence* seq = CCSequence::create(CCScaleTo::create(0.5, g_fResScale),CCFadeOut::create(0.2),NULL);
                ex->runAction(seq);
                ex->setPosition(obj->sprite->getPosition());
                this->addChild(ex);
                
                for (int i = 0; i < 7; i++)
                {
                    
                    b2BodyDef bd;
                    bd.type = b2_dynamicBody;
                    bd.angle = arc4random();
                    bd.linearVelocity = b2Vec2(arc4random()%10/10.0f,arc4random()%10/10.0f);
                    bd.angularVelocity = arc4random() % 10;
                    bd.position = b2Vec2(bodypos.x+arc4random()%3/10.0f,bodypos.y+arc4random()%3/10.0f);
                    b2CircleShape circle;
                    circle.m_radius = 0.1f * BodyScale;
                    b2FixtureDef fd;
                    fd.shape = &circle;
                    fd.density = 1.0f;
                    fd.filter.maskBits = 0;
                    
                    b2Body* piece = world->CreateBody(&bd);
                    piece->CreateFixture(&fd);
         
                    PhysicsSprite* sp = new PhysicsSprite;
                    sp->init("tomato_piece2.png");
                    sp->setPhysicsBody(piece);
                    this->addChild(sp);
                    
                }
            }
            else
            {
                char name[20] = {0};
                for (int i = 0; i < 7; i++)
                {
                    sprintf(name, "%s_piece%d.png",objectname1[spritetag],arc4random()%2+1);

                    b2BodyDef bd;
                    bd.type = b2_dynamicBody;
                    bd.angle = arc4random();
                    bd.linearVelocity = b2Vec2(arc4random()%10/10.0f,arc4random()%10/10.0f);
                    bd.angularVelocity = arc4random() % 10;
                    bd.position = b2Vec2(bodypos.x+arc4random()%3/10.0f,bodypos.y+arc4random()%3/10.0f);
                    b2CircleShape circle;
                    circle.m_radius = 0.1f * BodyScale;
                    b2FixtureDef fd;
                    fd.shape = &circle;
                    fd.density = 1.0f;
                    fd.filter.maskBits = 0;
                    
                    b2Body* piece = world->CreateBody(&bd);
                    piece->CreateFixture(&fd);

                    PhysicsSprite* sp = new PhysicsSprite;
                    sp->init(name);
                    sp->setPhysicsBody(piece);
                    this->addChild(sp);
                    
                }
            }
            
            
                
            this->addChild(splitsp1);
            this->addChild(splitsp2);
            
            if(obj->getTag() == lbl_Target->getTag())
            {
                CCLayerColor* colorlayer = CCLayerColor::create();
                colorlayer->setColor(ccRED);
                colorlayer->setOpacity(120);
                CCSequence* seq = CCSequence::create(CCFadeIn::create(0.2),CCFadeOut::create(0.2),CCCallFunc::create(this, callfunc_selector(NinjaScene::removeColorLayer)),NULL);
                colorlayer->runAction(seq);
                this->addChild(colorlayer);
                
                m_levelInfo.Lifes--;
                CCSprite* heart = (CCSprite*)this->getChildByTag(HEART_TAG+m_levelInfo.Lifes+1);
                heart->removeFromParent();
                
                if (m_levelInfo.Lifes == 0)
                    goGameOver();
            }
            else
            {
                m_levelInfo.Score += 50;
                char num_Score[10];
                sprintf(num_Score, "Score:%d", m_levelInfo.Score);
                lbl_Score->setString(num_Score);
                
                if (m_levelInfo.Score == m_levelInfo.Points1)
                {
                    m_levelInfo.Levelnum += 1;
                    CCUserDefault::sharedUserDefault()->setIntegerForKey(LEVEL, m_levelInfo.Levelnum);
                    CCDirector::sharedDirector()->replaceScene(NinjaScene::scene());
                }
            }
            
            obj->sprite->removeFromParentAndCleanup(true);
            world->DestroyBody(obj->body);
            objary->removeObject(obj);
            delete obj;
            obj = NULL;
            

            
            break;
        }

        }
    }
}

void NinjaScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
        CCTouch *touch = (CCTouch *)*it;
        if (_map.find(touch) == _map.end()) continue;
        
        CCBlade *blade = _map[touch];
        blade->autoCleanup();
        _map.erase(touch);
    }
    

}

void NinjaScene::update(float dt)
{
	int32 velocityIterations = 8;
	int32 positionIterations = 1;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(0.04f, velocityIterations, positionIterations);
    
    timetemp += dt;
    if (timetemp>(60.0f/m_levelInfo.Ticks))
    {
        tossobject();
        timetemp = 0.0f;
    }
    
    m_levelInfo.Time -=dt;
    char time_label[10];
    sprintf(time_label, "time:%d", (int)m_levelInfo.Time);
    lbl_Time->setString(time_label);
    
    if ((int)m_levelInfo.Time == 0)
        goGameOver();        
    
    BaseLayer::update(dt);
}

void NinjaScene::tossobject()
{
        if (strcmp("x", levelinfo->getData(m_nTicknum, 13).c_str()) == 0)
        {
            CCLog(levelinfo->getData(m_nTicknum, 4).c_str(),NULL);
            CCLOG("firstcol-%d",atoi(levelinfo->getData(m_nTicknum, 0).c_str()));
            updatelbltarget();
        }
        for (int i = 0; i < 13; i++)
        {
            if (!(strcmp("", levelinfo->getData(m_nTicknum, i).c_str()) == 0))
            {
                Object* obj = new Object;
                obj->create(world, this, i, atoi(levelinfo->getData(m_nTicknum, i).c_str()), lbl_Target->getTag(),wordinfo);
                objary->addObject(obj);
            }            
        }
        
        
        m_nTicknum ++;
}


void NinjaScene::readWordInfo()
{
    wordinfo = new CCArray;
    wordinfo->initWithCapacity(30);
    CCSVParse* csvFile = new CCSVParse();
    csvFile->openFile("russian.csv");
    
    
    int num_Rows = csvFile->getRows();
    for (int i=0; i<num_Rows; i++)
    {
        CCString* s_Num = new CCString();
        CCString* s_Eng = new CCString();
        CCString* s_Tran = new CCString();
        
        int lev = atoi(csvFile->getData(i, 0).c_str());
        if(lev == m_levelInfo.Levelnum)
        {
            CCLOG("levelinfo %d",lev);
            CCLog(csvFile->getData(i, 1).c_str(),NULL);
            string temp = csvFile->getData(i, 1);
            for (int i = 0; i < temp.length(); i++) {
                temp[i] = toupper(temp[i]);
            }
            //            s_Eng -> initWithFormat("%s",csvFile->getData(i, 1).c_str());
            //            s_Tran -> initWithFormat("%s",csvFile->getData(i, 2).c_str());
            s_Eng -> initWithFormat("%s",temp.c_str());
            s_Tran -> initWithFormat("%s",csvFile->getData(i, 1).c_str());
            
            if(s_Num != NULL)
            {
                CCLog("Added");
                CCArray* row = new CCArray;
                row->initWithCapacity(2);
                row->addObject(s_Eng);
                row->addObject(s_Tran);
                wordinfo->addObject(row);
            }
        }
    }
    updatelbltarget();
}

void NinjaScene::updatelbltarget()
{
    int target = arc4random() % wordinfo->count();
    CCArray *targetrow = (CCArray*) wordinfo->objectAtIndex(target);
    CCString *targetword = (CCString*) targetrow->objectAtIndex(0);
    lbl_Target->setString(targetword->getCString());
    lbl_Target->setTag(target);
}

void NinjaScene::menuCallback(CCObject* pSender)
{
    int nTag = ((MyMenuItem*)pSender)->getTag();
    if (nTag == MENU_SOUND_TAG) {
        if (CCUserDefault::sharedUserDefault()->getBoolForKey(SOUNDONOFF))
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey(SOUNDONOFF, false);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setBoolForKey(SOUNDONOFF, true);
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
        }
    }
    else if (nTag == MENU_EFFECT_TAG){
        
    }
    else if (nTag == MENU_HELP_TAG){
        
    }
    else if (nTag == MENU_RETURN_TAG){
        
        onClickMenuBtn();
        
        scheduleOnce(schedule_selector(NinjaScene::goGameSelect), 0.4f);
        
        
    }
}


void NinjaScene::goGameSelect()
{
    CCDirector::sharedDirector()->replaceScene(GameSelectScene::scene());
}

void NinjaScene::goGameOver()
{
    CCSprite* game_over_sprite = CCSprite::create("game_over.png");
    game_over_sprite->setScale(g_fResScale);
    this->addChild(game_over_sprite);
    game_over_sprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->unschedule(schedule_selector(NinjaScene::update));
    this->setTouchEnabled(false);
    this->scheduleOnce(schedule_selector(NinjaScene::goGameSelect), 3.0f);
}

void NinjaScene::removeColorLayer(CCLayerColor* sender)
{
    this->removeChild(sender);
}
                                                     
void NinjaScene::draw()
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
    CCLayer::draw();

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
	
	kmGLPushMatrix();
	
	world->DrawDebugData();
	
	kmGLPopMatrix();
}
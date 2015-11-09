//
//  Config.cpp
//  QuillysRun
//
//  Created by Ry on 2013.9.5
//
//

#include "Config.h"

ccColor4F g_drawColor[MAX_LEVEL] = {
    {0/255.f,   174/255.f,  239/255.f, 1.f},
    {163/255.f, 152/255.f,  252/255.f, 1.f},
    {252/255.f, 32/255.f,   216/255.f, 1.f},
    {61/255.f,  255/255.f,  22/255.f, 1.f},
    {68/255.f,  64/255.f,   231/255.f, 1.f},
    {71/255.f,  55/255.f,   189/255.f, 1.f},
    {176/255.f, 113/255.f,  108/255.f, 1.f},
    {90/255.f,  70/255.f,   232/255.f, 1.f},

};
ccColor4F g_borderColor = {64/255.f,     41/255.f,      229/255.f, 1.f};

ccColor4F g_drawLevelLineColor[MAX_LEVEL] = {
    {255/255.f,     244/255.f,      95/255.f, 1.f},
    {255/255.f,     31/255.f,       216/255.f, 1.f},
    {61/255.f,      255/255.f,      31/255.f, 1.f},
    {0/255.f,       174/255.f,      239/255.f, 1.f},
    {74/255.f,      61/255.f,       173/255.f, 1.f},
    {249/255.f,     160/255.f,      27/255.f, 1.f},
    {140/255.f,     127/255.f,      239/255.f, 1.f},
    {0/255.f,       170/255.f,      173/255.f, 1.f},
    
};

void addGlowEffect(CCSprite* sprite,
                   
                   const ccColor3B& colour, const CCSize& size)

{
    
    CCPoint pos = ccp(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    CCSprite* glowSprite = CCSprite::create("particle-fire.png");
    
    glowSprite->setColor(colour);
    glowSprite->setPosition(pos);
    glowSprite->setRotation(sprite->getRotation());
    
    _ccBlendFunc f = {GL_ONE, GL_ONE};
    glowSprite->setBlendFunc(f);
    sprite->addChild(glowSprite, -1);
    
    // Run some animation which scales a bit the glow
    
    CCSequence* s1 = CCSequence::create(CCScaleTo::create(0.9f, size.width, size.height),
                                        CCScaleTo::create(0.9f, size.width*0.75f, size.height*0.75f), NULL);
    CCRepeatForever* r = CCRepeatForever::create(s1);
    
    glowSprite->runAction(r);
    
}
// According to the map data, determine what object is at the position
GameRoleEnum getRoleEnum(char role)
{
	if ("H"[0] == role || "h"[0] == role || "V"[0] == role || "v"[0] == role || "G"[0] == role || "g"[0] == role ||
        "R"[0] == role || "r"[0] == role || "Y"[0] == role || "y"[0] == role || "L"[0] == role || "l"[0] == role || "e"[0] == role || '_' == role) {
		return GameRoleWallEnum;
	}
	if("p"[0] == role){
		return GameRoleGhostEnum;
	}
	if("m"[0] == role){
		return GameRoleManEnum;
	}
	if ("."[0] == role) {
		return GameRoleSmallPeaEnum;
	}
	if("*"[0] == role){
		return GameRoleBigPeaEnum;
	}
    if("q"[0] == role){
		return GameRoleExitEnum;
	}
    if("X"[0] == role){
		return GameRoleCrossEnum;
	}
    if("T"[0] == role){
		return GameRoleTopWayEnum;
	}
    if("Z"[0] == role){
		return GameRoleHomeEnum;
	}
    
	return GameRoleNoneEnum;
}

void Config::setScaleFactor(CCSize size)
{
    _scaleX = size.width / DESIGN_RESOLUTION_WIDTH;
    _scaleY = size.height / DESIGN_RESOLUTION_HEIGHT;
}

void Config::setScale(CCNode *node)
{
    node->setScaleX(_scaleX);
    node->setScaleY(_scaleY);
    
}

void Config::setScale(CCNode *node, float factorX, float factorY)
{
    node->setScaleX(_scaleX * factorX);
    node->setScaleY(_scaleY * factorY);
}

float Config::getX(float x)
{
    return _scaleX * x;
}

float Config::getyScale()
{
    return _scaleY;
}

float Config::getxScale()
{
    return _scaleX;
}

float Config::getY(float y)
{
    return _scaleY * y;
}

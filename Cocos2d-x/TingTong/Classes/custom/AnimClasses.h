//
//  AnimClasses.h
//  TingTong
//
//  Created by jyn on 4/11/14.
//
//

#ifndef __TingTong__AnimClasses__
#define __TingTong__AnimClasses__

#include "cocos2d.h"
#include "GameUtils.h"

USING_NS_CC;

float nextFloat();

class AnimParams
{
public:
    virtual bool Update(float dt){return false;}
    virtual CCPoint getPos(CCPoint pt){return pt;}
    virtual float getRot(float rot){return rot;}
    virtual float getScale(float scale){return scale;}
    virtual int getColor(int clr){return clr;}
    virtual int getColor(int clr, int idx){return getColor(clr);}
};


class AnimPulse  : public  AnimParams
{
private:
    float CurTime = 0.0f;
    float Interp = 0.0f;
    float Cycle;
    
    float Scale1, Scale2;
    int Color1, Color2;
public:
    AnimPulse(float cycle, float scl1, float scl2, int clr1, int clr2);
    AnimPulse(float cycle, float scl1, float scl2, int clr1, int clr2, float offset);
    bool Update(float dt);
    float getScale(float scale);
    int getColor(int clr);
};


class AnimTravel : public AnimParams
{
protected:
    int FromX, FromY, ToX, ToY;
    float EndTime  = 0.0f, CurTime = 0.0f;
    
public:
    AnimTravel(float travelTime, int fromX, int fromY, int toX, int toY, int cX, int cY)
    {
        EndTime = travelTime;
        FromX = fromX;
        FromY = fromY;
        ToX = toX;
        ToY = toY;
        
        if(cX != 0 && FromX != ToX)
        {
            float s = (float)(cX - FromX) / (float)(ToX - FromX);
            CurTime = s*travelTime;
        }
        if(cY != 0 && FromY != ToY)
        {
            float s = (float)(cY - FromY) / (float)(ToY - FromY);
            CurTime = s*travelTime;
        }
    }
    bool Update(float dt)
    {
        CurTime += dt;
        if(CurTime > EndTime)
            CurTime = fmodf(CurTime, EndTime);
        return true;
    }
    CCPoint getPos(CCPoint pt)
    {
        float s = CurTime/EndTime;
        float nx = FromX*(1.0f - s) + ToX*s;
        float ny = FromY*(1.0f - s) + ToY*s;
        pt = CCPoint((int)ceil(nx+0.5), (int)ceil(ny+0.5));
        return pt;
    }
};


class AnimWave : public AnimParams
{
protected:
    int FromX, FromY, ToX, ToY;
    float WaitTime, EndTime, CurTime = 0.0f;
    
public:
    AnimWave(float travelTime, float waitTime, int fromX, int fromY, int toX, int toY, int cX, int cY)
    {
        EndTime = travelTime+waitTime;
        WaitTime = waitTime;
        FromX = fromX;
        FromY = fromY;
        ToX = toX;
        ToY = toY;
        
        if(cX != 0 && FromX != ToX)
        {
            float s = (float)(cX - FromX) / (float)(ToX - FromX);
            CurTime = s*travelTime;
        }
        if(cY != 0 && FromY != ToY)
        {
            float s = (float)(cY - FromY) / (float)(ToY - FromY);
            CurTime = s*travelTime;
        }
    }
    bool Update(float dt)
    {
        CurTime += dt;
        if(CurTime > EndTime)
            CurTime = fmodf(CurTime, EndTime);
        return true;
    }
    CCPoint getPos(CCPoint pt)
    {
        if(CurTime < WaitTime)
        {
            pt = CCPoint(FromX, FromY);
            return pt;
        }
        
        float s = (CurTime - WaitTime)/(EndTime - WaitTime);
        float nx = FromX*(1.0f - s) + ToX*s;
        float ny = FromY*(1.0f - s) + ToY*s;
        pt = CCPoint((int)ceil(nx+0.5), (int)ceil(ny+0.5));
        return pt;
    }
    int getColor(int clr)
    {
        if(CurTime < WaitTime)
            return 0;
        
        float s = (CurTime - WaitTime)/(EndTime - WaitTime);
        float sins = (float)sin(s*M_PI);
        
        return GameUtils::interpolateColor(0x00FFFFFF, 0xEEFFFFFF, sins);
    }
};


class AnimTravelOffset : public AnimParams
{
protected:
    int FromX, FromY, ToX, ToY;
    float EndTime, CurTime = 0.0f;
    
    float RotSpeed;
    float Rot = 0;
    
    int OffWidth, OffHeight;
    float OffWidthSpeed, OffHeightSpeed;
    float TimeW = 0, TimeH = 0;
public:
    AnimTravelOffset(float travelTime, int fromX, int fromY, int toX, int toY, int cX, int cY, float rotSpeed, float horSpeed, float vertSpeed, int horSize, int vertSize)
    {
        EndTime = travelTime;
        FromX = fromX;
        FromY = fromY;
        ToX = toX;
        ToY = toY;
        CurTime = 0.0f;
        RotSpeed = rotSpeed;
        OffWidth = horSize;
        OffHeight = vertSize;
        OffWidthSpeed = horSpeed;
        OffHeightSpeed = vertSpeed;
        
        if(fabsf(RotSpeed) > 0.01f)
            Rot = nextFloat()*360;
        
        TimeW = (float)(nextFloat()*(M_PI*2.0f));
        TimeH = (float)(nextFloat()*(M_PI*2.0f));
        
        if(cX != 0 && FromX != ToX)
        {
            float s = (float)(cX - FromX) / (float)(ToX - FromX);
            CurTime = s*travelTime;
        }
        if(cY != 0 && FromY != ToY)
        {
            float s = (float)(cY - FromY) / (float)(ToY - FromY);
            CurTime = s*travelTime;
        }
    }
    bool Update(float dt)
    {
        CurTime += dt;
        if(CurTime > EndTime)
            CurTime = fmodf(CurTime, EndTime);
        
        Rot += RotSpeed*dt;
        if(Rot < 0)
            Rot += 360;
        if(Rot > 360)
            Rot -= 360;
		
        TimeW += OffWidthSpeed*dt;
        if(TimeW > M_PI*2.0f)
            TimeW = fmodf(TimeW, (M_PI*2.0f));
        
        TimeH += OffHeightSpeed*dt;
        if(TimeH > M_PI*2.0f)
            TimeH = fmodf(TimeH, (M_PI*2.0f));
        
        return true;
    }
    CCPoint getPos(CCPoint pt)
    {
        float s = CurTime/EndTime;
        float nx = FromX*(1.0f - s) + ToX*s + (float)(sin(TimeW)*OffWidth);
        float ny = FromY*(1.0f - s) + ToY*s + (float)(sin(TimeH)*OffHeight);
        pt = CCPoint(round(nx+0.5), round(ny+0.5));
        return pt;
    }
    float getRot(float rot)
    {
        return Rot;
    }
};
class AnimSparkleRandom : public AnimParams
{
protected:
    //    Random Rand;
    float AnimTime;
    float WaitTimeMin;
    float WaitTimeMax;
    float WaitTime;
    float CurTime;
    
public:
    AnimSparkleRandom(float animTime, float waitTimeMin, float waitTimeMax)
    {
        //        Rand = rand;
        AnimTime = animTime;
        WaitTimeMin = waitTimeMin;
        WaitTimeMax = waitTimeMax;
        WaitTime = waitTimeMin + nextFloat() * (waitTimeMax-waitTimeMin);
        CurTime = nextFloat() * (WaitTime+AnimTime);
    }
    
    bool Update(float dt)
    {
        CurTime += dt;
        if(CurTime > WaitTime+AnimTime)
        {
            CurTime = 0.0f;
            WaitTime = WaitTimeMin + nextFloat() * (WaitTimeMax-WaitTimeMin);
            
        }
        
        return true;
    }
    float getRot(float rot)
    {
        if(CurTime < WaitTime)
            return 0;
        
        float s = (CurTime - WaitTime)/(AnimTime);
        return 90*s;
    }
    int getColor(int clr)
    {
        if(CurTime < WaitTime)
            return 0;
        
        float s = (CurTime - WaitTime)/(AnimTime);
        float sins = (float)sin(s*M_PI);
        
        return GameUtils::interpolateColor(0x00FFFFFF, 0xFFFFFFFF, sins);
    }
};

#endif /* defined(__TingTong__AnimClasses__) */

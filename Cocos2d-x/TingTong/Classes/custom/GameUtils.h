//
//  GameUtils.h
//  LingLing
//
//  Created by jyn on 3/8/14.
//
//

#ifndef __LingLing__GameUtils__
#define __LingLing__GameUtils__

#include "TypeDef.h"
#include "cocos2d.h"

class GameUtils
{
public:
    static int clampColor(float v)
	{
		if(v <= 0.0f)
			return 0;
		if(v >= 1.0f)
			return 255;
		
		return (int)(v*255.0f);// + 0.5f);
	};
    
	static int buildColor(float r, float g, float b)
	{
		return 0xFF000000 | (clampColor(r) << 16) | (clampColor(g) << 8) | clampColor(b);
	};
    
	static int buildColor(float r, float g, float b, float a)
	{
		return (clampColor(a) << 24) | (clampColor(r) << 16) | (clampColor(g) << 8) | clampColor(b);
	};
	
	static int clampInt(int v, int from, int to)
	{
		if(v < from)
			return from;
		if(v > to)
			return to;
		return v;
	};
    
	static float clampFloat(float v, float from, float to)
	{
		if(v < from)
			return from;
		if(v > to)
			return to;
		return v;
	};
	
	static float getA(int c)
	{
		return (float)((c>>24)&0xFF) / 255.0f;
	};
    
	static float getR(int c)
	{
		return (float)((c>>16)&0xFF) / 255.0f;
	};
    
	static float getG(int c)
	{
		return (float)((c>>8)&0xFF) / 255.0f;
	};
    
	static float getB(int c)
	{
		return (float)(c&0xFF) / 255.0f;
	};
	
	static int interpolateColor(int c1, int c2, float t)
	{
		if(t <= 0.0f)
			return c1;
		if(t >= 1.0f)
			return c2;
		
		float a = getA(c1)*(1.0f - t) + getA(c2)*t;
		float r = getR(c1)*(1.0f - t) + getR(c2)*t;
		float g = getG(c1)*(1.0f - t) + getG(c2)*t;
		float b = getB(c1)*(1.0f - t) + getB(c2)*t;
		
//        int al = clampColor(a);
//        al = al << 24;
		return (clampColor(a) << 24) | (clampColor(r) << 16) | (clampColor(g) << 8) | clampColor(b);
	};
    
	static int interpolateColor(int c1, int c2, int c3, float t)
	{
		if(t <= 0.0f)
			return c1;
		if(t >= 1.0f)
			return c3;
		
		if(t < 0.5f)
			return interpolateColor(c1, c2, t*2.0f);
		
		return interpolateColor(c2, c3, (t-0.5f)*2.0f);
	};
    
	static int interpolateColor(int c1, int c2, int c3, int c4, float t)
	{
		if(t <= 0.0f)
			return c1;
		if(t >= 1.0f)
			return c4;
		
		if(t < 1.0f/3.0f)
			return interpolateColor(c1, c2, t*3.0f);
		else if(t < 2.0f/3.0f)
			return interpolateColor(c2, c3, (t-1.0f/3.0f)*3.0f);
		else
			return interpolateColor(c3, c4, (t-2.0f/3.0f)*3.0f);
	};
    
	static int interpolateColor(int c1, int c2, int c3, float t, float split)
	{
		if(t <= 0.0f)
			return c1;
		if(t >= 1.0f)
			return c3;
		
		float eps = 0.0001f;
		if(split > 1.0f - eps)
			split = 1.0f - eps;
		if(split < eps)
			split = eps;
		
		if(t < split)
			return interpolateColor(c1, c2, t/split);
		
		return interpolateColor(c2, c3, (t-split)/(1.0f - split));
	};
    
	
	static int buildInt(byte b1, byte b2, byte b3, byte b4)
	{
		return ((b1&0xFF) << 24) | ((b2&0xFF) << 16) | ((b3&0xFF) << 8) | (b4&0xFF);
	};
	
	static short buildShort(byte b1, byte b2)
	{
		return (short)(((b1&0xFF) << 8) | (b2&0xFF));
	};
	
	static float interpolateAngleDeg(float a1, float a2, float t)
	{
		if(fabsf(a2 - a1) < 180.0f)
			return a1*(1.0f - t) + a2*t;
		else if(a2 > a1)
			return (a1+360.0f)*(1.0f - t) + a2*t;
		else
			return (a1)*(1.0f - t) + (a2+360.0f)*t;
	};
	
	static float interpolateAngle(float a1, float a2, float t)
	{
		float ret = 0.0f;
        
		if(a1 > M_PI*2.0f)
			a1 = fmodf(a1, (M_PI*2.0f));
		else if(a1 < 0.0f)
			a1 = (float)(M_PI*2.0f - fmodf(-a1, (M_PI*2.0f)));
		
		if(a2 > M_PI*2.0f)
			a2 = (float)(fmodf(a2, (M_PI*2.0f)));
		else if(a2 < 0.0f)
			a2 = (float)(M_PI*2.0f - fmodf(-a2, (M_PI*2.0f)));
        
		
		if(fabsf(a1-a2) < fabsf(a1-(a2+M_PI*2.0f)) && fabsf(a1-a2) < fabsf(a1-(a2-M_PI*2.0f)))
		{
			ret = a1*(1.0f - t) + a2*t;
		}
		else if(fabsf(a1-(a2+M_PI*2.0f)) < fabsf(a1-(a2-M_PI*2.0f)))
		{
			ret = (float)(a1*(1.0f - t) + (a2+M_PI*2.0f)*t);
		}
		else
		{
			ret = (float)(a1*(1.0f - t) + (a2-M_PI*2.0f)*t);
		}
		return (float)(fmodf(ret , (M_PI*2.0f)));
	}
	
//	static void shuffle(int* table, Random rnd)
//	{
//		int l = table.length;
//		if(l < 3)
//			return;
//		int s, ni;
//		for(int i = 0; i < l; i++)
//		{
//			ni = rnd.nextInt(l);
//			s = table[i];
//			table[i] = table[ni];
//			table[ni] = s;
//		}
//	}

};

#endif /* defined(__LingLing__GameUtils__) */

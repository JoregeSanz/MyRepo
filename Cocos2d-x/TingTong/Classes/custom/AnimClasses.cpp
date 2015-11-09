//
//  AnimClasses.cpp
//  TingTong
//
//  Created by jyn on 4/11/14.
//
//

#include "AnimClasses.h"

float nextFloat()
{
    int rnd = arc4random() % 255;
    float res  = (float)rnd / 255.0f;
    return res;
}

//  PolygonSprite.h
//  iChop
//
//  Created by zamfir dumitru on 17/06/13.
//
//

#ifndef __iChop__PolygonSprite__
#define __iChop__PolygonSprite__

#include <iostream>
#include "cocos2d.h"
#include "PRFilledPolygon.h"
#include "Box2D.h"
//#include "GLES-Render.h"
#include "polypartition.h"

#define PTM_RATIO 32

#define calculate_determinant_2x2(x1,y1,x2,y2) x1*y2-y1*x2
#define calculate_determinant_2x3(x1,y1,x2,y2,x3,y3) x1*y2+x2*y3+x3*y1-y1*x2-y2*x3-y3*x1


//Struct that can hold b2Vec2 array and count of that array. same as the one from GameScene.h but we can't include that file because problems will occure
typedef struct{
    b2Vec2 *array;
    int32 count;
}vecArrWithCount;


class PolygonSprite:public PRFilledPolygon{
public:
    bool original;
    b2Body *body;//reference of body from GameScene
    b2World *theWorld;//reference of world from GameScene
    //GLESDebugDraw *debugDraw;
    b2Vec2 centroid;
 
    //temp fuctions for debugging the letters world, a bit of a hack really...
    b2World *lettersWorld;//for debuging
    //GLESDebugDraw *debugDrawLettersWrld;
    void setLettersWorldForDebugging(b2World *world);
    
    bool sliceEntered;
    bool sliceExited;
    b2Vec2 entryPoint;
    b2Vec2 exitPoint;
    double sliceEntryTime;
    
    
    //These 3 lines help with the slicing a choppable when slice point started inside the choppable 
    bool slicePointExitedFromPolygon;
    b2Vec2 slicePointStartedInside;//if the slice started from inside the choppable, store the point in this variable
    b2Vec2 slicePointExitFromInside;
    
    virtual bool initWithTexture(CCTexture2D *texture, b2Body *bodyArg, bool originalArg);
    static PolygonSprite *spriteWithTexture(CCTexture2D *texture, b2Body *bodyArg, bool originalArg);
    
    b2Vec2 *arrangeVertices(b2Vec2 *vertices, int count);
    
    b2Body *createBodyForWorld(b2World * world, b2Vec2 position, float rotation, b2Vec2 *vertices, int32 vertexCount, float density, float friction, float restitution);
    void activateCollision();
    void deactivateCollsion();
    void setPosition(CCPoint position);
    CCAffineTransform nodeToParentTransform();
    
    void moveBodyBy(CCPoint originalPos, CCPoint moveBy, float duration);
    void stopMovingBody();
    bool moveBodyAccoringToFunction;
    CREATE_FUNC(PolygonSprite);

};

#endif /* defined(__iChop__PolygonSprite__) */












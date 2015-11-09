//
//  PolygonSprite.cpp
//  iChop
//
//  Created by zamfir dumitru on 17/06/13.
//
//

#include "PolygonSprite.h"
using namespace cocos2d;

//used by arrageVertices function
int comparator_PolySpr(const void *a, const void *b) {
    const b2Vec2 *va = (const b2Vec2 *)a;
    const b2Vec2 *vb = (const b2Vec2 *)b;
    
    if (va->x > vb->x) {
        return 1;
    } else if (va->x < vb->x) {
        return -1;
    }
    return 0;
}

//partitioning concave polygons.... dfunction prototype
vecArrWithCount polygonFixtureVertices(TPPLPoly *vertices);
vecArrWithCount polygonFixtures(list<TPPLPoly> *polyFixt);





PolygonSprite *PolygonSprite::spriteWithTexture(CCTexture2D *texture, b2Body *bodyArg, bool original){
    
    PolygonSprite *polySprite = new PolygonSprite();
    if(polySprite && polySprite->initWithTexture(texture, bodyArg, original)){
        polySprite->autorelease();
        return polySprite;
    }
    CC_SAFE_DELETE(polySprite);
    return NULL;
}





bool PolygonSprite::initWithTexture(CCTexture2D *texture, b2Body *bodyArg, bool originalArg){
    if(PRFilledPolygon::init()){
        //std::cout<<"\nPolygonSprite::initWithTexture was called\n";
    
        
        sliceEntered = false;
        sliceExited = false;
        entryPoint.SetZero();
        exitPoint.SetZero();
        slicePointStartedInside.SetZero();
        slicePointExitFromInside.SetZero();
        slicePointExitedFromPolygon = false;
        sliceEntryTime = 0.0;
        
        moveBodyAccoringToFunction = false;
        
        
        
        
        /*CREATING THE TEXTURE OF THE CHOPPABLE*/
        
        
        //only one fixture
        b2Fixture *fixture = (b2Fixture *)bodyArg->GetFixtureList();
        b2PolygonShape *shape = (b2PolygonShape*)fixture->GetShape();
        int vertexCount = shape->GetVertexCount();
        
        Vector2dVector vec;
        for(int i = 0;i<vertexCount;i++){
            vec.push_back(Vector2d(shape->GetVertex(i).x*PTM_RATIO, shape->GetVertex(i).y*PTM_RATIO));
        }
        
        //Now create texture with the vertices we gatherd from all the fixtures
        this->initWithPointsAndTexture(vec, texture);//call to PRFilledPolygon
        
        /*TEXTURE OF THE CHOPPABLE HAS BEEN CREATED*/
        
        
        
        body = bodyArg;
        body->SetUserData(this);
    
        original = originalArg;
        centroid = this->body->GetLocalCenter();
        this->setAnchorPoint(ccp(centroid.x * PTM_RATIO / texture->getContentSize().width,
                                 centroid.y * PTM_RATIO / texture->getContentSize().height));
        
        
        //debug code
        theWorld = body->GetWorld();
        
        /*
        //DEBUG DRAWING
        debugDraw = new GLESDebugDraw(PTM_RATIO);
        theWorld->SetDebugDraw(debugDraw);
        
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        debugDraw->SetFlags(flags);
        */
            
        return true;
    }
    return false;
}



//temp func for drawing the letters world, fastest way to do it
void PolygonSprite::setLettersWorldForDebugging(b2World *world){
    
    /*
    //debug code
    lettersWorld = world;
    //DEBUG DRAWING
    debugDrawLettersWrld = new GLESDebugDraw(PTM_RATIO);
    lettersWorld->SetDebugDraw(debugDrawLettersWrld);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    debugDrawLettersWrld->SetFlags(flags);
    */

}


b2Body *PolygonSprite::createBodyForWorld(b2World *world, b2Vec2 position, float rotation, b2Vec2 *vertices, int32 vertexCount, float density, float friction, float restitution){
    //std::cout<<"\ncreateBodyForWorld was called";

    
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    bodyDef.angle = rotation;
    b2Body *aBody = world->CreateBody(&bodyDef);

    
    //create body with only one fixture, requires convex vertices
     b2FixtureDef fixtureDef;
     fixtureDef.density = density;
     fixtureDef.friction = friction;
     fixtureDef.restitution = restitution;
     fixtureDef.filter.categoryBits = 0;
     fixtureDef.filter.maskBits = 0;
     
     b2PolygonShape shape;
     shape.Set(vertices, vertexCount);
     fixtureDef.shape = &shape;
     aBody->CreateFixture(&fixtureDef);
    
    
    
    
    
    return aBody;

}








//arrange in counter-clockwise order
b2Vec2 *PolygonSprite::arrangeVertices(b2Vec2 *vertices, int count){
    float determinant;
    int iCounterClockWise = 1;
    int iClockWise = count - 1;
    b2Vec2 referencePointA,referencePointB;
    b2Vec2 *sortedVertices = (b2Vec2*)calloc(count, sizeof(b2Vec2));
    //sort all vertices in ascending order according to their x-coordinate so you can get two points of a line
    qsort(vertices, count, sizeof(b2Vec2), comparator_PolySpr);
    sortedVertices[0] = vertices[0];
    referencePointA = vertices[0];          //leftmost point
    referencePointB = vertices[count-1];    //rightmost point
    
    //you arrange the points by filling our vertices in both clockwise and counter-clockwise directions using the determinant function
    for (int i=1;i<count-1;i++)
    {
        determinant = calculate_determinant_2x3(referencePointA.x, referencePointA.y, referencePointB.x, referencePointB.y, vertices[i].x, vertices[i].y);
        
     
        if (determinant<0){sortedVertices[iCounterClockWise++] = vertices[i];}
        else{sortedVertices[iClockWise--] = vertices[i];}//endif
    
    }//endif
    sortedVertices[iCounterClockWise] = vertices[count-1];
    return sortedVertices;
    
}



void PolygonSprite::activateCollision(){
  
    for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2Filter filter = fixture->GetFilterData();
        filter.categoryBits = 0x0001;
        filter.maskBits = 0x0001;
        fixture->SetFilterData(filter);
        
    }
}


void PolygonSprite::deactivateCollsion(){

    for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {    b2Filter filter = fixture->GetFilterData();
        filter.categoryBits = 0;
        filter.maskBits = 0;
        fixture->SetFilterData(filter);
    }

}



void PolygonSprite::setPosition(cocos2d::CCPoint position){

    PRFilledPolygon::setPosition(position);
    body->SetTransform(b2Vec2(position.x/PTM_RATIO, position.y/PTM_RATIO),0);


}

void PolygonSprite::stopMovingBody(){
    std::cout<<"stopMovingBody was called";
    this->removeChildByTag(6464, true);
    moveBodyAccoringToFunction = false;
}

//move b2body by cloning node transformations
void PolygonSprite::moveBodyBy(cocos2d::CCPoint originalPos, cocos2d::CCPoint moveBy, float duration){

    CCNode *node = CCNode::create();
    node->setPosition(originalPos.x, originalPos.y+320);
    this->addChild(node);
    node->setTag(6464);
    node->runAction(CCJumpBy::create(duration, moveBy, 450, 1));
    moveBodyAccoringToFunction = true;
    
    //int rotateBy = 10;
    //if(moveBy.x > 0){rotateBy = -10;}
    //node->runAction(CCRotateBy::create(duration, rotateBy));
    
    CCDelayTime *delay = CCDelayTime::create(duration);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(PolygonSprite::stopMovingBody));
    this->runAction(CCSequence::create(delay, call, NULL));
}





CCAffineTransform PolygonSprite::nodeToParentTransform(){
    //if(body){std::cout<<"\nPolygonSprite::nodeToParentTransform was called on body: "<<body;}else{std::cout<<"\nads"<<body;}
    
    /*
    //debug code
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    theWorld->DrawDebugData();
    kmGLPopMatrix();
    
    //temp debug for letters world
    if(lettersWorld){lettersWorld->DrawDebugData();}
   */
    
    
    
    if(moveBodyAccoringToFunction){
        this->setPosition(ccp(this->getChildByTag(6464)->getPosition().x,this->getChildByTag(6464)->getPosition().y));
    }
    
    
    b2Vec2 pos = body->GetPosition();
    
    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    
    
    if (isIgnoreAnchorPointForPosition()) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }
    
    // Make matrix
    float radians = body->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);
    
    if(!m_obAnchorPointInPoints.equals(CCPointZero)){
        x += c*-m_obAnchorPointInPoints.x+ -s*-m_obAnchorPointInPoints.y;
        y += s*-m_obAnchorPointInPoints.x+ c*-m_obAnchorPointInPoints.y;
    }
    
    // Rot, Translate Matrix
    CCAffineTransform transform = CCAffineTransformMake(c, s,
                                                        -s,c,
                                                        x,y );
    
    return transform;
     
}





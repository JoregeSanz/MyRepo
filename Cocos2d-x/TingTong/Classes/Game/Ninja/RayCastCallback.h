//
//  b2RayCastCallback.h
//  iChop
//
//  Created by zamfir dumitru on 18/06/13.
//
//

#ifndef iChop_RayCastCallback_h
#define iChop_RayCastCallback_h

#define collinear(x1,y1,x2,y2,x3,y3) fabsf((y1-y2) * (x1-x3) - (y1-y3) * (x1-x2))

#include "Box2D.h"
#include "PolygonSprite.h"
#include "cocos2d.h"
class RayCastCallback : public b2RayCastCallback{
public:
    RayCastCallback(){
    }
    
    //finds which fixture the ray hit, then this func is called
    float32 ReportFixture(b2Fixture *fixture,const b2Vec2 &point,const b2Vec2 &normal,float32 fraction){
        //std::cout<<"\nReportFixture was called\n";
    
           
        //check if ray intersected polygon sprite
        PolygonSprite *polygonSpr = (PolygonSprite*)fixture->GetBody()->GetUserData();
        if(!polygonSpr){
            //std::cout<<"Not a polygon sprite";
            return 0;}//there are other bodies that get ray cast. we don't want a crash
        
        if (!polygonSpr->sliceEntered){
            polygonSpr->sliceEntered = true;
            //you need to get the point coordinates within the shape
            polygonSpr->entryPoint = polygonSpr->body->GetLocalPoint(point);
            
            struct timeval tv;gettimeofday(&tv, NULL);
            int32 secs = tv.tv_sec%100000;
            float32 milSecs = tv.tv_usec/10000;
            float32 newSecs = secs+(milSecs/100);
     
            polygonSpr->sliceEntryTime = newSecs+1;
            //CCLog("\nSlice entered at world coords: %f,%f and polygon coords: %f,%f\n",point.x*PTM_RATIO,point.y*PTM_RATIO,
            //                                        polygonSpr->entryPoint.x*PTM_RATIO,polygonSpr->entryPoint.y*PTM_RATIO);
            
           
        }//for the slice that slices polygon even if it started from inside the polygon
        else if(!polygonSpr->slicePointExitedFromPolygon){
        
    
            polygonSpr->slicePointExitFromInside = polygonSpr->body->GetLocalPoint(point);
            polygonSpr->slicePointExitedFromPolygon = true;
            
            
        }//for normal slice that started from outside the polygon
        
         else if (!polygonSpr->sliceExited){
            
            polygonSpr->exitPoint = polygonSpr->body->GetLocalPoint(point);
            
            b2Vec2 entrySide = polygonSpr->entryPoint - polygonSpr->centroid;
            b2Vec2 exitSide = polygonSpr->exitPoint - polygonSpr->centroid;
            if (entrySide.x * exitSide.x > 0 || entrySide.y * exitSide.y > 0){
  
                polygonSpr->sliceExited = true;//this means it will get cut. the rest down there \/ means it won't get cut
            }
            /*else{//usless code down here
                CCLog("else statement ran!");
                //if the cut didn't cross the centroid, you check if the entry and exit point lie on the same line
                b2Fixture *fixture = polygonSpr->body->GetFixtureList();
                b2PolygonShape *polygon = (b2PolygonShape*)fixture->GetShape();
                int count = polygon->GetVertexCount();
                
                bool onSameLine = false;
                for (int i = 0 ; i < count; i++){
                    b2Vec2 pointA = polygon->GetVertex(i);
                    b2Vec2 pointB;
                    
                    if (i == count - 1){
                        pointB = polygon->GetVertex(0);
                    }
                    else {
                        pointB = polygon->GetVertex(i+1);
                    }//endif
                    
                    float collinear = collinear(pointA.x,pointA.y,polygonSpr->entryPoint.x,polygonSpr->entryPoint.y, pointB.x,pointB.y);
                    
                    if (collinear <= 0.00001){
                        float collinear2=collinear(pointA.x,pointA.y,polygonSpr->exitPoint.x,polygonSpr->exitPoint.y,pointB.x,pointB.y);
                        if (collinear2 <= 0.00001){
                            onSameLine = true;
                        }
                        break;
                    }//endif
                }//endfor
                
                if (onSameLine){
                    polygonSpr->entryPoint = polygonSpr->exitPoint;
                    polygonSpr->sliceEntryTime = clock()+1;
                    polygonSpr->sliceExited = false;
                    std::cout<<"\ncout test 2";
                }
                else {
                    polygonSpr->sliceExited = false;
                    std::cout<<"\ncout test 3";
                }//endif
            }
            */
             
            //CCLog("Slice exited at world coords:%f,%f and polygon coords:%f,%f",point.x*PTM_RATIO,point.y*PTM_RATIO,
            //                                    polygonSpr->exitPoint.y*PTM_RATIO,polygonSpr->exitPoint.y*PTM_RATIO);
   
        }
        return 1;
    }
};


#endif

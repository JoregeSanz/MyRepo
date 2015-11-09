#ifndef _SClipNode_H
#define _SClipNode_H

#include "cocos2d.h"
USING_NS_CC;


namespace   cocos2d {
    
    class ClipNode : public CCNode
    {
                
        CCRect scissorRect; //in pixels
        CCRect clippingRegionInNodeCoordinates;
    public:
        //
        ClipNode();
        ~ClipNode();
       
        void setClippingRegion( CCRect clipRect);
       
        CC_SYNTHESIZE(bool,clipsToBounds,ClipsToBounds);
        
        //Overloading C++ methods
        
      	virtual void visit(void);
              
        void setScale(float newscale);
        
      

    };


}//namespace   cocos2d 
#endif

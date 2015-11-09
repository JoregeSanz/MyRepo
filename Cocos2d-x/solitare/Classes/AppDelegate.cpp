#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    log(" *********  1  ********* ");

    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);

        log(" *********  2   ********* ");
    }

    log(" ********  3   ********** ");

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    //set design resolution
    glview->setDesignResolutionSize(DESIGN_WIDTH_LANDSCAPE, DESIGN_HEIGHT_LANDSCAPE, ResolutionPolicy::NO_BORDER);
    
    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    auto scene = GameScene::createScene();

    // run
    director->runWithScene(scene);

    //preload sounds
    Utils::preloadSounds();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    Utils::share()->saveGameState();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

#pragma mark - auto orientation

void AppDelegate::onOrientationChange(int width, int height)
{
    return;
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
//    glview->setFrameSize(width, height);
//    
//    //set design resolution
//    if( width < height ) {
//        
//        glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::SHOW_ALL);
//    }
//    else {
//        
//        glview->setDesignResolutionSize(DESIGN_HEIGHT, DESIGN_WIDTH, ResolutionPolicy::SHOW_ALL);
//    }
    
    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    auto scene = GameScene::createScene();
    
    // run
    director->replaceScene(scene);
}

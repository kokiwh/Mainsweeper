#include "AppDelegate.h"
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
    if(!glview) {
        glview = GLViewImpl::create("Mainsweeper");
        director->setOpenGLView(glview);
    }

    // 画像調整　参考 http://gaomar.blog.fc2.com/blog-entry-5.html cocos2d-x のversionがちがうためソース微妙に違う
    // kResolutionShowAll   縦横比を保って画面一杯に表示
    // kResolutionExactFit  縦横比を保たないで、引き伸ばされた感じで表示
    // kResolutionNoBorder  縦横比を保って、全体表示する感じです。
    glview->setDesignResolutionSize(1364, 768, ResolutionPolicy::NO_BORDER);


    
    // BGM・SE
    auto audio = SimpleAudioEngine::getInstance();
    audio->setBackgroundMusicVolume(0.5f);
    audio->setEffectsVolume(0.3f);
    audio->preloadBackgroundMusic("bgm/main.mp3");
    audio->preloadEffect("se/click.mp3");
    audio->playBackgroundMusic("bgm/main.mp3", true);


    // 左下の情報
#if (DEBUG)
    director->setDisplayStats(true);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);


    // create a scene. it's an autorelease object
    auto scene = Game::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

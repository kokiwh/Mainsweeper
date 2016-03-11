#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Config.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

class Game : public cocos2d::Layer
{
protected:
    
    Size _visibleSize;
    Size _winSize;
    Vec2 _origin;
    
    SimpleAudioEngine* _audio;

    // 背景の表示関連 //
    cocos2d::Sprite* m_background;

    // パネルの一辺のサイズを記録する
    float m_panelSize;

    vector<Sprite*> panelBlocks;
    
    void touchEnabled(bool flg);
    
    void initForVariables();
    void showBackground();
    void initPanel();
    Point getPosition(int posIndexX, int posIndexY);
    void showPanel();

    void getTouchPanelTag(Point touchPoint);
    int getTag(int posIndexX, int posIndexY);
    void showGameOver(Sprite* sprite_panel);
    void showOpenPanel(Sprite* sprite_panel);
    void replaceImage(Sprite* sprite_panel, string file_name);
    double getSec();
    void searchBomb(Sprite* sprite_panel);
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);

    // タップ開始時に処理されるメソッドを宣言
    virtual bool TouchBegan(Touch* pTouch, Event* pEvent);
    // タップ終了時に処理されるメソッドを宣言
    virtual void TouchEnded(Touch* pTouch, Event* pEvent);
};

#endif // __GAME_SCENE_H__

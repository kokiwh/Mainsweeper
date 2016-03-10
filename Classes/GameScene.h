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

    
    // コマの一辺のサイズを記録する
    float m_blockSize;

    // コマの種類ごとのグループ
    map<Config::kBlock, list<int>> m_blockTags;


    void initForVariables();
    void showBackground();
    int getTag(int posIndexX, int posIndexY);
    void showBlock();
    vector<Config::kBlock> blockTypes;
    Point getPosition(int posIndexX, int posIndexY);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Game);
};

#endif // __GAME_SCENE_H__

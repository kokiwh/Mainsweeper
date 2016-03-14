#ifndef __PANEL_SPRITE_H__
#define __PANEL_SPRITE_H__

#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;

class PanelSprite : public cocos2d::Sprite
{
private:
//    int _position;
    EventListener* _touchListener;
public:
    // publicメンバとして virtual int getNowPos(void) const {return m_nowtPos;}
    CC_SYNTHESIZE_READONLY(int, m_nowPos, NowPos);
//    std::vector<Sprite*> panelBlocks;
    // create関数
    static PanelSprite* create();
    void addEvents();
    void removeEvents();
    void setPos(int position);
    // タップ開始時に処理されるメソッドを宣言
    virtual bool TouchBegan(Touch* pTouch, Event* pEvent);
    // タップ終了時に処理されるメソッドを宣言
    virtual void TouchEnded(Touch* pTouch, Event* pEvent);
    // ファイル名を取得
    std::string getImageFileName(int num);
};

#endif // __PANEL_SPRITE_H__

#include "PanelSprite.h"
#include "GameScene.h"

PanelSprite* PanelSprite::create() {
    PanelSprite* panelSprite = new PanelSprite();

    //　ファイル名を取得できたかの判定
    if (panelSprite && panelSprite->initWithFile(JPG_PANEL_BEFORE)) {
        // 取得できた場合
        // メモリの何か
        panelSprite->autorelease();
        // 初期タグをセット
        panelSprite->setTag(Config::kTagPanel);
        // タッチイベントを設定
        panelSprite->addEvents();
        return panelSprite;
    }
    CC_SAFE_DELETE(panelSprite);
    return NULL;
}

// タッチイベント追加
void PanelSprite::addEvents()
{
    auto listener = EventListenerTouchOneByOne::create();
    // 他のタッチイベントを受け付けない
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PanelSprite::TouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(PanelSprite::TouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

// タッチイベント削除
void PanelSprite::removeEvents()
{
    // イベント削除
    this->getEventDispatcher()->removeEventListener(_touchListener);
    _touchListener = nullptr;
}

// ポジションをセット
void PanelSprite::setPos(int position)
{
    m_nowPos = position;
}


// タップ開始時に処理される
bool PanelSprite::TouchBegan(Touch* pTouch, Event* pEvent)
{
    log("touch start!");
    // タップされた座標を左下から取得
    Vec2 point = pTouch->getLocation();
    // spriteの領域を取得
    Rect rect = this->getBoundingBox();
    // 取得した領域にタップしたポイントが含まれているか falseの場合は中断
    return rect.containsPoint(point);
}

// タップ終了時に処理される
void PanelSprite::TouchEnded(Touch* pTouch, Event* pEvent)
{
    // タップポイント取得
    // 今のシーンを取得
    auto base = Director::getInstance()->getRunningScene();
    auto game = (Game*) base->getChildren().at(1);

    // ボムの位置
    if (this->getTag() == Config::kTagBomb) {
        // ゲームオーバ
        game->showGameOver(this);
        // イベント全て削除
        this->getEventDispatcher()->removeAllEventListeners();
        game->showResetButton();
    }
    else
    {
        // パネルオープン
        game->showOpenPanel(this,m_nowPos);
        // タグを開き済みに変更
        this->setTag(Config::kTagPanelAfter);
        // イベント削除
        this->removeEvents();
    }
    log("touch end!");
}


// 画像ファイル名を取得
std::string PanelSprite::getImageFileName(int num)
{
    if (num <= 0) {
        return "panel_after.jpg";
    }
    else if (num > 0 && num < 9)
    {
        std::string name = StringUtils::format("%d.png", num);
        return name;
    }
    CCAssert(false, "non file");
    return "";
}

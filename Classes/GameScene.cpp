#include "GameScene.h"
//#include "BlockSprite.h"

USING_NS_CC;

Scene* Game::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Game::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// 初期処理
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Game::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    // タップイベントを取得する
    touchEnabled(true);
    
    // 変数初期化
    initForVariables();
    
    // 背景設定
    showBackground();
    
    // パネル初期化
    initPanel();
    
    // コマ表示 10 × 16のフィールド
    showPanel();
    
    return true;
}

// タップイベントを取得する 引数は有効性
void Game::touchEnabled(bool flg)
{
    // setTouchEnabled(true); 非推奨
    // setTouchMode(kCCTouchesOneByOne); 非推奨
    // _touchEnabledはcclayerで用意されてる変数 bool
    if (_touchEnabled != flg) {
        // 有効性をセット
        _touchEnabled = flg;
        if (flg) {
            // すでにイベントが設定されていた場合はreturn
            // nullptrはポインタとして扱ってくれる
            if (_touchListener != nullptr) {
                return;
            }
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = CC_CALLBACK_2(Game::TouchBegan, this);
            listener->onTouchEnded = CC_CALLBACK_2(Game::TouchEnded, this);
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
            // cocos2dxのEventListener
            _touchListener = listener;
        }
        else
        {
            // イベント削除
            this->getEventDispatcher()->removeEventListener(_touchListener);
            _touchListener = nullptr;
        }
    }
    
}


// 変数初期化
void Game::initForVariables()
{
    
    // 乱数初期化
    srand((unsigned)time(NULL));
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _winSize = Director::getInstance()->getWinSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    _audio = SimpleAudioEngine::getInstance();
    
    // パネルの1辺の長さを取得
    Sprite* panel = Sprite::create(JPG_PANEL_BEFORE);
    m_panelSize = panel->getContentSize().height;
}

// 背景を設置
void Game::showBackground()
{
    // 背景
    m_background = Sprite::create(PNG_BACKGROUND);
    m_background->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    m_background->setPosition(_visibleSize.width/2 + _origin.x, _origin.y);
    this->addChild(m_background, Config::kTagBackground, Config::kZOrderBackground);
}

// パネル初期化
void Game::initPanel()
{
    log("START drawSprite --------");
    double tStart = getSec();
    // 爆弾の数
    int max_bomb_num = MAX_BOMB_NUM;
    // 画像をキャッシュする
    // Director::getInstance()->getTextureCache()->addImage(JPG_PANEL_BEFORE);
    // パネルを作成 10×16
    for (int x = 0; x < MAX_BLOCK_X; x++) {
        for (int y = 0; y < MAX_BLOCK_Y; y++) {
            // 初期パネル画像
            Sprite* panel = Sprite::create(JPG_PANEL_BEFORE);
            // パネルの位置を設定
            panel->setPosition(getPosition(x, y));
            // 爆弾をセット (爆弾が残っている場合 && 2分の1の確率)
            if (max_bomb_num > 0 && (rand()%2) > 0) {
                // 爆弾のタグをパネルにセット
                panel->setTag(Config::kTagBomb);
                // 爆弾の残り数をデクリメント
                --max_bomb_num;
            }
            else
            {
                // パネルのタグをパネルにセット
                panel->setTag(Config::kTagPanel);
            }
            // パネルをvectorに追加
            panelBlocks.push_back(panel);
        }
    }

    double tEnd = getSec();
    log(" END  drawSprite -------- %f", tEnd - tStart);

    // 爆弾が全てセットしきれなかった場合
    if (max_bomb_num > 0) {
        // 爆弾をセットする．
        while (max_bomb_num < 1) {
            // ランダムでパネルを取得
            Sprite* panel = panelBlocks[rand() % panelBlocks.size()];
            // 爆弾がセットされていなければtrue
            if (panel->getTag() != Config::kTagBomb) {
                // 爆弾のタグをパネルにセット
                panel->setTag(Config::kTagBomb);
                // 爆弾の残り数をデクリメント
                --max_bomb_num;
            }
        }
    }
}

// パネルの位置を取得する
Point Game::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = m_background->getContentSize().width * 0.18;
    float offsetY = m_background->getContentSize().height * 0.05;
    return Point((posIndexX + 0.5) * m_panelSize + offsetX, (posIndexY + 0.5) * m_panelSize + offsetY);
}


// コマを表示させる
void Game::showPanel()
{
    // コマを表示 vectorのfor文
    for (auto sprite_panel : panelBlocks)
    {
        // パネルを背景のスプライトに組み込み
        m_background->addChild(sprite_panel);
    }
}

// メニュー終了
void Game::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


// タップ開始時に処理される
bool Game::TouchBegan(Touch* pTouch, Event* pEvent)
{
    log("touch start!");
    return true;
}

// タップ終了時に処理される
void Game::TouchEnded(Touch* pTouch, Event* pEvent)
{
    // タップポイント取得
    Point touchPoint = m_background->convertTouchToNodeSpace(pTouch);
    // タップした位置のパネルを取得
    getTouchPanelTag(touchPoint);
    
    
    log("touch end!");
}


// タップされたコマのタグを取得
void Game::getTouchPanelTag(Point touchPoint)
{
    // コマを表示 vectorのfor文
    for (auto sprite_panel : panelBlocks)
    {
        auto rect = sprite_panel->getBoundingBox();
        if (rect.containsPoint(touchPoint)) {
            // ボムの位置
            if (sprite_panel->getTag() == Config::kTagBomb) {
                log("Game Over");
                showGameOver(sprite_panel);
            }
            else
            {
                log("Open Panel");
                showOpenPanel(sprite_panel);
            }
            break;
        }
    }
}

// コマのタグを取得
int Game::getTag(int posIndexX, int posIndexY)
{
    return Config::kTagBasePanel + posIndexX * 100 + posIndexY;
}

// ゲームオーバ
void Game::showGameOver(Sprite* sprite_panel)
{
    // スプライトのアクションを停止
    sprite_panel->stopAllActions();
    // 画像を置き換える
    replaceImage(sprite_panel, PNG_BOMB);
    // タップイベント無効化
    touchEnabled(false);
}

void Game::showOpenPanel(Sprite* sprite_panel)
{
    // 周囲のボムを検索
    searchBomb(sprite_panel);
    
    
    
    // スプライトのアクションを停止
    sprite_panel->stopAllActions();
    // 画像を置き換える
    replaceImage(sprite_panel, JPG_PANEL_AFTER);
}

// スプライトの画像を置き換える
void Game::replaceImage(Sprite* sprite_panel, string file_name)
{
    Sprite* frame = Sprite::create(file_name);
    sprite_panel->setTexture(frame->getTexture());
}

//=======================================================
/// 秒数取得
//=======================================================
double Game::getSec(){
    timeval val;
    gettimeofday(&val, nullptr);
    return (val.tv_sec) + (val.tv_usec) * 1e-6;
}

// 周りのボムを検索
void Game::searchBomb(Sprite* sprite_panel)
{
    
}
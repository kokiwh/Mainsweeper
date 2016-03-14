#include "GameScene.h"
#include "PanelSprite.h"

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
    // touchEnabled(true);
    
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
    PanelSprite* panel = PanelSprite::create();
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
    //    log("START drawSprite --------");
    //    double tStart = getSec();
    // 画像をキャッシュする
    // Director::getInstance()->getTextureCache()->addImage(JPG_PANEL_BEFORE);
    // パネルを作成 10×16
    for (int x = 0; x < MAX_BLOCK_X; x++) {
        for (int y = 0; y < MAX_BLOCK_Y; y++) {
            // 初期パネル画像
            PanelSprite* panel = PanelSprite::create();
            // パネルのポジションをセット(10000式)
            panel->setPos(getTag(x, y));
            // パネルの位置を設定(座標POINT)
            panel->setPosition(getPosition(x, y));
            //            // 爆弾をセット (爆弾が残っている場合 && 2分の1の確率)
            //            if (max_bomb_num > 0 && (rand()%2) > 0) {
            //                // 爆弾のタグをパネルにセット
            //                panel->setTag(Config::kTagBomb);
            //                // 爆弾の残り数をデクリメント
            //                --max_bomb_num;
            //            }
            //            else
            //            {
            //                // パネルのタグをパネルにセット
            //                panel->setTag(Config::kTagPanel);
            //            }
            // パネルをvectorに追加
//            panel->addVector();
            panelBlocks.push_back(panel);
        }
    }
    
    //    double tEnd = getSec();
    //    log(" END  drawSprite -------- %f", tEnd - tStart);
    
    // 爆弾の数
    int max_bomb_num = MAX_BOMB_NUM;
    // 爆弾をセットする．
    while (max_bomb_num > 0) {
        // ランダムでインデックスを取得
        int r_num = rand() % panelBlocks.size();
        // 爆弾がセットされていなければtrue
        if (panelBlocks[r_num]->getTag() != Config::kTagBomb) {
            // 爆弾のタグをパネルにセット
            panelBlocks[r_num]->setTag(Config::kTagBomb);
            // 爆弾の残り数をデクリメント
            --max_bomb_num;
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
    
    
    log("touch end!");
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
    // 画像を全て置き換える
    for (auto panel : panelBlocks)
    {
        switch (panel->getTag()) {
            case Config::kTagPanel:
                replaceImage(panel, panel->getImageFileName(0));
                break;
            case Config::kTagBomb:
                replaceImage(panel, PNG_BOMB);
                break;
            default:
                break;
        }
    }
    // タップイベント無効化
    // touchEnabled(false);
}

void Game::showOpenPanel(PanelSprite* sprite_panel, int position)
{

    // 周囲のボムを検索
//    searchBomb(position);
    int base_pos_x = (position - Config::kTagBasePanel) / 100;
    int base_pos_y = (position - Config::kTagBasePanel) % 100;
    int num = 0;
    
    for (PanelSprite* panel : panelBlocks)
    {
        if (panel->getTag() == Config::kTagBomb) {
            int p_position = panel->getNowPos();
            int p_pos_x = (p_position - Config::kTagBasePanel) / 100;
            int p_pos_y = (p_position - Config::kTagBasePanel) % 100;
            if ((abs(base_pos_x - p_pos_x) == 1 && abs(base_pos_y - p_pos_y) < 2) || (abs(base_pos_y - p_pos_y) == 1 && abs(base_pos_x - p_pos_x) < 2) ) {
                ++num;
            }
            
        }
    }
    
    // スプライトのアクションを停止
    sprite_panel->stopAllActions();
    
    
    // 画像を置き換える
    replaceImage(sprite_panel, sprite_panel->getImageFileName(num));
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

// リセットボタンの処理
void Game::menuResetCallback(Ref *pSender)
{
    // シーンを置き換え
    Director::getInstance()->replaceScene(Game::createScene());
}

// リセットボタンの作成
void Game::showResetButton()
{
    Size bgSize = m_background->getContentSize();
    
    // リセットボタン作成
    MenuItemImage* resetButton = MenuItemImage::create(PNG_RESET, PNG_RESET, CC_CALLBACK_1(Game::menuResetCallback, this));
    resetButton->setPosition(Point(bgSize.width * 0.78, bgSize.height * 0.06));
    
    // メニュー作成
    Menu* menu = Menu::create(resetButton, NULL);
    menu->setPosition(Vec2::ZERO);
    m_background->addChild(menu);
}

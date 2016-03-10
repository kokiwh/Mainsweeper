#include "GameScene.h"
#include "BlockSprite.h"

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
    if ( !Layer::init() )
    {
        return false;
    }

    // 変数初期化
    initForVariables();
    
    // 背景設定
    showBackground();
    
    // コマ表示 10 × 16のフィールド
    showBlock();


    return true;
}

// 変数初期化
void Game::initForVariables()
{

    _visibleSize = Director::getInstance()->getVisibleSize();
    _winSize = Director::getInstance()->getWinSize();
    _origin = Director::getInstance()->getVisibleOrigin();

    _audio = SimpleAudioEngine::getInstance();

    // コマの一辺の長さを取得
    BlockSprite* pBlock = BlockSprite::createWithBlockType(Config::kBlockBomb);
    m_blockSize = pBlock->getContentSize().height;
    
    // コマ種類の配列生成
    blockTypes.push_back(Config::kBlockBomb);
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


// コマのタグを取得
int Game::getTag(int posIndexX, int posIndexY)
{
    return Config::kTagBaseBlock + posIndexX * 100 + posIndexY;
}


// コマを表示させる
void Game::showBlock()
{
    // 10 × 16 のコマを作成する
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            // ランダムでコマを作成
            Config::kBlock blockType = Config::kBlockBomb;
            // 対応するタグを取得
            int tag = getTag(x, y);
            // コマ配列にタグを追加
            m_blockTags[blockType].push_back(tag);
            // コマを作成
            BlockSprite* pBlock = BlockSprite::createWithBlockType(blockType);
            // コマの位置を設定
            pBlock->setPosition(getPosition(x, y));
            // コマを背景のスプライトに組み込み
            m_background->addChild(pBlock,Config::kZOrderBlock, tag);
        }
    }
}

// コマの位置を取得する
Point Game::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = m_background->getContentSize().width * 0.168;
    float offsetY = m_background->getContentSize().height * 0.029;
    return Point((posIndexX + 0.5) * m_blockSize + offsetX, (posIndexY + 0.5) * m_blockSize + offsetY);
}

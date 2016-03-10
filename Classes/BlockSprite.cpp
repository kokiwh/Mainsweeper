#include "BlockSprite.h"

BlockSprite::BlockSprite()
{
}

BlockSprite::~BlockSprite()
{
}


BlockSprite* BlockSprite::createWithBlockType(Config::kBlock blockType)
{
    BlockSprite *pRet = new BlockSprite();
    if (pRet && pRet->initWithBlockType(blockType))
    {
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
}

bool BlockSprite::initWithBlockType(Config::kBlock blockType)
{
    //　ファイル名を取得できたかの判定 取得できた場合はスルー
    // Sprite::initWithFile => ファイル名を連想配列にある場合は何もしなく、ない場合はロードする
    // getBlockImageFileName => ファイル名を取得する(下にあるメソッド)
    if (!Sprite::initWithFile(getBlockImageFileName(blockType))) {
        return false;
    }
    
    m_blockType = blockType;
    
    return true;
}


// 引数に応じてファイル名を返す, 引数に合うものがない場合はメッセージを表示し空文字を返す
const char* BlockSprite::getBlockImageFileName(Config::kBlock blockType)
{
    switch (blockType) {
        case Config::kBlockBomb:
            return "yellow.png";
        default:
            CCAssert(false, "invalid blockType");
            return "";
    }
}

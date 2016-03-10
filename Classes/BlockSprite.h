#ifndef __BLOCKSPRITE_H__
#define __BLOCKSPRITE_H__

#include "cocos2d.h"
#include "Config.h"

class BlockSprite : public cocos2d::Sprite
{
protected:
    const char* getBlockImageFileName(Config::kBlock blockType);

public:

    // マクロ定義 アクセッサ
    // protectedメンバとして kBlock m_blockType;
    // publicメンバとして virtual kBlock getBlockType(void) const {return m_blockType;}
    CC_SYNTHESIZE_READONLY(Config::kBlock, m_blockType, BlockType);

    BlockSprite();
    virtual ~BlockSprite();
    // create関数
    static BlockSprite* createWithBlockType(Config::kBlock blockType);
    bool initWithBlockType(Config::kBlock blockType);
    
    // implement the "static create()" method manually
    CREATE_FUNC(BlockSprite);
};

#endif // __BLOCKSPRITE_H__

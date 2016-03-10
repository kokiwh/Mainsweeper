#ifndef Config_h
#define Config_h

// 背景画像のファイル名
#define PNG_BACKGROUND "background.png"
// 縦のコマ数
#define MAX_BLOCK_Y 8
// 横のコマ数
#define MAX_BLOCK_X 8

class Config
{
public:
    enum kTag
    {
        // 画像のタグ
        kTagBackground = 1,
        // コマの基準タグ
        kTagBaseBlock = 10000,
    };
    
    enum kZOrder
    {
        // zオーダー 表示順
        kZOrderBackground,
        kZOrderBlock,
    };
    
    enum kBlock
    {
        kBlockBomb,
    };

    struct PositionIndex
    {
        PositionIndex(int x1, int y1)
        {
            x = x1;
            y = y1;
        }
        int x;
        int y;
    };

};

#endif /* Config_h */

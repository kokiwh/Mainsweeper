#ifndef Config_h
#define Config_h

// 背景画像のファイル名
#define PNG_BACKGROUND "background.png"
// パネルの初期画像
#define JPG_PANEL_BEFORE "panel_before.jpg"
// ボム画像
#define PNG_BOMB "bomb.png"
// リセットボタンファイル名
#define PNG_RESET "reset.png"
// 縦のコマ数
#define MAX_BLOCK_Y 16
// 横のコマ数
#define MAX_BLOCK_X 16
// 爆弾の数
#define MAX_BOMB_NUM 64

class Config
{
public:

    enum kTag
    {
        // 画像のタグ
        kTagBackground = 1, // 背景のタグ
        kTagPanel, // パネルのタグ
        kTagBomb, // 爆弾のタグ
        kTagPanelAfter, // 開き済みパネルのタグ
        // パネルの基準タグ
        kTagBasePanel = 10000,

    };
    
    enum kZOrder
    {
        // zオーダー 表示順
        kZOrderBackground,
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

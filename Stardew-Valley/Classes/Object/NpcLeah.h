// NpcLeah.h
#ifndef __NPC_LEAH_H__
#define __NPC_LEAH_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class NpcLeah : public cocos2d::Sprite
{
public:
    // 创建函数
    static NpcLeah* createWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile = "Leah.png");

    // 初始化函数
    bool initWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile);

private:
    // 触摸事件处理
    void setupTouchListener();

    // 显示对话
    void showDialogue();

    // 显示选项
    void showOptions();

    // 关闭对话
    void closeDialogue();

    // 对话相关变量
    cocos2d::LayerColor* _dialogueLayer;
    cocos2d::Sprite* _npcFace;
    cocos2d::ui::Text* _dialogueLabel;
    int _dialogueIndex;

    // 对话内容结构体
    struct DialogueLine {
        std::string text;
        std::string faceImage;
    };

    std::vector<DialogueLine> _npcDialogues;
};

#endif // __NPC_LEAH_H__

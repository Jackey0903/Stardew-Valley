// Npc.h
#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
struct DialogueLine {
    std::string text;
    std::string faceImage;
};
class Npc : public cocos2d::Sprite
{
private:
    // 对话相关变量
    cocos2d::LayerColor* _dialogueLayer;
    cocos2d::Sprite* _npcFace;
    cocos2d::ui::Text* _dialogueLabel;
    std::vector<std::string> options;;
    int _dialogueIndex;


    std::vector<DialogueLine> _npcDialogues;
    // 触摸事件处理
    void setupTouchListener();

    // 显示对话
    void showDialogue();

    // 显示选项
    void showOptions();

    // 关闭对话
    void closeDialogue();

public:
    std::string npcName;
    // 创建函数
    static Npc* createWithPosition(const cocos2d::Vec2& position, const std::string& npcName
                                  , std::vector<DialogueLine>& npcDialogues
                                  , std::vector<std::string>& option);

    // 初始化函数
    bool initWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile);

};

#endif // __NPC_H__

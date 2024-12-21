/****************************************************************
  * 项目名        : Stardew-Valley
  * 文件名        : Npc.h
  * 文件功能      : Npc类的实现
  * 作者          : 曹津硕、胡浩杰
  * 更新日期      : 2024/12/19
  * 许可证        : MIT License
****************************************************************/

#ifndef __NPC_H__
#define __NPC_H__

#include "cocos2d.h"
// 引入 UI 组件头文件
#include "ui/CocosGUI.h"

// 定义对话行结构体，包含文本和头像图像
struct DialogueLine {
    std::string text;       // 对话文本
    std::string faceImage;  // 头像图像文件路径
};

// 定义 Npc 类，继承自 cocos2d::Sprite
class Npc : public cocos2d::Sprite
{
private:
    // 对话相关变量
    cocos2d::LayerColor* _dialogueLayer;        // 对话层
    cocos2d::Sprite* _npcFace;                  // NPC 头像
    cocos2d::ui::Text* _dialogueLabel;          // 对话文本标签
    std::vector<std::string> options;           // 选项列表
    int _dialogueIndex;                         // 当前对话索引

    std::vector<DialogueLine> _npcDialogues;    // NPC 对话内容

    // 触摸事件处理函数
    void setupTouchListener();

    // 显示对话
    void showDialogue();

    // 显示选项
    void showOptions();

    // 关闭对话
    void closeDialogue();

public:
    std::string npcName;  // NPC 名称

    // 创建函数，使用指定位置和参数创建 Npc 对象
    static Npc* createWithPosition(
        const cocos2d::Vec2& position,
        const std::string& npcName,
        std::vector<DialogueLine>& npcDialogues,
        std::vector<std::string>& option);

    // 初始化函数
    bool initWithPosition(const cocos2d::Vec2& position, const std::string& spriteFile);

};

#endif // __NPC_H__


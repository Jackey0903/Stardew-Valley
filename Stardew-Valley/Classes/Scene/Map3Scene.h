/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Map3Scene.h
 * 文件功能      : 场景3派生类的功能实现
 * 作者          : 胡浩杰、曹津硕、胡正华
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __MAP3_SCENE_H__
#define __MAP3_SCENE_H__

#include "BaseMapScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h" 
#include "Object/Npc.h"

 // 使用cocos2d命名空间
class Map3Scene : public BaseMapScene
{
public:
    // 创建场景的静态方法
    static cocos2d::Scene* createScene();

    // 初始化方法，重写基类的init
    virtual bool init() override;

    // 场景进入时调用的方法，重写基类的onEnter
    virtual void onEnter() override;

    // 使用Cocos2d的CREATE_FUNC宏创建Map3Scene实例
    CREATE_FUNC(Map3Scene);

private:
    cocos2d::Sprite* _npc; // NPC精灵

    // 用于对话框显示的成员变量
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // 对话框背景层（C++11特性：类内初始化）
    cocos2d::Sprite* _npcFace = nullptr;             // NPC头像精灵（C++11特性：类内初始化）
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // 显示对话文本的控件（C++11特性：类内初始化）
    int _dialogueIndex = 0;                          // 当前显示的对话语句索引（C++11特性：类内初始化）

    // 存储NPC Leah的对话内容
    std::vector<DialogueLine> _npcLeahDialogues;

    // 存储NPC Leah的选项内容
    std::vector<std::string> LeahOption;
};

#endif // __MAP3_SCENE_H__

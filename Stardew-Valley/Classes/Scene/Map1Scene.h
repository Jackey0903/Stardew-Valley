/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Map1Scene.h
 * 文件功能      : 场景1派生类的功能实现
 * 作者          : 胡浩杰、曹津硕、胡正华
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __MAP1_SCENE_H__
#define __MAP1_SCENE_H__

#include "BaseMapScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"          // 引入Cocos2d UI模块
#include "Object/Npc.h"           // 引入Npc类

 // Map1Scene 类继承自 BaseMapScene，实现了场景1的具体功能
class Map1Scene : public BaseMapScene
{
public:
    // 创建并返回一个 Map1Scene 实例
    static cocos2d::Scene* createScene();

    // 初始化函数，重载自基类
    virtual bool init() override;

    // 进入场景时调用，重载自基类
    virtual void onEnter() override;

    // 使用宏创建 Map1Scene 对象
    CREATE_FUNC(Map1Scene);

private:
    bool _isCollectingStone;  // 是否正在收集矿石的标志
    cocos2d::Sprite* _npc;    // NPC精灵对象

    // 显示对话选项的函数
    void showOptions();

    // 关闭对话框的函数
    void closeDialogue();

    // 用于对话框显示的成员变量
    cocos2d::LayerColor* _dialogueLayer = nullptr;  // 对话框背景层
    cocos2d::Sprite* _npcFace = nullptr;            // NPC头像精灵
    cocos2d::ui::Text* _dialogueLabel = nullptr;    // 显示对话文本的控件
    int _dialogueIndex = 0;                         // 当前显示的对话语句索引

    // 存储矿石的容器
    std::vector<cocos2d::Sprite*> _mines;

    // 初始化事件监听器
    void initEventListeners();

    // 键盘按下事件回调
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 键盘释放事件回调
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 鼠标按下事件回调
    void onMouseDown(cocos2d::Event* event);

    // 收集矿石的函数
    void collectStoneAt(const cocos2d::Vec2& locationInMap);

    // 添加手动放置的矿石
    void addManualMines();

    // 存储NPC对话内容的容器
    std::vector<DialogueLine> _npcDemiDialogues;

    // NPC对话选项
    std::vector<std::string> DemiOption;
};

#endif  // __MAP1_SCENE_H__

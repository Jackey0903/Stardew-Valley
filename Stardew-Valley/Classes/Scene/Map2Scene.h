/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Map2Scene.h
 * 文件功能      : 场景2派生类的功能实现
 * 作者          : 胡浩杰、曹津硕、胡正华
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __MAP2_SCENE_H__    // 防止重复包含头文件
#define __MAP2_SCENE_H__

#include "BaseMapScene.h"    // 引入基类场景
#include "Object/Npc.h"      // 引入NPC类

 // Map2Scene 类继承自 BaseMapScene，实现了场景2的具体功能
class Map2Scene : public BaseMapScene
{
private:
    //按钮状态枚举 
    enum class State { OFF = 0, PLANT_TREE, CHOP_TREE, PLANT_CROP, HARVEST_CROP, TOTAL_STATES };
    // 当前状态
    State _currentState = State::OFF;
    // 状态按钮
    cocos2d::ui::Button* _btnState;
    // 按钮图片路径数组
    std::vector<std::string> _stateButtonImages = {
        "../Resources/KEYS/T_1.png",       // OFF
        "../Resources/KEYS/T_2.png",     // PLANT_TREE
        "../Resources/KEYS/T_3.png",     // CHOP_TREE
        "../Resources/KEYS/P_2.png",     // PLANT_CROP
        "../Resources/KEYS/P_3.png"      // HARVEST_CROP
    };
    // 状态按钮触摸事件处理
    void onStateButtonTouched(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    // 切换状态函数
    void switchState();

    // 更新状态标志
    void updateStateFlags();
public:
    // 创建并返回一个 Map2Scene 实例
    static cocos2d::Scene* createScene();

    // 初始化函数，重载自基类
    virtual bool init() override;

    // 进入场景时调用，重载自基类
    virtual void onEnter() override;

    // 使用宏创建 Map2Scene 对象
    CREATE_FUNC(Map2Scene);

private:
    bool _isPlanting;                             // 是否正在种植作物
    bool _isPlantingTree;                         // 是否正在种植树木
    std::vector<cocos2d::Sprite*> _crops;         // 存储作物的容器
    std::vector<cocos2d::Sprite*> _trees;         // 存储树木的容器
    cocos2d::Sprite* _npc;                        // NPC精灵对象

    // 用于对话框显示的成员变量
    cocos2d::LayerColor* _dialogueLayer = nullptr;   // 对话框背景层
    cocos2d::Sprite* _npcFace = nullptr;             // NPC头像精灵
    cocos2d::ui::Text* _dialogueLabel = nullptr;     // 显示对话文本的控件
    int _dialogueIndex = 0;                          // 当前显示的对话语句索引

    std::vector<DialogueLine> _npcLewisDialogues;    // NPC Lewis的对话内容
    std::vector<std::string> LewisOption;            // NPC Lewis的对话选项

    // 初始化事件监听器（键盘、鼠标）
    void initEventListeners();

    // 键盘按下事件回调
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 键盘释放事件回调
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 鼠标按下事件回调
    void onMouseDown(cocos2d::Event* event);

    // 种植作物的函数
    void plantCropAt(const cocos2d::Vec2& locationInMap);

    // 收割作物的函数
    void harvestCropAt(const cocos2d::Vec2& locationInMap);

    // 种植树木的函数
    void plantTreeAt(const cocos2d::Vec2& locationInMap);

    // 砍伐树木的函数
    void fellTreeAt(const cocos2d::Vec2& locationInMap);
};

#endif  // __MAP2_SCENE_H__

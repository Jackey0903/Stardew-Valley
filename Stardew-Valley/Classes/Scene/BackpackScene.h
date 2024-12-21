/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : BackpackScene.h
 * 文件功能      : 背包场景的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#ifndef BACKPACKSCENE_H
#define BACKPACKSCENE_H

#include "cocos2d.h"  // 引入Cocos2d框架
#include "../Player/Backpack.h"  // 引入背包类
#include "ui/CocosGUI.h"  // 引入UI模块，处理UI相关的元素

 // 背包场景类，继承自cocos2d::Scene
class BackpackScene : public cocos2d::Scene {
public:
    // 创建并返回背包场景
    static cocos2d::Scene* createScene();

    // 初始化背包场景
    virtual bool init();

    // 创建菜单，显示物品
    void createMenu();

    // 键盘按下事件的回调函数
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // Cocos2d的宏，用于实现创建背包场景实例
    CREATE_FUNC(BackpackScene);

    // 获取背包对象
    Backpack* getBackpack();

    // 更新玩家手持物品图标
    void updatePlayerWithItem();

    // 创建技能树UI
    void createSkillTree();

    // 移动速度滑动条变化时的回调函数
    void onSpeedSliderChanged(Ref* sender, cocos2d::ui::Slider::EventType type);

    // 人物大小滑动条变化时的回调函数
    void onSizeSliderChanged(Ref* sender, cocos2d::ui::Slider::EventType type);

private:
    // 私有成员变量，用于存储背包实例
    Backpack* _backpack;

    // 角色精灵
    cocos2d::Sprite* _playerSprite;

    // 当前工具精灵
    cocos2d::Sprite* _currentToolSprite;

    // 当前选中的物品
    Item* _selectedItem = nullptr;

    // 移动速度滑动条
    cocos2d::ui::Slider* _speedSlider;

    // 人物大小滑动条
    cocos2d::ui::Slider* _sizeSlider;
};

#endif // BACKPACKSCENE_H

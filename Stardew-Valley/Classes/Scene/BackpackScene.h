/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : BackpackScene.h
 * 文件功能      : 背包场景的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/
#ifndef BACKPACKSCENE_H
#define BACKPACKSCENE_H

#include "cocos2d.h"
#include "../Player/Backpack.h" // 包含背包类

class BackpackScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void createMenu(); // 创建菜单用于显示物品
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // 修复命名空间
    CREATE_FUNC(BackpackScene);
    Backpack* getBackpack(); // 声明获取背包的方法
    cocos2d::Sprite* _playerSprite;           // 角色精灵
	cocos2d::Sprite* _currentToolSprite;	  // 当前工具
    Item* _selectedItem = nullptr;            // 当前选中的物品
    void BackpackScene::updatePlayerWithItem();

private:
    Backpack* _backpack; // 私有成员变量，用于存储背包实例
};


#endif // BACKPACKSCENE_H

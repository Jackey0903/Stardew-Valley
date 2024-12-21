/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : StartMenu.h
 * 文件功能      : StartMenuScene的功能实现
 * 作者          : 曹津硕
 * 更新日期      : 2024/12/04
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __START_MENU_H__
#define __START_MENU_H__

#include "cocos2d.h"

USING_NS_CC;

class StartMenu : public Scene
{
public:

    static Scene* createScene();

    void exitGame(Ref* pSender);

    void startGame(Ref* pSender);

    void settings(Ref* pSender);

    virtual bool init() override; // C++11特性：override关键字

    // 创建 StartMenu 类的实例
    CREATE_FUNC(StartMenu); // C++11特性：宏定义简化创建过程

private:
    Sprite* _backgroundSprite; // 背景精灵
    Sprite* _title;            // 标题精灵
    MenuItemImage* startButton; // 开始按钮
    MenuItemImage* exitButton;  // 退出按钮
    Menu* menu;                 // 菜单
    Vec2 menuPosition;          // 菜单位置
    EventListenerMouse* listener; // 鼠标事件监听器

    void menuCloseCallback(Ref* pSender);

    void cleanMouse();

    void onMouseMove(EventMouse* event);
};

#endif // __START_MENU_H__

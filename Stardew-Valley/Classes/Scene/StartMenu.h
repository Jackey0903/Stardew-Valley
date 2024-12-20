

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
    virtual bool init();
    Sprite* _backgroundSprite;
    Sprite* _title;
    MenuItemImage* startButton;
    MenuItemImage* exitButton;
    Menu* menu;
    Vec2 menuPosition;
    EventListenerMouse* listener;


    // a selector callback
    void menuCloseCallback(Ref* pSender);
    void cleanMouse();
    void onMouseMove(EventMouse* event);
    // implement the "static create()" method manually
    CREATE_FUNC(StartMenu);
};

#endif // __START_MENU_H__

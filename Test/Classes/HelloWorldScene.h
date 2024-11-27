/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 **************************************************************** ************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // 实现键盘事件处理函数
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void HelloWorld::update(float delta);
    // 动画
    void HelloWorld::startWalkingAnimation(const std::string& direction);
    void HelloWorld::stopWalkingAnimation();
    void startGame(Ref* pSender);
    void exitGame(Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    void onMouseMove(cocos2d::EventMouse* event);
private:
    // 定义精灵的成员变量
    cocos2d::Sprite* _playerSprite;
    // 动画
    cocos2d::Animation* _walkAnimation;
    std::string _currentDirection;
    // 添加背景精灵成员变量
    cocos2d::Sprite* _backgroundSprite;
    cocos2d::Sprite* _title;
    cocos2d::Menu* menu;
    cocos2d::MenuItemImage* startButton;
    cocos2d::MenuItemImage* exitButton;
    // 当前精灵方向的纹理名称
    std::string _currentTexture;
    // 添加成员变量以跟踪移动状态
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
    bool _isMovingUp = false;
    bool _isMovingDown = false;
    // 添加成员变量来跟踪长按状态
    bool _isMoving = false;
};

#endif // __HELLOWORLD_SCENE_H__

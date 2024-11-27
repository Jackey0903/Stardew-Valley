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
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "AppDelegate.h" // 用于退出程序

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
void HelloWorld::exitGame(Ref* pSender)
{
    // 退出程序
    Director::getInstance()->end();
}
void HelloWorld::startGame(Ref* pSender)
{
    // 预加载纹理图集文件
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");
    this->removeChild(_backgroundSprite);
    this->removeChild(menu);
    this->removeChild(_title);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建地图背景
    _backgroundSprite = Sprite::create("Pierres General Store.png");
    if (_backgroundSprite == nullptr)
    {
        problemLoading("Pierres General Store.png");
    }
    else
    {
        // 获取背景原始尺寸
        Size bgSize = _backgroundSprite->getContentSize();

        // 获取可见尺寸
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // 计算缩放比例
        float scaleX = visibleSize.width / bgSize.width;
        float scaleY = visibleSize.height / bgSize.height;
        float scale = std::max(scaleX, scaleY); // 保持宽高比，可能会裁剪

        // 应用缩放比例
        _backgroundSprite->setScale(scale);

        // 设置背景图像的位置到屏幕中心
        _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
            visibleSize.height / 2));

        // 将背景添加到场景中
        this->addChild(_backgroundSprite, -1); // 添加在其他元素的下面
    }

    // 创建一个精灵
    _playerSprite = Sprite::create("Stand_Down.png"); // 将其赋值给成员变量
    if (_playerSprite == nullptr)
    {
        problemLoading("'Stand_Down.png'");
    }
    else
    {
        // 设置精灵的位置
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // 将精灵缩小一半
        _playerSprite->setScale(0.5f); // 缩小一半
        // 将精灵添加到场景中
        this->addChild(_playerSprite, 0);
    }

    // 创建键盘事件监听器
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 设置更新回调
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::update));

    // 初始化动画
    _walkAnimation = nullptr; // 初始化为 nullptr
}

void HelloWorld::startWalkingAnimation(const std::string& direction)
{
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 2; ++i) {
        std::string frameName = "Walk_" + direction + "_" + std::to_string(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        frames.pushBack(frame);
    }

    _walkAnimation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(_walkAnimation);
    _playerSprite->runAction(RepeatForever::create(animate));
}

void HelloWorld::stopWalkingAnimation()
{
    _playerSprite->stopAllActions(); // 停止所有动画
    _playerSprite->setTexture("Stand_" + _currentDirection + ".png"); // 设置待机状态
}

// 在 onKeyPressed 中设置 _isMoving 标志
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _isMoving = true;

    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_A:
            if (!_isMovingLeft) {
                _isMovingLeft = true;
                _currentTexture = "Stand_Left.png"; // 设置当前纹理
                _playerSprite->setTexture(_currentTexture); // 更新精灵纹理
                _currentDirection = "Left"; // 更新当前方向
                startWalkingAnimation(_currentDirection); // 开始动画
                _isMovingRight = false; // 禁用右
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if (!_isMovingRight) {
                _isMovingRight = true;
                _currentTexture = "Stand_Right.png"; // 设置当前纹理
                _playerSprite->setTexture(_currentTexture); // 更新精灵纹理
                _currentDirection = "Right"; // 更新当前方向
                startWalkingAnimation(_currentDirection); // 开始动画
                _isMovingLeft = false; // 禁用左
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if (!_isMovingUp) {
                _isMovingUp = true;
                _currentTexture = "Stand_Up.png"; // 设置当前纹理
                _playerSprite->setTexture(_currentTexture); // 更新精灵纹理
                _currentDirection = "Up"; // 更新当前方向
                startWalkingAnimation(_currentDirection); // 开始动画
                _isMovingDown = false; // 禁用下
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if (!_isMovingDown) {
                _isMovingDown = true;
                _currentTexture = "Stand_Down.png"; // 设置当前纹理
                _playerSprite->setTexture(_currentTexture); // 更新精灵纹理
                _currentDirection = "Down"; // 更新当前方向
                startWalkingAnimation(_currentDirection); // 开始动画
                _isMovingUp = false; // 禁用上
            }
            break;
        default:
            break;
    }
}

// 在 onKeyReleased 中重置 _isMoving 标志
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_A:
            _isMovingLeft = false;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            _isMovingRight = false;
            break;
        case EventKeyboard::KeyCode::KEY_W:
            _isMovingUp = false;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            _isMovingDown = false;
            break;
        default:
            break;
    }

    // 当所有方向键松开时，停止移动
    if (!_isMovingLeft && !_isMovingRight && !_isMovingUp && !_isMovingDown) {
        _isMoving = false;
        if (_currentDirection != "") {
            stopWalkingAnimation(); // 停止动画
            _currentDirection = ""; // 清空当前方向
        }
    }
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 创建背景图像
    _backgroundSprite = Sprite::create("BackGround.png");
    _title = Sprite::create("Title.png");
    if (_title == nullptr)
    {
        problemLoading("Title.png");
    }
    else
    {
        // 获取背景的原始尺寸
        Size titleSize = _title->getContentSize();

        // 计算缩放比例：确保背景图像与屏幕尺寸一致
        float scaleX = visibleSize.width / titleSize.width;
        float scaleY = visibleSize.height / titleSize.height;
        float scale = std::max(scaleX, scaleY); // 保持宽高比，可能会裁剪

        // 应用缩放比例
        _title->setScale(scale/5);

        // 设置背景图像的位置到屏幕中心
        _title->setPosition(Vec2(visibleSize.width / 2-titleSize.width/10,
            visibleSize.height / 2));

        // 将背景添加到场景中
        this->addChild(_title, 1); // 添加在其他元素的下面
    }
    if (_backgroundSprite == nullptr)
    {
        problemLoading("BackGround.png");
    }
    else
    {
        // 获取背景的原始尺寸
        Size bgSize = _backgroundSprite->getContentSize();

        // 计算缩放比例：确保背景图像与屏幕尺寸一致
        float scaleX = visibleSize.width / bgSize.width;
        float scaleY = visibleSize.height / bgSize.height;
        float scale = std::max(scaleX, scaleY); // 保持宽高比，可能会裁剪

        // 应用缩放比例
        _backgroundSprite->setScale(scale);

        // 设置背景图像的位置到屏幕中心
        _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
            visibleSize.height / 2));

        // 将背景添加到场景中
        this->addChild(_backgroundSprite, 0); // 添加在其他元素的下面
    }

    // 创建开始游戏按钮
    startButton = MenuItemImage::create("Start_D.png", "Start_L.png",
        CC_CALLBACK_1(HelloWorld::startGame, this));

    // 创建退出游戏按钮
    exitButton = MenuItemImage::create("Exit_D.png", "Exit_L.png",
        (CC_CALLBACK_1(HelloWorld::exitGame, this)));
    startButton->setScale(0.5f);
    exitButton->setScale(0.5f);
    // 将按钮设置为对称
    Size startSize = startButton->getContentSize();
    startButton->setPosition(Vec2(visibleSize.width / 2 - 500, visibleSize.height / 4));
    exitButton->setPosition(Vec2(visibleSize.width / 2 + 500, visibleSize.height / 4));
    // 创建一个菜单并将按钮添加到其中
    menu = Menu::create(startButton, exitButton, nullptr);
    //menu->alignItemsHorizontallyWithPadding(30);
    //menu->setPosition(Vec2::ZERO); // 将菜单放置在原点
    this->addChild(menu, 1);
    // 添加鼠标事件监听
    auto listener = EventListenerMouse::create();
    listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
// 鼠标移动事件处理
void HelloWorld::onMouseMove(cocos2d::EventMouse* event)
{
    auto mousePos = event->getLocation();

    // 检查鼠标是否在按钮上
    if (startButton->getBoundingBox().containsPoint(mousePos))
    {
        startButton->setNormalImage(Sprite::create("Start_L.png")); // 使用高亮图像
    }
    else
    {
        startButton->setNormalImage(Sprite::create("Start_D.png")); // 使用正常图像
    }

    // 检查鼠标是否在退出按钮上
    if (exitButton->getBoundingBox().containsPoint(mousePos))
    {
        exitButton->setNormalImage(Sprite::create("Exit_L.png")); // 使用高亮图像
    }
    else
    {
        exitButton->setNormalImage(Sprite::create("Exit_D.png")); // 使用正常图像
    }
}
// 添加 update 方法来处理连续移动
void HelloWorld::update(float delta)
{
    if (_isMoving) {
        Vec2 loc = _playerSprite->getPosition();

        if (_isMovingLeft) {
            loc.x -= 200 * delta;
            _backgroundSprite->setPositionX(_backgroundSprite->getPositionX() + 200 * delta); // 背景右移
        }
        if (_isMovingRight) {
            loc.x += 200 * delta;
            _backgroundSprite->setPositionX(_backgroundSprite->getPositionX() - 200 * delta); // 背景左移
        }
        if (_isMovingUp) {
            loc.y += 200 * delta;
            _backgroundSprite->setPositionY(_backgroundSprite->getPositionY() - 200 * delta); // 背景下移
        }
        if (_isMovingDown) {
            loc.y -= 200 * delta;
            _backgroundSprite->setPositionY(_backgroundSprite->getPositionY() + 200 * delta); // 背景上移
        }

        // 边界检测，限制背景和精灵在可见区域内
        auto visibleSize = Director::getInstance()->getVisibleSize();
        loc.x = MAX(0, MIN(loc.x, visibleSize.width));
        loc.y = MAX(0, MIN(loc.y, visibleSize.height));

        _playerSprite->setPosition(loc);
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

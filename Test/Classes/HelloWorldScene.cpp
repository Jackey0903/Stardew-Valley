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
#include "AppDelegate.h" // �����˳�����

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
    // �˳�����
    Director::getInstance()->end();
}
void HelloWorld::startGame(Ref* pSender)
{
    // Ԥ��������ͼ���ļ�
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");
    this->removeChild(_backgroundSprite);
    this->removeChild(menu);
    this->removeChild(_title);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // ������ͼ����
    _backgroundSprite = Sprite::create("Pierres General Store.png");
    if (_backgroundSprite == nullptr)
    {
        problemLoading("Pierres General Store.png");
    }
    else
    {
        // ��ȡ����ԭʼ�ߴ�
        Size bgSize = _backgroundSprite->getContentSize();

        // ��ȡ�ɼ��ߴ�
        auto visibleSize = Director::getInstance()->getVisibleSize();

        // �������ű���
        float scaleX = visibleSize.width / bgSize.width;
        float scaleY = visibleSize.height / bgSize.height;
        float scale = std::max(scaleX, scaleY); // ���ֿ�߱ȣ����ܻ�ü�

        // Ӧ�����ű���
        _backgroundSprite->setScale(scale);

        // ���ñ���ͼ���λ�õ���Ļ����
        _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
            visibleSize.height / 2));

        // ��������ӵ�������
        this->addChild(_backgroundSprite, -1); // ���������Ԫ�ص�����
    }

    // ����һ������
    _playerSprite = Sprite::create("Stand_Down.png"); // ���丳ֵ����Ա����
    if (_playerSprite == nullptr)
    {
        problemLoading("'Stand_Down.png'");
    }
    else
    {
        // ���þ����λ��
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // ��������Сһ��
        _playerSprite->setScale(0.5f); // ��Сһ��
        // ��������ӵ�������
        this->addChild(_playerSprite, 0);
    }

    // ���������¼�������
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // ���ø��»ص�
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::update));

    // ��ʼ������
    _walkAnimation = nullptr; // ��ʼ��Ϊ nullptr
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
    _playerSprite->stopAllActions(); // ֹͣ���ж���
    _playerSprite->setTexture("Stand_" + _currentDirection + ".png"); // ���ô���״̬
}

// �� onKeyPressed ������ _isMoving ��־
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _isMoving = true;

    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_A:
            if (!_isMovingLeft) {
                _isMovingLeft = true;
                _currentTexture = "Stand_Left.png"; // ���õ�ǰ����
                _playerSprite->setTexture(_currentTexture); // ���¾�������
                _currentDirection = "Left"; // ���µ�ǰ����
                startWalkingAnimation(_currentDirection); // ��ʼ����
                _isMovingRight = false; // ������
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if (!_isMovingRight) {
                _isMovingRight = true;
                _currentTexture = "Stand_Right.png"; // ���õ�ǰ����
                _playerSprite->setTexture(_currentTexture); // ���¾�������
                _currentDirection = "Right"; // ���µ�ǰ����
                startWalkingAnimation(_currentDirection); // ��ʼ����
                _isMovingLeft = false; // ������
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if (!_isMovingUp) {
                _isMovingUp = true;
                _currentTexture = "Stand_Up.png"; // ���õ�ǰ����
                _playerSprite->setTexture(_currentTexture); // ���¾�������
                _currentDirection = "Up"; // ���µ�ǰ����
                startWalkingAnimation(_currentDirection); // ��ʼ����
                _isMovingDown = false; // ������
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if (!_isMovingDown) {
                _isMovingDown = true;
                _currentTexture = "Stand_Down.png"; // ���õ�ǰ����
                _playerSprite->setTexture(_currentTexture); // ���¾�������
                _currentDirection = "Down"; // ���µ�ǰ����
                startWalkingAnimation(_currentDirection); // ��ʼ����
                _isMovingUp = false; // ������
            }
            break;
        default:
            break;
    }
}

// �� onKeyReleased ������ _isMoving ��־
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

    // �����з�����ɿ�ʱ��ֹͣ�ƶ�
    if (!_isMovingLeft && !_isMovingRight && !_isMovingUp && !_isMovingDown) {
        _isMoving = false;
        if (_currentDirection != "") {
            stopWalkingAnimation(); // ֹͣ����
            _currentDirection = ""; // ��յ�ǰ����
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
    // ��������ͼ��
    _backgroundSprite = Sprite::create("BackGround.png");
    _title = Sprite::create("Title.png");
    if (_title == nullptr)
    {
        problemLoading("Title.png");
    }
    else
    {
        // ��ȡ������ԭʼ�ߴ�
        Size titleSize = _title->getContentSize();

        // �������ű�����ȷ������ͼ������Ļ�ߴ�һ��
        float scaleX = visibleSize.width / titleSize.width;
        float scaleY = visibleSize.height / titleSize.height;
        float scale = std::max(scaleX, scaleY); // ���ֿ�߱ȣ����ܻ�ü�

        // Ӧ�����ű���
        _title->setScale(scale/5);

        // ���ñ���ͼ���λ�õ���Ļ����
        _title->setPosition(Vec2(visibleSize.width / 2-titleSize.width/10,
            visibleSize.height / 2));

        // ��������ӵ�������
        this->addChild(_title, 1); // ���������Ԫ�ص�����
    }
    if (_backgroundSprite == nullptr)
    {
        problemLoading("BackGround.png");
    }
    else
    {
        // ��ȡ������ԭʼ�ߴ�
        Size bgSize = _backgroundSprite->getContentSize();

        // �������ű�����ȷ������ͼ������Ļ�ߴ�һ��
        float scaleX = visibleSize.width / bgSize.width;
        float scaleY = visibleSize.height / bgSize.height;
        float scale = std::max(scaleX, scaleY); // ���ֿ�߱ȣ����ܻ�ü�

        // Ӧ�����ű���
        _backgroundSprite->setScale(scale);

        // ���ñ���ͼ���λ�õ���Ļ����
        _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
            visibleSize.height / 2));

        // ��������ӵ�������
        this->addChild(_backgroundSprite, 0); // ���������Ԫ�ص�����
    }

    // ������ʼ��Ϸ��ť
    startButton = MenuItemImage::create("Start_D.png", "Start_L.png",
        CC_CALLBACK_1(HelloWorld::startGame, this));

    // �����˳���Ϸ��ť
    exitButton = MenuItemImage::create("Exit_D.png", "Exit_L.png",
        (CC_CALLBACK_1(HelloWorld::exitGame, this)));
    startButton->setScale(0.5f);
    exitButton->setScale(0.5f);
    // ����ť����Ϊ�Գ�
    Size startSize = startButton->getContentSize();
    startButton->setPosition(Vec2(visibleSize.width / 2 - 500, visibleSize.height / 4));
    exitButton->setPosition(Vec2(visibleSize.width / 2 + 500, visibleSize.height / 4));
    // ����һ���˵�������ť��ӵ�����
    menu = Menu::create(startButton, exitButton, nullptr);
    //menu->alignItemsHorizontallyWithPadding(30);
    //menu->setPosition(Vec2::ZERO); // ���˵�������ԭ��
    this->addChild(menu, 1);
    // �������¼�����
    auto listener = EventListenerMouse::create();
    listener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
// ����ƶ��¼�����
void HelloWorld::onMouseMove(cocos2d::EventMouse* event)
{
    auto mousePos = event->getLocation();

    // �������Ƿ��ڰ�ť��
    if (startButton->getBoundingBox().containsPoint(mousePos))
    {
        startButton->setNormalImage(Sprite::create("Start_L.png")); // ʹ�ø���ͼ��
    }
    else
    {
        startButton->setNormalImage(Sprite::create("Start_D.png")); // ʹ������ͼ��
    }

    // �������Ƿ����˳���ť��
    if (exitButton->getBoundingBox().containsPoint(mousePos))
    {
        exitButton->setNormalImage(Sprite::create("Exit_L.png")); // ʹ�ø���ͼ��
    }
    else
    {
        exitButton->setNormalImage(Sprite::create("Exit_D.png")); // ʹ������ͼ��
    }
}
// ��� update ���������������ƶ�
void HelloWorld::update(float delta)
{
    if (_isMoving) {
        Vec2 loc = _playerSprite->getPosition();

        if (_isMovingLeft) {
            loc.x -= 200 * delta;
            _backgroundSprite->setPositionX(_backgroundSprite->getPositionX() + 200 * delta); // ��������
        }
        if (_isMovingRight) {
            loc.x += 200 * delta;
            _backgroundSprite->setPositionX(_backgroundSprite->getPositionX() - 200 * delta); // ��������
        }
        if (_isMovingUp) {
            loc.y += 200 * delta;
            _backgroundSprite->setPositionY(_backgroundSprite->getPositionY() - 200 * delta); // ��������
        }
        if (_isMovingDown) {
            loc.y -= 200 * delta;
            _backgroundSprite->setPositionY(_backgroundSprite->getPositionY() + 200 * delta); // ��������
        }

        // �߽��⣬���Ʊ����;����ڿɼ�������
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

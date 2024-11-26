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


// �� onKeyPressed ������ _isMoving ��־
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _isMoving = true;

    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_A:
            if (!_isMovingLeft) {
                _isMovingLeft = true;
                _playerSprite->setRotation(180); // �泯��
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if (!_isMovingRight) {
                _isMovingRight = true;
                _playerSprite->setRotation(0); // �泯��
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if (!_isMovingUp) {
                _isMovingUp = true;
                _playerSprite->setRotation(90); // �泯��
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if (!_isMovingDown) {
                _isMovingDown = true;
                _playerSprite->setRotation(270); // �泯��
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
    }
}

// ��� update ���������������ƶ�
void HelloWorld::update(float delta)
{
    if (_isMoving) {
        Vec2 loc = _playerSprite->getPosition();

        if (_isMovingLeft) {
            loc.x -= 20 * delta; // �����ƶ��ľ���
        }
        if (_isMovingRight) {
            loc.x += 20 * delta; // �����ƶ��ľ���
        }
        if (_isMovingUp) {
            loc.y += 20 * delta; // �����ƶ��ľ���
        }
        if (_isMovingDown) {
            loc.y -= 20 * delta; // �����ƶ��ľ���
        }

        // ��ȡ��Ļ�߽�
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        // �߽���
        loc.x = MAX(origin.x, MIN(loc.x, origin.x + visibleSize.width));
        loc.y = MAX(origin.y, MIN(loc.y, origin.y + visibleSize.height));

        _playerSprite->setPosition(loc);
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

    // ������ͼ����
    auto backgroundSprite = Sprite::create("bg.png");
    if (backgroundSprite == nullptr)
    {
        problemLoading("'bg.png'");
    }
    else
    {
        // ���ñ���ͼ���λ��
        backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x,
            visibleSize.height / 2 + origin.y));

        // ��ȡ����ԭʼ�ߴ�
        Size bgSize = backgroundSprite->getContentSize();

        // �������ű���
        float scaleX = visibleSize.width / bgSize.width;
        float scaleY = visibleSize.height / bgSize.height;
        float scale = MAX(scaleX, scaleY); // ���ֿ�߱ȣ����ܻ�ü�

        // Ӧ�����ű���
        backgroundSprite->setScale(scale);

        // ��������ӵ�������
        this->addChild(backgroundSprite, -1); // ���������Ԫ�ص�����
    }

    // ����һ������
    _playerSprite = Sprite::create("man.png"); // ���丳ֵ����Ա����
    if (_playerSprite == nullptr)
    {
        problemLoading("'player1.png'");
    }
    else
    {
        // ���þ����λ��
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // ��������Сһ��
        _playerSprite->setScale(0.5f); // ��Сһ��

        // ����һ������
        auto animation = Animation::create();
        animation->addSpriteFrameWithFile("man.png");
        animation->addSpriteFrameWithFile("man.png");
        animation->setDelayPerUnit(0.5f); // ÿ֡�����ĳ���ʱ��
        animation->setLoops(-1); // ����ѭ��

        // ����һ����������
        auto animate = Animate::create(animation);

        // �þ���ִ�ж���
        _playerSprite->runAction(animate);
        
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

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

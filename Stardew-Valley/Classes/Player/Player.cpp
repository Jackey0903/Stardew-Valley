/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Player.cpp
 * 文件功能      : 玩家类的功能实现
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/

#include "proj.win32/AudioPlayer.h"
#include "Player.h"
#include "Scene/BackpackScene.h"
#include "Scene/MapScene.h"
#include "Scene/MapLayer.h"
#include "Player/Backpack.h"
#include "Player/Item.h"

USING_NS_CC;

extern std::string g_selectedMap;

Player* Player::create()
{
    Player* ret = new(std::nothrow) Player();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Player::init()
{
    if (!Node::init())
        return false;

    _backpack = new Backpack();
    _backpack->addItem(new Item("Potion", "sword.png", 10));
    _backpack->addItem(new Item("Sword", "sword.png", 1));
    _backpack->addItem(new Item("Shield", "sword.png", 1));

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 将玩家初始位置设置在屏幕中央
    _playerSprite = Sprite::create("Stand_Down.png");
    _playerSprite->setScale(0.5f );
    _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    CCLOG("Player Sprite Position: %f, %f", _playerSprite->getPosition().x, _playerSprite->getPosition().y);
    this->addChild(_playerSprite);

    _isMoving = false;
    _isMovingLeft = false;
    _isMovingRight = false;
    _isMovingUp = false;
    _isMovingDown = false;
    _isInBackpackScene = false;
    _isInMapScene = false;
    _currentDirection = "Down";
    _currentTexture = "Stand_Down.png";
    _tiledMap = nullptr;

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
    return true;
}

void Player::update(float delta)
{
    if (!_tiledMap) {
        // 未设置地图则不做逻辑
        return;
    }

    Vec2 loc = _playerSprite->getPosition();
    Vec2 mapPos = _tiledMap->getPosition();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Rect playerBounds(
        visibleSize.width / 2 - 50,
        visibleSize.height / 2 - 50,
        100,
        100
    );

    Vec2 newLoc = loc;
    float speed = 50.0f;
    if (_isMovingLeft) newLoc.x -= speed * delta;
    if (_isMovingRight) newLoc.x += speed * delta;
    if (_isMovingUp) newLoc.y += speed * delta;
    if (_isMovingDown) newLoc.y -= speed * delta;

    _playerSprite->setPosition(newLoc);

    Rect playerRect = _playerSprite->getBoundingBox();
    Rect adjustedPlayerRect(
        playerRect.origin.x - (mapPos.x - initPositionMap.x),
        playerRect.origin.y - (mapPos.y - initPositionMap.y),
        playerRect.size.width,
        playerRect.size.height
    );

    if (isCollidingWithWall(adjustedPlayerRect))
    {
        CCLOG("Player is colliding with a wall.");
        _playerSprite->setPosition(loc);
        return;
    }

    if (!playerBounds.containsPoint(newLoc)) {
        // 玩家超出中间区域则移动地图
        if (_isMovingLeft && playerBounds.getMinX() > newLoc.x) {
            mapPos.x += speed * delta;
        }
        if (_isMovingRight && playerBounds.getMaxX() < newLoc.x) {
            mapPos.x -= speed * delta;
        }
        if (_isMovingUp && playerBounds.getMaxY() < newLoc.y) {
            mapPos.y -= speed * delta;
        }
        if (_isMovingDown && playerBounds.getMinY() > newLoc.y) {
            mapPos.y += speed * delta;
        }

        // 地图已移动，玩家位置重置为loc
        _playerSprite->setPosition(loc);
    }
    else {
        loc = newLoc;
        _playerSprite->setPosition(loc);
    }

    /* mapPos.x = MAX(visibleSize.width - _tiledMap->getContentSize().width, MIN(0, mapPos.x));
     mapPos.y = MAX(visibleSize.height - _tiledMap->getContentSize().height, MIN(0, mapPos.y));*/
    _tiledMap->setPosition(mapPos);
}

bool Player::isCollidingWithWall(const cocos2d::Rect& rect)
{
    auto parentScene = this->getParent();
    if (!parentScene) return false;

    auto mapLayer = parentScene->getChildByName<MapLayer*>("MapLayer");
    if (!mapLayer) return false;

    auto wallRects = mapLayer->getWallRects();
    for (const auto& wRect : wallRects)
    {
        if (rect.intersectsRect(wRect))
        {
            return true;
        }
    }
    return false;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _isMoving = true;
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_A:
            if (!_isMovingLeft) {
                _isMovingLeft = true;
                _currentTexture = "Stand_Left.png";
                _playerSprite->setTexture(_currentTexture);
                _currentDirection = "Left";
                startWalkingAnimation(_currentDirection);
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if (!_isMovingRight) {
                _isMovingRight = true;
                _currentTexture = "Stand_Right.png";
                _playerSprite->setTexture(_currentTexture);
                _currentDirection = "Right";
                startWalkingAnimation(_currentDirection);
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if (!_isMovingUp) {
                _isMovingUp = true;
                _currentTexture = "Stand_Up.png";
                _playerSprite->setTexture(_currentTexture);
                _currentDirection = "Up";
                startWalkingAnimation(_currentDirection);
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if (!_isMovingDown) {
                _isMovingDown = true;
                _currentTexture = "Stand_Down.png";
                _playerSprite->setTexture(_currentTexture);
                _currentDirection = "Down";
                startWalkingAnimation(_currentDirection);
            }
            break;
        case EventKeyboard::KeyCode::KEY_B:
            _isInBackpackScene = true;
            openBackpack();
            break;
        case EventKeyboard::KeyCode::KEY_M:
            _isInMapScene = true;
            openMapScene();
            break;
        default:
            break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
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
        case EventKeyboard::KeyCode::KEY_B:
            _isInBackpackScene = false;
            break;
        case EventKeyboard::KeyCode::KEY_M:
            _isInMapScene = false;
            break;
        default:
            break;
    }

    if (!_isMovingLeft && !_isMovingRight && !_isMovingUp && !_isMovingDown)
    {
        _isMoving = false;
        if (!_currentDirection.empty())
        {
            stopWalkingAnimation();
            _currentDirection = "";
        }
    }
}

void Player::startWalkingAnimation(const std::string& direction)
{
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 2; i++)
    {
        std::string frameName = "Walk_" + direction + "_" + std::to_string(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) frames.pushBack(frame);
    }
    auto walkAnimation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(walkAnimation);
    _playerSprite->runAction(RepeatForever::create(animate));
}

void Player::stopWalkingAnimation()
{
    _playerSprite->stopAllActions();
    if (!_currentDirection.empty())
        _playerSprite->setTexture("Stand_" + _currentDirection + ".png");
}

void Player::openBackpack()
{
    auto scene = BackpackScene::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
}

void Player::openMapScene()
{
    auto mapScene = MapScene::createScene();
    Director::getInstance()->pushScene(TransitionFade::create(0.5f, mapScene));
}

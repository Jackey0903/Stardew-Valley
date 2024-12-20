// Player.cpp
#include "Player.h"
#include "Scene/BackpackScene.h"
#include "Scene/MapScene.h"
#include "Scene/MapLayer.h"
#include "Backpack.h"
#include "Item.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

extern std::string g_selectedMap;
extern float speed;

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

    // 初始化背包
    _backpack = new Backpack();
    _backpack->addItem(new Item("Potion", "potion.png", 10));
    _backpack->addItem(new Item("Sword", "sword.png", 1));
    _backpack->addItem(new Item("Shield", "shield.png", 1));

    // 加载行走动画帧
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建玩家精灵
    _playerSprite = Sprite::create("Stand_Down.png");
    _playerSprite->setScale(0.5f);
    if (g_selectedMap == "" || g_selectedMap == "Map/Map3/map3.tmx")
    {
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x - 40, visibleSize.height / 2 + origin.y - 10));
    }
    else if (g_selectedMap == "Map/Map2/map2.tmx")
    {
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x - 150, visibleSize.height / 2 + origin.y - 120));
    }
    else
    {
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x + 50, visibleSize.height / 2 + origin.y - 50));
    }
    CCLOG("Player Sprite Position: %f, %f", _playerSprite->getPosition().x, _playerSprite->getPosition().y);
    this->addChild(_playerSprite);

    // 初始化移动状态
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
    initPositionMap = Vec2(0, 0); // 初始化地图初始位置

    // 设置键盘监听器
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    this->scheduleUpdate();
    return true;
}

void Player::setTiledMap(TMXTiledMap* tiledMap)
{
    _tiledMap = tiledMap;
    CCLOG("Player 设置了 TMXTiledMap");
}

void Player::setInitPositionMap(const Vec2& initMapPosition)
{
    initPositionMap = initMapPosition;
    CCLOG("Player 初始化地图位置: (%.2f, %.2f)", initPositionMap.x, initPositionMap.y);
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
    speed = 200.0f; // 根据需要调整速度
    if (_isMovingLeft) newLoc.x -= speed * delta;
    if (_isMovingRight) newLoc.x += speed * delta;
    if (_isMovingUp) newLoc.y += speed * delta;
    if (_isMovingDown) newLoc.y -= speed * delta;

    // 设置新位置前记录当前位置
    Vec2 previousLoc = loc;

    _playerSprite->setPosition(newLoc);

    // 获取地图的位置变化
    Vec2 newMapPos = mapPos;

    // 如果玩家超出中心区域，移动地图
    if (!playerBounds.containsPoint(newLoc)) {
        if (_isMovingLeft && (newLoc.x < playerBounds.getMinX())) {
            newMapPos.x += speed * delta;
        }
        if (_isMovingRight && (newLoc.x > playerBounds.getMaxX())) {
            newMapPos.x -= speed * delta;
        }
        if (_isMovingUp && (newLoc.y > playerBounds.getMaxY())) {
            newMapPos.y -= speed * delta;
        }
        if (_isMovingDown && (newLoc.y < playerBounds.getMinY())) {
            newMapPos.y += speed * delta;
        }

        _tiledMap->setPosition(newMapPos);
        CCLOG("地图位置更新为: (%.2f, %.2f)", newMapPos.x, newMapPos.y);

        // 玩家位置重置为中心区域边界
        Vec2 clampedPos = _playerSprite->getPosition();
        if (_isMovingLeft) clampedPos.x = playerBounds.getMinX();
        if (_isMovingRight) clampedPos.x = playerBounds.getMaxX();
        if (_isMovingUp) clampedPos.y = playerBounds.getMaxY();
        if (_isMovingDown) clampedPos.y = playerBounds.getMinY();
        _playerSprite->setPosition(clampedPos);
        CCLOG("玩家位置重置为: (%.2f, %.2f)", clampedPos.x, clampedPos.y);
    }

    // 计算玩家在地图坐标系中的位置
    Vec2 playerMapPos = _playerSprite->getPosition() - _tiledMap->getPosition();

    Rect playerRect = _playerSprite->getBoundingBox();

    Rect adjustedPlayerRect(
        playerRect.origin.x - (mapPos.x - initPositionMap.x),
        playerRect.origin.y - (mapPos.y - initPositionMap.y),
        playerRect.size.width,
        playerRect.size.height
    );

    // 调试日志
    CCLOG("Player Position: (%.2f, %.2f)", _playerSprite->getPosition().x, _playerSprite->getPosition().y);
    CCLOG("Map Position: (%.2f, %.2f)", mapPos.x, mapPos.y);
    CCLOG("Player Map Position: (%.2f, %.2f)", playerMapPos.x, playerMapPos.y);

    if (isCollidingWithWall(adjustedPlayerRect)) // 使用地图坐标系的位置
    {
        CCLOG("Player is colliding with a wall.");
        _playerSprite->setPosition(previousLoc);
        _tiledMap->setPosition(mapPos); // 重置地图位置
        CCLOG("地图位置重置为: (%.2f, %.2f)", mapPos.x, mapPos.y);
        return;
    }
}

bool Player::isCollidingWithWall(const Rect& rect)
{
    if (!_tiledMap) return false;

    // 获取名为 "MapLayer" 的地图层
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

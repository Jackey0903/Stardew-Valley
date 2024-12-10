/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Player.cpp
 * 文件功能      : 玩家类的功能实现
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/
#include "Player.h"
#include "proj.win32/AudioPlayer.h"

USING_NS_CC;

Scene* Player::createScene()
{
    return Player::create();
}
bool Player::isCollidingWithWall(const cocos2d::Rect& playerRect)
{
    for (const auto& wallRect : _wallRects)
    {
        if (playerRect.intersectsRect(wallRect))
        {
            return true; // 发生碰撞
        }
    }
    return false; // 无碰撞
}
void Player::startWalkingAnimation(const std::string& direction)
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

void Player::stopWalkingAnimation()
{
    _playerSprite->stopAllActions(); // 停止所有动画
    _playerSprite->setTexture("Stand_" + _currentDirection + ".png"); // 设置待机状态
}

// 在 onKeyPressed 中设置 _isMoving 标志
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
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
        case EventKeyboard::KeyCode::KEY_B: // 添加对 B 键的响应
            openBackpack(); // 打开背包
            break;
        default:
            break;
    }
}

// 在 onKeyReleased 中重置 _isMoving 标志
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
        case EventKeyboard::KeyCode::KEY_B: // 添加对 B 键的响应
            _isInBackpackScene = false; // 释放标志
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
bool Player::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 初始化背包
    _backpack = new Backpack();
    _backpack->addItem(new Item("Potion", "sword.png", 10));
    _backpack->addItem(new Item("Sword", "sword.png", 1));
    _backpack->addItem(new Item("Shield", "sword.png", 1));

    // 加载瓦片地图
    _tiledMap = TMXTiledMap::create("map2-1.tmx"); // 替换为你的 .tmx 文件路径
    this->addChild(_tiledMap, 0); // 将地图添加到场景中

 
	_tiledMap->setScale(3.0f); // 缩放地图（可选）
    
    // 获取地图层
    //auto layer = _tiledMap->getLayer("grass"); // 将 "LayerName" 替换为你地图的实际层名称

    // 如果需要，你可以对地图层进行进一步的操作
    // 比如修改位置、透明度等
    //layer->setPosition(Vec2(0, 0));

    // 预加载纹理图集文件
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建一个精灵
    _playerSprite = Sprite::create("Stand_Down.png"); // 将其赋值给成员变量
    // 设置精灵的位置
    _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
      
    // 将精灵缩小一半
    _playerSprite->setScale(0.5f); // 缩小一半

    // 将精灵添加到场景中
    this->addChild(_playerSprite, 2);
	_playerSprite->setPosition(Vec2(visibleSize.width / 2-30, visibleSize.height / 2));
    // 创建键盘事件监听器
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 设置更新回调
    this->schedule(CC_SCHEDULE_SELECTOR(Player::update));

    // 初始化动画
    _walkAnimation = nullptr; // 初始化为 nullptr



    auto wallLayer = _tiledMap->getLayer("wall"); // 确保层名称正确
    if (wallLayer)
    {
        // 遍历所有瓦片
        for (int x = 0; x < wallLayer->getLayerSize().width; ++x)
        {
            for (int y = 0; y < wallLayer->getLayerSize().height; ++y)
            {
                auto tileGID = wallLayer->getTileGIDAt(Vec2(x, y));
                if (tileGID != 0) // 假设 GID 为 0 表示无效瓦片
                {
                    // 获取瓦片位置
                    Vec2 tilePos = wallLayer->getPositionAt(Vec2(x, y));
                    // 获取瓦片大小
                    Size tileSize = _tiledMap->getTileSize();
                    // 创建瓦片矩形
                    Rect tileRect = Rect(tilePos.x, tilePos.y, tileSize.width, tileSize.height);
                    _wallRects.push_back(tileRect);
                }
            }
        }
        CCLOG("Collected %zu wall rectangles.", _wallRects.size());
    }
    else
    {
        CCLOG("Wall layer is null");
    }




    return true;
}

// 添加 update 方法来处理连续移动
void Player::update(float delta)
{
    if (_isMoving) {
        Vec2 loc = _playerSprite->getPosition();
		Vec2 loc2 = _playerSprite->getPosition();
        if (_isMovingLeft) {
            loc.x -= 200 * delta;
            //_tiledMap->setPositionX(_tiledMap->getPositionX() + 200 * delta); // 背景右移
        }
        if (_isMovingRight) {
            loc.x += 200 * delta;
            //_backgroundSprite->setPositionX(_backgroundSprite->getPositionX() - 200 * delta); // 背景左移
        }
        if (_isMovingUp) {
            loc.y += 200 * delta;
            //_backgroundSprite->setPositionY(_backgroundSprite->getPositionY() - 200 * delta); // 背景下移
        }
        if (_isMovingDown) {
            loc.y -= 200 * delta;
            //_backgroundSprite->setPositionY(_backgroundSprite->getPositionY() + 200 * delta); // 背景上移
        }

        // 边界检测，限制背景和精灵在可见区域内
        auto visibleSize = Director::getInstance()->getVisibleSize();
        loc.x = MAX(0, MIN(loc.x, visibleSize.width));
        loc.y = MAX(0, MIN(loc.y, visibleSize.height));
        _playerSprite->setPosition(loc);
        // 创建玩家的矩形
        Rect playerRect = _playerSprite->getBoundingBox();

        // 检查与墙壁的碰撞
        if (isCollidingWithWall(playerRect))
        {
            _playerSprite->setPosition(loc2);
            return;
        }

        Backpack* backpack = new Backpack();
        backpack->addItem(new Item("小麦种子", "player.png", 10));
        backpack->addItem(new Item("锄头", "tool.png", 1));

        // 列出背包中的物品
        backpack->listItems();



        _playerSprite->setPosition(loc);
    }
}

void Player::openBackpack() {
    auto scene = BackpackScene::createScene(); // 传递背包实例
    Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
}


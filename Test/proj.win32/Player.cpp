#include "Player.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* Player::createScene()
{
    return Player::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
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

    // 加载瓦片地图
    _tiledMap = TMXTiledMap::create("map1.tmx"); // 替换为你的 .tmx 文件路径
    if (_tiledMap == nullptr) {
        problemLoading("map1.tmx");
        CCLOG("Error: _tiledMap is null!");
        return false;
    }
    this->addChild(_tiledMap, 0); // 将地图添加到场景中




    // 获取地图层

    _wallLayer = _tiledMap->getLayer("wall"); // 获取墙壁图层

    // 如果需要，你可以对地图层进行进一步的操作
    // 比如修改位置、透明度等


    // 预加载纹理图集文件
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Right.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Walk_Down.plist");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //// 创建地图背景
    //_backgroundSprite = Sprite::create("Pierres General Store.png");
    //if (_backgroundSprite == nullptr)
    //{
    //    problemLoading("Pierres General Store.png");
    //}
    //else
    //{
    //    // 获取背景原始尺寸
    //    Size bgSize = _backgroundSprite->getContentSize();

    //    // 获取可见尺寸
    //    auto visibleSize = Director::getInstance()->getVisibleSize();

    //    // 计算缩放比例
    //    float scaleX = visibleSize.width / bgSize.width;
    //    float scaleY = visibleSize.height / bgSize.height;
    //    float scale = std::max(scaleX, scaleY); // 保持宽高比，可能会裁剪

    //    // 应用缩放比例
    //    _backgroundSprite->setScale(scale);

    //    // 设置背景图像的位置到屏幕中心
    //    _backgroundSprite->setPosition(Vec2(visibleSize.width / 2,
    //        visibleSize.height / 2));

    //    // 将背景添加到场景中
    //    this->addChild(_backgroundSprite, -1); // 添加在其他元素的下面
    //}

    // 创建一个精灵
    _playerSprite = Sprite::create("Stand_Down.png"); // 将其赋值给成员变量
    if (_playerSprite == nullptr)
    {
        problemLoading("'player1.png'");
    }
    else
    {
        // 设置精灵的位置
        _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // 将精灵缩小一半
        _playerSprite->setScale(0.5f); // 缩小一半

        // 将精灵添加到场景中
        this->addChild(_playerSprite, 2);
    }

    // 创建键盘事件监听器
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 设置更新回调
    this->schedule(CC_SCHEDULE_SELECTOR(Player::update));

    // 初始化动画
    _walkAnimation = nullptr; // 初始化为 nullptr

    return true;
}

bool Player::isCollisionWithWall(const cocos2d::Vec2& nextPosition)
{
    // 获取墙体图层
    if (!_wallLayer)
    {
        CCLOG("Error: 'wall' layer not found in the tiled map.");
        return false; // 如果没有找到墙体图层，直接返回 false
    }

    // 将屏幕坐标转换为瓦片坐标
    Size tileSize = _tiledMap->getTileSize();
    Vec2 tileCoord(
        nextPosition.x / tileSize.width,   // 计算 x 坐标的瓦片位置
        nextPosition.y / tileSize.height   // 计算 y 坐标的瓦片位置
    );

    // 获取该位置的瓦片 GID
    int tileGID = _wallLayer->getTileGIDAt(tileCoord);

    // 如果该瓦片的 GID 不为 0，表示该位置有墙壁
    return (tileGID != 0); // 如果碰到墙壁，返回 true
}



// 添加 update 方法来处理连续移动
void Player::update(float delta)
{
    if (_isMoving) {
        Vec2 loc = _playerSprite->getPosition();

        if (_isMovingLeft) {
            loc.x -= 200 * delta;
            //_backgroundSprite->setPositionX(_backgroundSprite->getPositionX() + 200 * delta); // 背景右移
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

        //检查日志输出
        //CCLOG("Player initialized");  // 确认是否成功进入 init()
        //CCLOG("Update called");       // 确认 update 是否被定期调用

        //_playerSprite->setPosition(loc);



        // 判断是否碰到墙壁
        if (!isCollisionWithWall(loc)) {
            _playerSprite->setPosition(loc);  // 如果没有碰撞，更新玩家位置
            CCLOG("Player moved to (%f, %f)", loc.x, loc.y); // 输出调试信息
        }
        else {
            CCLOG("Collision detected at (%f, %f)", loc.x, loc.y); // 输出碰撞信息
        }


    }
}


/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Player.cpp
 * 文件功能      : 玩家类实现
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#include "Player.h"
#include "Backpack.h"
#include "Item.h"
#include "Scene/BackpackScene.h"
#include "Scene/MapLayer.h"
#include "Scene/MapScene.h"
#include "proj.win32/Constant.h"
#include "ui/CocosGUI.h"

// ==================================================
// Refactored with Flyweight Pattern
// ==================================================
#include "Animation/AnimationFlyweight.h"

USING_NS_CC;

extern std::string g_selectedMap;
extern float speed;

// 创建Player实例，采用C++11的智能指针方式处理内存管理
Player *Player::create() {
  Player *ret = new (std::nothrow)
      Player();           // 使用new(std::nothrow)避免内存分配失败时抛出异常
  if (ret && ret->init()) // 如果初始化成功，返回对象
  {
    ret->autorelease(); // 使用autorelease自动释放内存
    return ret;
  }
  delete ret; // 内存分配失败时手动释放
  return nullptr;
}

// 初始化Player对象
bool Player::init() {
  if (!Node::init()) // 初始化基类Node
    return false;

  // 初始化背包并添加初始物品
  _backpack = new Backpack();
  _backpack->addItem(new Item("Sword", "Bag/sword.png", 1));

  // ==================================================
  // [ORIGINAL CODE - Before Flyweight Pattern]
  // 加载行走动画的帧 - 原始实现：每次都直接加载
  // SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
  //     "Action/Walk_Left.plist");
  // SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
  //     "Action/Walk_Right.plist");
  // SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
  //     "Action/Walk_Up.plist");
  // SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
  //     "Action/Walk_Down.plist");
  // ==================================================

  // ==================================================
  // [REFACTORED CODE - With Flyweight Pattern]
  // 使用享元工厂预加载动画，共享动画资源
  // ==================================================
  AnimationFlyweight::getInstance()->preloadPlayerAnimations();

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // 创建玩家精灵并设置初始位置
  _playerSprite = Sprite::create("Action/Stand_Down.png");
  _playerSprite->setScale(0.5f); // 设置精灵缩放比例

  // 根据选择的地图调整玩家的位置
  if (g_selectedMap == "" || g_selectedMap == "Map/Map3/map3.tmx") {
    _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x - 40,
                                    visibleSize.height / 2 + origin.y - 10));
  } else if (g_selectedMap == "Map/Map2/map2.tmx") {
    _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x + 50,
                                    visibleSize.height / 2 + origin.y));
  } else {
    _playerSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x + 50,
                                    visibleSize.height / 2 + origin.y - 50));
  }

  // 调试日志：输出玩家精灵的位置
  CCLOG("Player Sprite Position: %f, %f", _playerSprite->getPosition().x,
        _playerSprite->getPosition().y);
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
  _currentTexture = "Action/Stand_Down.png";
  _tiledMap = nullptr;
  initPositionMap = Vec2(0, 0); // 初始化地图初始位置

  // ==================================================
  // [ORIGINAL CODE - Before Adapter Pattern]
  // initTouchControls();
  // _keyboardListener = EventListenerKeyboard::create();
  // _keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed,
  // this); _keyboardListener->onKeyReleased =
  // CC_CALLBACK_2(Player::onKeyReleased, this);
  // _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener,
  // this);
  // ==================================================

  // ==================================================
  // [REFACTORED CODE - With Adapter Pattern]
  // 使用输入适配器统一管理键盘和触摸输入
  // ==================================================
  initInputAdapters();

  this->scheduleUpdate(); // 每帧更新
  return true;
}

// 设置当前地图
void Player::setTiledMap(TMXTiledMap *tiledMap) {
  _tiledMap = tiledMap;
  CCLOG("Player 设置了 TMXTiledMap");
}

// 设置初始地图位置
void Player::setInitPositionMap(const Vec2 &initMapPosition) {
  initPositionMap = initMapPosition;
  CCLOG("Player 初始化地图位置: (%.2f, %.2f)", initPositionMap.x,
        initPositionMap.y);
}

// 每帧更新玩家位置
void Player::update(float delta) {
  if (!_tiledMap) {
    // 如果没有设置地图，则不进行更新
    return;
  }

  Vec2 loc = _playerSprite->getPosition(); // 获取玩家当前精灵的位置
  Vec2 mapPos = _tiledMap->getPosition();  // 获取地图的位置

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Rect playerBounds(visibleSize.width / 2 - 50, visibleSize.height / 2 - 50,
                    100, 100);

  Vec2 newLoc = loc;
  if (_isMovingLeft)
    newLoc.x -= speed * delta;
  if (_isMovingRight)
    newLoc.x += speed * delta;
  if (_isMovingUp)
    newLoc.y += speed * delta;
  if (_isMovingDown)
    newLoc.y -= speed * delta;

  // 设置新位置前记录当前位置
  Vec2 previousLoc = loc;

  _playerSprite->setPosition(newLoc);                // 设置玩家的新位置
  Rect playerRect = _playerSprite->getBoundingBox(); // 获取玩家精灵的包围盒

  Rect adjustedPlayerRect(playerRect.origin.x, playerRect.origin.y,
                          playerRect.size.width, playerRect.size.height);

  // 判断是否与墙壁发生碰撞
  if (isCollidingWithWall(adjustedPlayerRect)) // C++11特性：lambda函数
  {
    CCLOG("Player is colliding with a wall.");
    _playerSprite->setPosition(previousLoc); // 恢复玩家位置
    return;
  }

  // 获取地图的新位置
  Vec2 newMapPos = mapPos;

  // 如果玩家超出中心区域，则移动地图
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

    _tiledMap->setPosition(newMapPos);       // 更新地图位置
    _playerSprite->setPosition(previousLoc); // 恢复玩家位置
  }
}

// 检查玩家是否与墙壁发生碰撞
bool Player::isCollidingWithWall(const Rect &rect) {
  if (!_tiledMap)
    return false; // 如果没有地图，则返回false

  // 获取地图层对象
  auto parentScene = this->getParent();
  if (!parentScene)
    return false;

  auto mapLayer = parentScene->getChildByName<MapLayer *>("MapLayer");
  if (!mapLayer)
    return false;

  Vec2 mapAnchor = _tiledMap->getAnchorPointInPoints() * _tiledMap->getScale();
  Vec2 mapPos = _tiledMap->getPosition();
  auto wallRects = mapLayer->getWallRects();
  mapLayer->collisionDrawNode->clear(); // 清除之前的碰撞绘制

  Vec2 vertices[4] = {
      Vec2(rect.origin.x, rect.origin.y),
      Vec2(rect.origin.x + rect.size.width, rect.origin.y),
      Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height),
      Vec2(rect.origin.x, rect.origin.y + rect.size.height)};

  // mapLayer->collisionDrawNode->drawPolygon(vertices, 4, Color4F(0, 0, 0, 0),
  // 1, Color4F(1, 0, 0, 1));  // 绘制碰撞区域
  for (const auto &wRect : wallRects) {
    float x = wRect.origin.x - mapAnchor.x + mapPos.x;
    float y = wRect.origin.y - mapAnchor.y + mapPos.y;
    float width = wRect.size.width;
    float height = wRect.size.height;
    Rect RectinWindow(x, y, width, height);
    Vec2 vertices[4] = {Vec2(RectinWindow.origin.x, RectinWindow.origin.y),
                        Vec2(RectinWindow.origin.x + RectinWindow.size.width,
                             RectinWindow.origin.y),
                        Vec2(RectinWindow.origin.x + RectinWindow.size.width,
                             RectinWindow.origin.y + RectinWindow.size.height),
                        Vec2(RectinWindow.origin.x,
                             RectinWindow.origin.y + RectinWindow.size.height)};
    // mapLayer->collisionDrawNode->drawPolygon(vertices, 4, Color4F(0, 0, 0,
    // 0), 1, Color4F(1, 0, 0, 1));
    if (rect.intersectsRect(RectinWindow)) // 检查碰撞
    {
      return true;
    }
  }
  return false;
}

// ==================================================
// [ORIGINAL CODE - Before Adapter Pattern]
// 按键按下事件 - 原始实现
// ==================================================
// void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
//   _isMoving = true;
//   switch (keyCode) {
//   case EventKeyboard::KeyCode::KEY_A:
//     if (!_isMovingLeft) {
//       _isMovingLeft = true;
//       _currentTexture = "Action/Stand_Left.png";
//       _playerSprite->setTexture(_currentTexture);
//       _currentDirection = "Left";
//       startWalkingAnimation(_currentDirection);
//     }
//     break;
//   // ... 其他方向类似 ...
//   case EventKeyboard::KeyCode::KEY_B:
//     _isInBackpackScene = true;
//     openBackpack();
//     break;
//   case EventKeyboard::KeyCode::KEY_M:
//     _isInMapScene = true;
//     openMapScene();
//     break;
//   default:
//     break;
//   }
// }
// ==================================================

// ==================================================
// [ORIGINAL CODE - Before Adapter Pattern]
// 按键释放事件 - 原始实现
// ==================================================
// void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
//   switch (keyCode) {
//   case EventKeyboard::KeyCode::KEY_A:
//     _isMovingLeft = false;
//     break;
//   // ... 其他方向类似 ...
//   }
//   if (!_isMovingLeft && !_isMovingRight && !_isMovingUp && !_isMovingDown) {
//     _isMoving = false;
//     if (!_currentDirection.empty()) {
//       stopWalkingAnimation();
//       _currentDirection = "";
//     }
//   }
// }
// ==================================================

// ==================================================
// [REFACTORED CODE - With Adapter Pattern]
// IInputHandler 接口实现 - 统一的输入处理
// ==================================================

// 初始化输入适配器
void Player::initInputAdapters() {
  // 创建键盘适配器
  _keyboardAdapter = new KeyboardInputAdapter(this);
  _keyboardAdapter->setupListener(this);

  // 创建触摸适配器
  _touchAdapter = new TouchInputAdapter(this);
  _touchAdapter->createButtons(this, KEYS_CENTER_X, KEYS_CENTER_Y, KEYS_RADIUS);

  CCLOG("[Player] Input adapters initialized");
}

// 移动开始 - IInputHandler接口实现
void Player::onMoveStart(const std::string &direction) {
  _isMoving = true;

  if (direction == "Up" && !_isMovingUp) {
    _isMovingUp = true;
    _currentDirection = "Up";
  } else if (direction == "Down" && !_isMovingDown) {
    _isMovingDown = true;
    _currentDirection = "Down";
  } else if (direction == "Left" && !_isMovingLeft) {
    _isMovingLeft = true;
    _currentDirection = "Left";
  } else if (direction == "Right" && !_isMovingRight) {
    _isMovingRight = true;
    _currentDirection = "Right";
  }

  _currentTexture = "Action/Stand_" + _currentDirection + ".png";
  _playerSprite->setTexture(_currentTexture);
  startWalkingAnimation(_currentDirection);
}

// 移动停止 - IInputHandler接口实现
void Player::onMoveStop(const std::string &direction) {
  if (direction == "Up") {
    _isMovingUp = false;
  } else if (direction == "Down") {
    _isMovingDown = false;
  } else if (direction == "Left") {
    _isMovingLeft = false;
  } else if (direction == "Right") {
    _isMovingRight = false;
  }

  if (!_isMovingUp && !_isMovingDown && !_isMovingLeft && !_isMovingRight) {
    _isMoving = false;
    if (!_currentDirection.empty()) {
      stopWalkingAnimation();
      _currentDirection = "";
    }
  }
}

// 动作事件 - IInputHandler接口实现
void Player::onAction(const std::string &action) {
  if (action == "Backpack") {
    _isInBackpackScene = true;
    openBackpack();
  } else if (action == "Map") {
    _isInMapScene = true;
    openMapScene();
  }
}

// ==================================================
// [ORIGINAL CODE - Before Flyweight Pattern]
// 开始行走动画 - 原始实现：每次都创建新的Animation对象
// ==================================================
// void Player::startWalkingAnimation(const std::string &direction) {
//   Vector<SpriteFrame *> frames;
//   for (int i = 1; i <= 2; i++) {
//     std::string frameName = "Action/Walk_" + direction + "_" +
//                             std::to_string(i) +
//                             ".png"; // C++11特性：std::to_string
//     auto frame =
//         SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
//     if (frame)
//       frames.pushBack(frame);
//   }
//   auto walkAnimation =
//       Animation::createWithSpriteFrames(frames, 0.1f);      // 创建行走动画
//   auto animate = Animate::create(walkAnimation);            // 创建动画
//   _playerSprite->runAction(RepeatForever::create(animate)); // 执行循环动画
// }
// ==================================================

// ==================================================
// [REFACTORED CODE - With Flyweight Pattern]
// 开始行走动画 - 重构后：从享元池获取共享的Animation对象
// ==================================================
void Player::startWalkingAnimation(const std::string &direction) {
  // 从享元工厂获取共享的动画对象
  std::string animKey = "Player_Walk_" + direction;
  auto animation = AnimationFlyweight::getInstance()->getAnimation(animKey);

  if (animation) {
    auto animate = Animate::create(animation);
    _playerSprite->runAction(RepeatForever::create(animate));
  } else {
    // 兼容性回退：如果享元池中没有，使用原始方式
    CCLOG("[Player] Animation not found in Flyweight pool: %s",
          animKey.c_str());
    Vector<SpriteFrame *> frames;
    for (int i = 1; i <= 2; i++) {
      std::string frameName =
          "Action/Walk_" + direction + "_" + std::to_string(i) + ".png";
      auto frame =
          SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
      if (frame)
        frames.pushBack(frame);
    }
    if (!frames.empty()) {
      auto walkAnimation = Animation::createWithSpriteFrames(frames, 0.1f);
      auto animate = Animate::create(walkAnimation);
      _playerSprite->runAction(RepeatForever::create(animate));
    }
  }
}

// 停止行走动画
void Player::stopWalkingAnimation() {
  _playerSprite->stopAllActions(); // 停止所有动作
  if (!_currentDirection.empty())
    _playerSprite->setTexture("Action/Stand_" + _currentDirection +
                              ".png"); // 设置站立动画
}

// 打开背包场景
void Player::openBackpack() {
  auto scene = BackpackScene::createScene();
  Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
}

// 打开地图场景
void Player::openMapScene() {
  auto mapScene = MapScene::createScene();
  Director::getInstance()->pushScene(TransitionFade::create(0.5f, mapScene));
}

// 安卓端控制按钮处理
void Player::initTouchControls() {
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // 按钮大小和透明度设置
  Size buttonSize(80, 80);
  float opacity = 100.0f; // 半透明

  // 创建上按钮
  _btnUp = cocos2d::ui::Button::create(
      "../Resources/KEYS/U.png"); // 需要准备半透明的按钮图片
  _btnUp->setPosition(Vec2(KEYS_CENTER_X, KEYS_CENTER_Y + KEYS_RADIUS));
  _btnUp->setScale(0.2f);
  _btnUp->setOpacity(opacity);
  this->addChild(_btnUp);

  // 创建下按钮
  _btnDown = cocos2d::ui::Button::create("../Resources/KEYS/D.png");
  _btnDown->setPosition(Vec2(KEYS_CENTER_X, KEYS_CENTER_Y - KEYS_RADIUS));
  _btnDown->setScale(0.2f);
  _btnDown->setOpacity(opacity);
  this->addChild(_btnDown);

  // 创建左按钮
  _btnLeft = cocos2d::ui::Button::create("../Resources/KEYS/L.png");
  _btnLeft->setPosition(Vec2(KEYS_CENTER_X - KEYS_RADIUS, KEYS_CENTER_Y));
  _btnLeft->setScale(0.2f);
  _btnLeft->setOpacity(opacity);
  this->addChild(_btnLeft);

  // 创建右按钮
  _btnRight = cocos2d::ui::Button::create("../Resources/KEYS/R.png");
  _btnRight->setPosition(Vec2(KEYS_CENTER_X + KEYS_RADIUS, KEYS_CENTER_Y));
  _btnRight->setScale(0.2f);
  _btnRight->setOpacity(opacity);
  this->addChild(_btnRight);

  // 创建背包按钮
  _btnBackpack = cocos2d::ui::Button::create("../Resources/KEYS/B.png");
  _btnBackpack->setPosition(
      Vec2(KEYS_BACKPACK_X, KEYS_BACKPACK_Y)); // 调整位置根据需求
  _btnBackpack->setScale(0.2f);
  _btnBackpack->setOpacity(opacity);
  this->addChild(_btnBackpack);

  // 创建地图按钮
  _btnMap = cocos2d::ui::Button::create("../Resources/KEYS/M.png");
  _btnMap->setPosition(Vec2(KEYS_MAP_X, KEYS_MAP_Y)); // 调整位置根据需求
  _btnMap->setScale(0.2f);
  _btnMap->setOpacity(opacity);
  this->addChild(_btnMap);

  // 设置触摸事件
  auto touchListener = EventListenerTouchOneByOne::create();
  touchListener->setSwallowTouches(true);

  // 上按钮触摸事件
  _btnUp->addTouchEventListener(
      [this](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
          _isMovingUp = true;
          _currentTexture = "Action/Stand_Up.png";
          _playerSprite->setTexture(_currentTexture);
          _currentDirection = "Up";
          startWalkingAnimation(_currentDirection);
        } else if (type == cocos2d::ui::Widget::TouchEventType::ENDED ||
                   type == cocos2d::ui::Widget::TouchEventType::CANCELED) {
          _isMovingUp = false;
          if (!_isMovingLeft && !_isMovingRight && !_isMovingDown) {
            _isMoving = false;
            stopWalkingAnimation();
          }
        }
      });

  // 下按钮触摸事件
  _btnDown->addTouchEventListener(
      [this](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
          _isMovingDown = true;
          _currentTexture = "Action/Stand_Down.png";
          _playerSprite->setTexture(_currentTexture);
          _currentDirection = "Down";
          startWalkingAnimation(_currentDirection);
        } else if (type == cocos2d::ui::Widget::TouchEventType::ENDED ||
                   type == cocos2d::ui::Widget::TouchEventType::CANCELED) {
          _isMovingDown = false;
          if (!_isMovingLeft && !_isMovingRight && !_isMovingUp) {
            _isMoving = false;
            stopWalkingAnimation();
          }
        }
      });

  // 左按钮触摸事件
  _btnLeft->addTouchEventListener(
      [this](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
          _isMovingLeft = true;
          _currentTexture = "Action/Stand_Left.png";
          _playerSprite->setTexture(_currentTexture);
          _currentDirection = "Left";
          startWalkingAnimation(_currentDirection);
        } else if (type == cocos2d::ui::Widget::TouchEventType::ENDED ||
                   type == cocos2d::ui::Widget::TouchEventType::CANCELED) {
          _isMovingLeft = false;
          if (!_isMovingRight && !_isMovingUp && !_isMovingDown) {
            _isMoving = false;
            stopWalkingAnimation();
          }
        }
      });

  // 右按钮触摸事件
  _btnRight->addTouchEventListener(
      [this](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
          _isMovingRight = true;
          _currentTexture = "Action/Stand_Right.png";
          _playerSprite->setTexture(_currentTexture);
          _currentDirection = "Right";
          startWalkingAnimation(_currentDirection);
        } else if (type == cocos2d::ui::Widget::TouchEventType::ENDED ||
                   type == cocos2d::ui::Widget::TouchEventType::CANCELED) {
          _isMovingRight = false;
          if (!_isMovingLeft && !_isMovingUp && !_isMovingDown) {
            _isMoving = false;
            stopWalkingAnimation();
          }
        }
      });
  // 背包按钮触摸事件
  _btnBackpack->addTouchEventListener(
      [this](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
          _isInBackpackScene = true;
          openBackpack(); // 打开背包
        }
      });

  // 地图按钮触摸事件
  _btnMap->addTouchEventListener(
      [this](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
          _isInMapScene = true;
          openMapScene(); // 打开地图场景
        }
      });
}
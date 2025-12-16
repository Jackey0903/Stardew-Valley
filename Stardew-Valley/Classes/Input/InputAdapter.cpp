/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : InputAdapter.cpp
 * 文件功能      : 输入适配器实现
 * 作者          : 同学1
 * 更新日期      : 2024/12/16
 * 许可证        : MIT License
 * 重构说明      : Refactored with Adapter Pattern
 ****************************************************************/

#include "InputAdapter.h"
#include "proj.win32/Constant.h"

USING_NS_CC;

// ==================================================
// Refactored with Adapter Pattern
// ==================================================

// ==================== KeyboardInputAdapter 实现 ====================

KeyboardInputAdapter::KeyboardInputAdapter(IInputHandler *handler)
    : _handler(handler), _listener(nullptr) {}

void KeyboardInputAdapter::setupListener(Node *node) {
  _listener = EventListenerKeyboard::create();
  _listener->onKeyPressed =
      CC_CALLBACK_2(KeyboardInputAdapter::onKeyPressed, this);
  _listener->onKeyReleased =
      CC_CALLBACK_2(KeyboardInputAdapter::onKeyReleased, this);
  node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener,
                                                                     node);
  CCLOG("[KeyboardInputAdapter] Keyboard listener setup complete");
}

void KeyboardInputAdapter::removeListener() {
  if (_listener) {
    Director::getInstance()->getEventDispatcher()->removeEventListener(
        _listener);
    _listener = nullptr;
    CCLOG("[KeyboardInputAdapter] Keyboard listener removed");
  }
}

void KeyboardInputAdapter::onKeyPressed(EventKeyboard::KeyCode keyCode,
                                        Event *event) {
  // 检查是否是方向键
  std::string direction = keyCodeToDirection(keyCode);
  if (!direction.empty()) {
    _handler->onMoveStart(direction);
    return;
  }

  // 检查是否是动作键
  std::string action = keyCodeToAction(keyCode);
  if (!action.empty()) {
    _handler->onAction(action);
  }
}

void KeyboardInputAdapter::onKeyReleased(EventKeyboard::KeyCode keyCode,
                                         Event *event) {
  std::string direction = keyCodeToDirection(keyCode);
  if (!direction.empty()) {
    _handler->onMoveStop(direction);
  }
}

std::string
KeyboardInputAdapter::keyCodeToDirection(EventKeyboard::KeyCode keyCode) {
  switch (keyCode) {
  case EventKeyboard::KeyCode::KEY_W:
    return "Up";
  case EventKeyboard::KeyCode::KEY_S:
    return "Down";
  case EventKeyboard::KeyCode::KEY_A:
    return "Left";
  case EventKeyboard::KeyCode::KEY_D:
    return "Right";
  default:
    return "";
  }
}

std::string
KeyboardInputAdapter::keyCodeToAction(EventKeyboard::KeyCode keyCode) {
  switch (keyCode) {
  case EventKeyboard::KeyCode::KEY_B:
    return "Backpack";
  case EventKeyboard::KeyCode::KEY_M:
    return "Map";
  default:
    return "";
  }
}

// ==================== TouchInputAdapter 实现 ====================

TouchInputAdapter::TouchInputAdapter(IInputHandler *handler)
    : _handler(handler), _btnUp(nullptr), _btnDown(nullptr), _btnLeft(nullptr),
      _btnRight(nullptr), _btnBackpack(nullptr), _btnMap(nullptr) {}

void TouchInputAdapter::createButtons(Node *parent, float centerX,
                                      float centerY, float radius) {
  float opacity = 100.0f;
  float scale = 0.2f;

  // 创建上按钮
  _btnUp = ui::Button::create("../Resources/KEYS/U.png");
  _btnUp->setPosition(Vec2(centerX, centerY + radius));
  _btnUp->setScale(scale);
  _btnUp->setOpacity(static_cast<GLubyte>(opacity));
  setupDirectionButton(_btnUp, "Up");
  parent->addChild(_btnUp);

  // 创建下按钮
  _btnDown = ui::Button::create("../Resources/KEYS/D.png");
  _btnDown->setPosition(Vec2(centerX, centerY - radius));
  _btnDown->setScale(scale);
  _btnDown->setOpacity(static_cast<GLubyte>(opacity));
  setupDirectionButton(_btnDown, "Down");
  parent->addChild(_btnDown);

  // 创建左按钮
  _btnLeft = ui::Button::create("../Resources/KEYS/L.png");
  _btnLeft->setPosition(Vec2(centerX - radius, centerY));
  _btnLeft->setScale(scale);
  _btnLeft->setOpacity(static_cast<GLubyte>(opacity));
  setupDirectionButton(_btnLeft, "Left");
  parent->addChild(_btnLeft);

  // 创建右按钮
  _btnRight = ui::Button::create("../Resources/KEYS/R.png");
  _btnRight->setPosition(Vec2(centerX + radius, centerY));
  _btnRight->setScale(scale);
  _btnRight->setOpacity(static_cast<GLubyte>(opacity));
  setupDirectionButton(_btnRight, "Right");
  parent->addChild(_btnRight);

  // 创建背包按钮
  _btnBackpack = ui::Button::create("../Resources/KEYS/B.png");
  _btnBackpack->setPosition(Vec2(KEYS_BACKPACK_X, KEYS_BACKPACK_Y));
  _btnBackpack->setScale(scale);
  _btnBackpack->setOpacity(static_cast<GLubyte>(opacity));
  setupActionButton(_btnBackpack, "Backpack");
  parent->addChild(_btnBackpack);

  // 创建地图按钮
  _btnMap = ui::Button::create("../Resources/KEYS/M.png");
  _btnMap->setPosition(Vec2(KEYS_MAP_X, KEYS_MAP_Y));
  _btnMap->setScale(scale);
  _btnMap->setOpacity(static_cast<GLubyte>(opacity));
  setupActionButton(_btnMap, "Map");
  parent->addChild(_btnMap);

  CCLOG("[TouchInputAdapter] Touch buttons created");
}

void TouchInputAdapter::setupDirectionButton(ui::Button *btn,
                                             const std::string &direction) {
  btn->addTouchEventListener(
      [this, direction](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
          _handler->onMoveStart(direction);
        } else if (type == ui::Widget::TouchEventType::ENDED ||
                   type == ui::Widget::TouchEventType::CANCELED) {
          _handler->onMoveStop(direction);
        }
      });
}

void TouchInputAdapter::setupActionButton(ui::Button *btn,
                                          const std::string &action) {
  btn->addTouchEventListener(
      [this, action](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          _handler->onAction(action);
        }
      });
}

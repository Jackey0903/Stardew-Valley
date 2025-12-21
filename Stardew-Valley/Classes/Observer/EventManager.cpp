/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : EventManager.cpp
 * 文件功能      : 事件管理器实现
 * 作者          : 同学2
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with Observer Pattern
 ****************************************************************/

#include "EventManager.h"
#include "cocos2d.h"
#include <algorithm>

USING_NS_CC;

// ==================================================
// 预定义事件类型常量
// ==================================================
const std::string EventManager::EVENT_SCENE_CHANGE = "SCENE_CHANGE";
const std::string EventManager::EVENT_BACKPACK_UPDATE = "BACKPACK_UPDATE";
const std::string EventManager::EVENT_PLAYER_MOVE = "PLAYER_MOVE";
const std::string EventManager::EVENT_CROP_PLANT = "CROP_PLANT";
const std::string EventManager::EVENT_CROP_HARVEST = "CROP_HARVEST";
const std::string EventManager::EVENT_NPC_INTERACT = "NPC_INTERACT";
const std::string EventManager::EVENT_GAME_SAVE = "GAME_SAVE";
const std::string EventManager::EVENT_GAME_LOAD = "GAME_LOAD";

// ==================================================
// Observer Pattern Implementation
// ==================================================

EventManager *EventManager::getInstance() {
  static EventManager instance;
  return &instance;
}

EventManager::EventManager() : _nextCallbackId(1) {
  CCLOG("[EventManager] Observer Pattern instance created");
}

EventManager::~EventManager() { clearAll(); }

// ==================== 观察者管理 ====================

void EventManager::addObserver(const std::string &eventType,
                               IEventObserver *observer) {
  if (!observer) {
    CCLOG("[EventManager] Warning: Trying to add null observer");
    return;
  }

  // [ORIGINAL CODE - Before Observer Pattern]
  // 原始代码中，组件之间直接调用方法，如：
  // backpackScene->updateItems();
  // gameScene->onPlayerMove();
  //
  // [REFACTORED CODE - With Observer Pattern]
  // 现在组件通过事件系统通信：
  // EventManager::getInstance()->addObserver("BACKPACK_UPDATE", this);

  _observers[eventType].push_back(observer);
  CCLOG("[EventManager] Added observer '%s' for event '%s'",
        observer->getObserverName().c_str(), eventType.c_str());
}

int EventManager::addListener(const std::string &eventType,
                              EventCallback callback) {
  if (!callback) {
    CCLOG("[EventManager] Warning: Trying to add null callback");
    return -1;
  }

  int id = _nextCallbackId++;
  _callbacks[eventType].push_back({id, callback});

  CCLOG("[EventManager] Added callback #%d for event '%s'", id,
        eventType.c_str());
  return id;
}

void EventManager::removeObserver(const std::string &eventType,
                                  IEventObserver *observer) {
  auto it = _observers.find(eventType);
  if (it != _observers.end()) {
    auto &observers = it->second;
    observers.erase(std::remove(observers.begin(), observers.end(), observer),
                    observers.end());

    CCLOG("[EventManager] Removed observer for event '%s'", eventType.c_str());
  }
}

void EventManager::removeListener(const std::string &eventType,
                                  int callbackId) {
  auto it = _callbacks.find(eventType);
  if (it != _callbacks.end()) {
    auto &callbacks = it->second;
    callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(),
                                   [callbackId](const CallbackEntry &entry) {
                                     return entry.id == callbackId;
                                   }),
                    callbacks.end());

    CCLOG("[EventManager] Removed callback #%d for event '%s'", callbackId,
          eventType.c_str());
  }
}

void EventManager::removeAllObservations(IEventObserver *observer) {
  for (auto &pair : _observers) {
    auto &observers = pair.second;
    observers.erase(std::remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

  CCLOG("[EventManager] Removed all observations for observer '%s'",
        observer->getObserverName().c_str());
}

// ==================== 事件分发 ====================

void EventManager::dispatchEvent(const GameEvent &event) {
  // [ORIGINAL CODE - Before Observer Pattern]
  // 原始代码直接调用目标对象的方法，造成紧耦合：
  // player->onBackpackUpdate();
  // gameScene->refreshUI();
  //
  // [REFACTORED CODE - With Observer Pattern]
  // 现在通过事件系统，发送者不需要知道接收者：
  // EventManager::getInstance()->dispatchEvent(GameEvent("BACKPACK_UPDATE"));

  CCLOG("[EventManager] Dispatching event: %s", event.type.c_str());

  // 通知接口观察者
  auto observerIt = _observers.find(event.type);
  if (observerIt != _observers.end()) {
    // 创建副本以防止迭代时修改
    std::vector<IEventObserver *> observersCopy = observerIt->second;
    for (auto observer : observersCopy) {
      if (observer) {
        CCLOG("[EventManager] Notifying observer: %s",
              observer->getObserverName().c_str());
        observer->onEvent(event);
      }
    }
  }

  // 通知Lambda回调
  auto callbackIt = _callbacks.find(event.type);
  if (callbackIt != _callbacks.end()) {
    // 创建副本以防止迭代时修改
    std::vector<CallbackEntry> callbacksCopy = callbackIt->second;
    for (const auto &entry : callbacksCopy) {
      if (entry.callback) {
        CCLOG("[EventManager] Calling callback #%d", entry.id);
        entry.callback(event);
      }
    }
  }
}

void EventManager::dispatchEvent(const std::string &eventType) {
  GameEvent event(eventType);
  dispatchEvent(event);
}

// ==================== 调试支持 ====================

void EventManager::logObservers() const {
  CCLOG("========== EventManager Observers ==========");

  for (const auto &pair : _observers) {
    CCLOG("Event '%s': %zu observers", pair.first.c_str(), pair.second.size());
    for (const auto &observer : pair.second) {
      if (observer) {
        CCLOG("  - %s", observer->getObserverName().c_str());
      }
    }
  }

  for (const auto &pair : _callbacks) {
    CCLOG("Event '%s': %zu callbacks", pair.first.c_str(), pair.second.size());
    for (const auto &entry : pair.second) {
      CCLOG("  - Callback #%d", entry.id);
    }
  }

  CCLOG("============================================");
}

void EventManager::clearAll() {
  _observers.clear();
  _callbacks.clear();
  CCLOG("[EventManager] Cleared all observers and callbacks");
}

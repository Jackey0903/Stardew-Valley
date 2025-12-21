/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : EventManager.h
 * 文件功能      : 事件管理器，实现观察者模式的事件分发系统
 * 作者          : 同学2
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with Observer Pattern
 ****************************************************************/

#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// ==================================================
// Refactored with Observer Pattern
// 观察者模式：实现事件驱动的松耦合通信机制
// ==================================================

/**
 * GameEvent - 游戏事件数据结构
 * 封装事件类型和相关数据
 */
struct GameEvent {
  std::string type;                                  // 事件类型
  std::unordered_map<std::string, std::string> data; // 事件数据（字符串键值对）

  GameEvent(const std::string &eventType) : type(eventType) {}

  // 添加数据
  void setData(const std::string &key, const std::string &value) {
    data[key] = value;
  }

  // 获取数据
  std::string getData(const std::string &key) const {
    auto it = data.find(key);
    return (it != data.end()) ? it->second : "";
  }
};

/**
 * IEventObserver - 事件观察者接口 (Observer)
 *
 * 设计模式：Observer（观察者模式）
 *
 * 角色说明：
 * - Subject: EventManager
 * - Observer: IEventObserver 接口
 * - ConcreteObserver: 实现此接口的类
 *
 * 问题：游戏中各组件之间直接调用，耦合度高
 * 解决：使用观察者模式，通过事件系统解耦组件通信
 */
class IEventObserver {
public:
  virtual ~IEventObserver() = default;

  /**
   * 事件回调函数
   * @param event 触发的事件
   */
  virtual void onEvent(const GameEvent &event) = 0;

  /**
   * 获取观察者名称（用于调试）
   */
  virtual std::string getObserverName() const = 0;
};

// 事件回调函数类型（用于Lambda注册）
using EventCallback = std::function<void(const GameEvent &)>;

/**
 * EventManager - 事件管理器 (Subject)
 *
 * 设计模式：Observer（观察者模式）+ Singleton
 *
 * 功能：
 * 1. 管理事件观察者的注册和注销
 * 2. 分发事件给所有注册的观察者
 * 3. 支持接口观察者和Lambda回调
 *
 * 预定义事件类型：
 * - "SCENE_CHANGE": 场景切换
 * - "BACKPACK_UPDATE": 背包更新
 * - "PLAYER_MOVE": 玩家移动
 * - "CROP_PLANT": 种植作物
 * - "CROP_HARVEST": 收获作物
 * - "NPC_INTERACT": NPC交互
 */
class EventManager {
public:
  // 预定义事件类型常量
  static const std::string EVENT_SCENE_CHANGE;
  static const std::string EVENT_BACKPACK_UPDATE;
  static const std::string EVENT_PLAYER_MOVE;
  static const std::string EVENT_CROP_PLANT;
  static const std::string EVENT_CROP_HARVEST;
  static const std::string EVENT_NPC_INTERACT;
  static const std::string EVENT_GAME_SAVE;
  static const std::string EVENT_GAME_LOAD;

  /**
   * 获取单例实例
   */
  static EventManager *getInstance();

  // ==================== 观察者管理 ====================

  /**
   * 注册观察者（接口方式）
   * @param eventType 要监听的事件类型
   * @param observer 观察者对象
   */
  void addObserver(const std::string &eventType, IEventObserver *observer);

  /**
   * 注册观察者（Lambda方式）
   * @param eventType 要监听的事件类型
   * @param callback 回调函数
   * @return 回调ID，用于注销
   */
  int addListener(const std::string &eventType, EventCallback callback);

  /**
   * 注销观察者（接口方式）
   * @param eventType 事件类型
   * @param observer 要移除的观察者
   */
  void removeObserver(const std::string &eventType, IEventObserver *observer);

  /**
   * 注销观察者（Lambda方式）
   * @param eventType 事件类型
   * @param callbackId 回调ID
   */
  void removeListener(const std::string &eventType, int callbackId);

  /**
   * 移除指定观察者的所有订阅
   */
  void removeAllObservations(IEventObserver *observer);

  // ==================== 事件分发 ====================

  /**
   * 发送事件（通知所有观察者）
   * @param event 要发送的事件
   */
  void dispatchEvent(const GameEvent &event);

  /**
   * 发送简单事件（无附加数据）
   * @param eventType 事件类型
   */
  void dispatchEvent(const std::string &eventType);

  // ==================== 调试支持 ====================

  /**
   * 打印当前注册的观察者信息
   */
  void logObservers() const;

  /**
   * 清除所有观察者
   */
  void clearAll();

private:
  EventManager();
  ~EventManager();

  // 禁用拷贝
  EventManager(const EventManager &) = delete;
  EventManager &operator=(const EventManager &) = delete;

  // 接口观察者列表（按事件类型分组）
  std::unordered_map<std::string, std::vector<IEventObserver *>> _observers;

  // Lambda回调列表（按事件类型分组）
  struct CallbackEntry {
    int id;
    EventCallback callback;
  };
  std::unordered_map<std::string, std::vector<CallbackEntry>> _callbacks;

  // 下一个回调ID
  int _nextCallbackId;
};

#endif // __EVENT_MANAGER_H__

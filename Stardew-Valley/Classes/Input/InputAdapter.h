/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : InputAdapter.h
 * 文件功能      : 输入适配器，统一键盘和触摸输入接口
 * 作者          : 同学1
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with Adapter Pattern
 ****************************************************************/

#ifndef __INPUT_ADAPTER_H__
#define __INPUT_ADAPTER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

// ==================================================
// Refactored with Adapter Pattern
// 适配器模式：将不同的输入方式（键盘、触摸）适配到统一的接口
// ==================================================

/**
 * IInputHandler - 输入处理接口（Target）
 *
 * 设计模式：Adapter（适配器模式）
 *
 * 问题：Player类同时处理键盘和触摸两种输入，代码重复约400行
 * 解决：定义统一的输入接口，使用适配器将不同输入源转换为统一调用
 *
 * 角色说明：
 * - Target: IInputHandler接口
 * - Adapter: KeyboardInputAdapter, TouchInputAdapter
 * - Adaptee: EventListenerKeyboard, ui::Button
 * - Client: Player类
 */
class IInputHandler {
public:
  virtual ~IInputHandler() = default;

  /**
   * 移动开始事件
   * @param direction 方向 ("Up", "Down", "Left", "Right")
   */
  virtual void onMoveStart(const std::string &direction) = 0;

  /**
   * 移动停止事件
   * @param direction 方向
   */
  virtual void onMoveStop(const std::string &direction) = 0;

  /**
   * 动作事件（打开背包、地图等）
   * @param action 动作名称 ("Backpack", "Map")
   */
  virtual void onAction(const std::string &action) = 0;
};

/**
 * KeyboardInputAdapter - 键盘输入适配器
 *
 * 将Cocos2d的键盘事件适配到IInputHandler接口
 */
class KeyboardInputAdapter {
public:
  /**
   * 构造函数
   * @param handler 输入处理器（通常是Player对象）
   */
  explicit KeyboardInputAdapter(IInputHandler *handler);

  /**
   * 设置键盘监听器
   * @param node 要监听的节点
   */
  void setupListener(cocos2d::Node *node);

  /**
   * 移除键盘监听器
   */
  void removeListener();

private:
  IInputHandler *_handler;
  cocos2d::EventListenerKeyboard *_listener;

  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                    cocos2d::Event *event);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                     cocos2d::Event *event);

  // 辅助方法：按键转方向
  std::string keyCodeToDirection(cocos2d::EventKeyboard::KeyCode keyCode);
  // 辅助方法：按键转动作
  std::string keyCodeToAction(cocos2d::EventKeyboard::KeyCode keyCode);
};

/**
 * TouchInputAdapter - 触摸输入适配器
 *
 * 将触摸按钮事件适配到IInputHandler接口
 */
class TouchInputAdapter {
public:
  /**
   * 构造函数
   * @param handler 输入处理器
   */
  explicit TouchInputAdapter(IInputHandler *handler);

  /**
   * 创建触摸控制按钮
   * @param parent 父节点
   * @param centerX 方向键中心X坐标
   * @param centerY 方向键中心Y坐标
   * @param radius 方向键半径
   */
  void createButtons(cocos2d::Node *parent, float centerX, float centerY,
                     float radius);

private:
  IInputHandler *_handler;

  // 方向按钮
  cocos2d::ui::Button *_btnUp;
  cocos2d::ui::Button *_btnDown;
  cocos2d::ui::Button *_btnLeft;
  cocos2d::ui::Button *_btnRight;

  // 功能按钮
  cocos2d::ui::Button *_btnBackpack;
  cocos2d::ui::Button *_btnMap;

  // 辅助方法：设置方向按钮
  void setupDirectionButton(cocos2d::ui::Button *btn,
                            const std::string &direction);
  // 辅助方法：设置功能按钮
  void setupActionButton(cocos2d::ui::Button *btn, const std::string &action);
};

#endif // __INPUT_ADAPTER_H__

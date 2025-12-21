/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : PlayerState.h
 * 文件功能      : 玩家状态模式接口和具体状态类
 * 作者          : 同学2
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with State Pattern
 ****************************************************************/

#ifndef __PLAYER_STATE_H__
#define __PLAYER_STATE_H__

#include "cocos2d.h"
#include <string>

// 前向声明
class Player;

// ==================================================
// Refactored with State Pattern
// 状态模式：将玩家的不同移动状态封装为独立的状态类
// ==================================================

/**
 * IPlayerState - 玩家状态接口 (State)
 *
 * 设计模式：State（状态模式）
 *
 * 角色说明：
 * - State: IPlayerState 接口
 * - ConcreteState: IdleState, WalkingState
 * - Context: Player 类
 *
 * 问题：Player 类使用多个布尔标志管理移动状态，
 *      导致状态逻辑分散，难以扩展新状态
 *
 * 解决：使用状态模式将每种状态封装为独立类
 */
class IPlayerState {
public:
  virtual ~IPlayerState() = default;

  /**
   * 进入状态时调用
   * @param player 玩家对象指针
   */
  virtual void enter(Player *player) = 0;

  /**
   * 每帧更新
   * @param player 玩家对象指针
   * @param delta 帧间隔时间
   */
  virtual void update(Player *player, float delta) = 0;

  /**
   * 退出状态时调用
   * @param player 玩家对象指针
   */
  virtual void exit(Player *player) = 0;

  /**
   * 处理移动开始事件
   * @param player 玩家对象指针
   * @param direction 移动方向
   * @return 返回新状态（如果需要状态转换），否则返回nullptr
   */
  virtual IPlayerState *handleMoveStart(Player *player,
                                        const std::string &direction) = 0;

  /**
   * 处理移动停止事件
   * @param player 玩家对象指针
   * @param direction 停止的方向
   * @return 返回新状态（如果需要状态转换），否则返回nullptr
   */
  virtual IPlayerState *handleMoveStop(Player *player,
                                       const std::string &direction) = 0;

  /**
   * 获取状态名称（用于调试）
   */
  virtual std::string getStateName() const = 0;
};

/**
 * IdleState - 空闲状态（站立）
 * 玩家未移动时的状态
 */
class IdleState : public IPlayerState {
public:
  static IdleState *getInstance();

  void enter(Player *player) override;
  void update(Player *player, float delta) override;
  void exit(Player *player) override;

  IPlayerState *handleMoveStart(Player *player,
                                const std::string &direction) override;
  IPlayerState *handleMoveStop(Player *player,
                               const std::string &direction) override;

  std::string getStateName() const override { return "Idle"; }

private:
  IdleState() = default;
};

/**
 * WalkingState - 行走状态
 * 玩家正在移动时的状态
 */
class WalkingState : public IPlayerState {
public:
  static WalkingState *getInstance();

  void enter(Player *player) override;
  void update(Player *player, float delta) override;
  void exit(Player *player) override;

  IPlayerState *handleMoveStart(Player *player,
                                const std::string &direction) override;
  IPlayerState *handleMoveStop(Player *player,
                               const std::string &direction) override;

  std::string getStateName() const override { return "Walking"; }

private:
  WalkingState() = default;
};

/**
 * PlayerStateContext - 状态上下文辅助类
 * 管理玩家当前状态和状态转换
 */
class PlayerStateContext {
public:
  PlayerStateContext();
  ~PlayerStateContext();

  /**
   * 设置关联的玩家对象
   */
  void setPlayer(Player *player);

  /**
   * 切换到新状态
   */
  void changeState(IPlayerState *newState);

  /**
   * 获取当前状态
   */
  IPlayerState *getCurrentState() const;

  /**
   * 每帧更新
   */
  void update(float delta);

  /**
   * 处理移动开始事件
   */
  void handleMoveStart(const std::string &direction);

  /**
   * 处理移动停止事件
   */
  void handleMoveStop(const std::string &direction);

private:
  Player *_player;
  IPlayerState *_currentState;
};

#endif // __PLAYER_STATE_H__

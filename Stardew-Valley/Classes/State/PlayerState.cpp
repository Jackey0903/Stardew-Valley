/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : PlayerState.cpp
 * 文件功能      : 玩家状态模式实现
 * 作者          : 同学2
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with State Pattern
 ****************************************************************/

#include "PlayerState.h"
#include "Player/Player.h"

USING_NS_CC;

// ==================================================
// IdleState Implementation
// ==================================================

IdleState *IdleState::getInstance() {
  static IdleState instance;
  return &instance;
}

void IdleState::enter(Player *player) {
  CCLOG("[State] Entering IdleState");

  // 停止行走动画
  if (player) {
    player->stopWalkingAnimation();
  }
}

void IdleState::update(Player *player, float delta) {
  // 空闲状态不需要特殊更新逻辑
  // 玩家站立不动
}

void IdleState::exit(Player *player) { CCLOG("[State] Exiting IdleState"); }

IPlayerState *IdleState::handleMoveStart(Player *player,
                                         const std::string &direction) {
  // [ORIGINAL CODE - Before State Pattern]
  // 原始代码在 Player::onMoveStart 中直接设置布尔标志：
  // if (direction == "Left") _isMovingLeft = true;
  // ...
  // _isMoving = true;
  //
  // [REFACTORED CODE - With State Pattern]
  // 状态转换：从空闲状态转换到行走状态
  CCLOG("[State] IdleState: Move start -> %s", direction.c_str());
  return WalkingState::getInstance();
}

IPlayerState *IdleState::handleMoveStop(Player *player,
                                        const std::string &direction) {
  // 空闲状态下收到停止移动事件，保持空闲状态
  return nullptr;
}

// ==================================================
// WalkingState Implementation
// ==================================================

WalkingState *WalkingState::getInstance() {
  static WalkingState instance;
  return &instance;
}

void WalkingState::enter(Player *player) {
  CCLOG("[State] Entering WalkingState");

  // 开始行走动画
  // 动画会在 handleMoveStart 中根据方向启动
}

void WalkingState::update(Player *player, float delta) {
  // [ORIGINAL CODE - Before State Pattern]
  // 原始代码在 Player::update 中直接检查布尔标志：
  // if (_isMovingLeft) newLoc.x -= speed * delta;
  // if (_isMovingRight) newLoc.x += speed * delta;
  // ...
  //
  // [REFACTORED CODE - With State Pattern]
  // 行走状态的更新逻辑现在由 Player::update 调用
  // 具体移动计算仍在 Player 中，但状态管理由此类负责
}

void WalkingState::exit(Player *player) {
  CCLOG("[State] Exiting WalkingState");

  // 退出行走状态时停止动画
  if (player) {
    player->stopWalkingAnimation();
  }
}

IPlayerState *WalkingState::handleMoveStart(Player *player,
                                            const std::string &direction) {
  // 已经在行走状态，继续行走（可能改变方向）
  CCLOG("[State] WalkingState: Continue moving -> %s", direction.c_str());

  // 启动对应方向的动画
  if (player) {
    player->startWalkingAnimation(direction);
  }

  return nullptr; // 保持当前状态
}

IPlayerState *WalkingState::handleMoveStop(Player *player,
                                           const std::string &direction) {
  // [ORIGINAL CODE - Before State Pattern]
  // 原始代码在 Player::onMoveStop 中直接设置布尔标志：
  // if (direction == "Left") _isMovingLeft = false;
  // ...
  // 然后检查是否所有方向都停止了：
  // if (!_isMovingLeft && !_isMovingRight && !_isMovingUp && !_isMovingDown)
  //     _isMoving = false;
  //
  // [REFACTORED CODE - With State Pattern]
  // 需要检查是否还有其他方向在移动
  // 如果没有其他方向移动，转换到空闲状态
  CCLOG("[State] WalkingState: Move stop -> %s", direction.c_str());

  // 这里简化处理：收到停止事件时，检查玩家是否完全停止
  // 实际检查由 PlayerStateContext 或 Player 完成
  // 暂时返回 nullptr，由上层决定是否切换状态
  return nullptr;
}

// ==================================================
// PlayerStateContext Implementation
// ==================================================

PlayerStateContext::PlayerStateContext()
    : _player(nullptr), _currentState(IdleState::getInstance()) {}

PlayerStateContext::~PlayerStateContext() {
  // 状态使用单例，不需要删除
}

void PlayerStateContext::setPlayer(Player *player) { _player = player; }

void PlayerStateContext::changeState(IPlayerState *newState) {
  if (_currentState == newState || newState == nullptr) {
    return;
  }

  CCLOG("[StateContext] Changing state: %s -> %s",
        _currentState ? _currentState->getStateName().c_str() : "null",
        newState->getStateName().c_str());

  if (_currentState) {
    _currentState->exit(_player);
  }

  _currentState = newState;

  if (_currentState) {
    _currentState->enter(_player);
  }
}

IPlayerState *PlayerStateContext::getCurrentState() const {
  return _currentState;
}

void PlayerStateContext::update(float delta) {
  if (_currentState && _player) {
    _currentState->update(_player, delta);
  }
}

void PlayerStateContext::handleMoveStart(const std::string &direction) {
  if (_currentState && _player) {
    IPlayerState *newState = _currentState->handleMoveStart(_player, direction);
    if (newState) {
      changeState(newState);
      // 状态切换后，再次调用处理以启动动画
      _currentState->handleMoveStart(_player, direction);
    }
  }
}

void PlayerStateContext::handleMoveStop(const std::string &direction) {
  if (_currentState && _player) {
    IPlayerState *newState = _currentState->handleMoveStop(_player, direction);
    if (newState) {
      changeState(newState);
    }
  }
}

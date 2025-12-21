/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : GameManager.cpp
 * 文件功能      : 游戏管理器单例类实现
 * 作者          : 同学3
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with Singleton Pattern
 ****************************************************************/

#include "GameManager.h"
#include "cocos2d.h"

USING_NS_CC;

// ==================================================
// Singleton Pattern Implementation
// ==================================================

GameManager *GameManager::getInstance() {
  // C++11保证局部静态变量的线程安全初始化
  static GameManager instance;
  return &instance;
}

GameManager::GameManager()
    : _currentMap(""), _playerSpeed(DEFAULT_PLAYER_SPEED) {
  CCLOG("[GameManager] Singleton instance created");
  CCLOG("[GameManager] Default speed: %.2f", _playerSpeed);
}

// ==================== 地图管理 ====================

void GameManager::setCurrentMap(const std::string &mapPath) {
  // [ORIGINAL CODE - Before Singleton Pattern]
  // g_selectedMap = mapPath;
  //
  // [REFACTORED CODE - With Singleton Pattern]
  _currentMap = mapPath;
  CCLOG("[GameManager] Map set to: %s", _currentMap.c_str());
}

std::string GameManager::getCurrentMap() const {
  // [ORIGINAL CODE - Before Singleton Pattern]
  // return g_selectedMap;
  //
  // [REFACTORED CODE - With Singleton Pattern]
  return _currentMap;
}

void GameManager::clearCurrentMap() {
  // [ORIGINAL CODE - Before Singleton Pattern]
  // g_selectedMap.clear();
  //
  // [REFACTORED CODE - With Singleton Pattern]
  _currentMap.clear();
  CCLOG("[GameManager] Map cleared");
}

bool GameManager::hasMapSet() const { return !_currentMap.empty(); }

// ==================== 玩家设置 ====================

void GameManager::setPlayerSpeed(float speed) {
  // [ORIGINAL CODE - Before Singleton Pattern]
  // ::speed = speed;  // 全局变量
  //
  // [REFACTORED CODE - With Singleton Pattern]
  // 添加范围检查
  if (speed < 100.0f) {
    speed = 100.0f;
  } else if (speed > 300.0f) {
    speed = 300.0f;
  }
  _playerSpeed = speed;
  CCLOG("[GameManager] Player speed set to: %.2f", _playerSpeed);
}

float GameManager::getPlayerSpeed() const {
  // [ORIGINAL CODE - Before Singleton Pattern]
  // return ::speed;  // 全局变量
  //
  // [REFACTORED CODE - With Singleton Pattern]
  return _playerSpeed;
}

// ==================== 游戏状态 ====================

void GameManager::resetToDefaults() {
  _currentMap.clear();
  _playerSpeed = DEFAULT_PLAYER_SPEED;
  CCLOG("[GameManager] Reset to defaults");
}

// ==================== 调试支持 ====================

void GameManager::logCurrentState() const {
  CCLOG("========== GameManager State ==========");
  CCLOG("  Current Map: %s",
        _currentMap.empty() ? "(none)" : _currentMap.c_str());
  CCLOG("  Player Speed: %.2f", _playerSpeed);
  CCLOG("========================================");
}

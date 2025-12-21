/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : GameManager.h
 * 文件功能      : 游戏管理器单例类，集中管理游戏全局状态
 * 作者          : 同学3
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with Singleton Pattern
 ****************************************************************/

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <string>

// ==================================================
// Refactored with Singleton Pattern
// 单例模式：确保游戏管理器只有一个实例，提供全局访问点
// ==================================================

/**
 * GameManager - 游戏管理器单例类
 *
 * 设计模式：Singleton（单例模式）
 *
 * 问题：全局变量分散在 GlobalVars 中，难以管理和追踪
 * 解决：使用单例模式，将所有全局状态集中管理
 *
 * 角色说明：
 * - Singleton: GameManager 类本身
 *
 * 重构说明：
 * - 原始代码使用 extern 全局变量 (g_selectedMap, speed)
 * - 重构后使用 GameManager::getInstance()->getXxx() 访问
 */
class GameManager {
public:
  /**
   * 获取单例实例
   * 使用C++11线程安全的局部静态变量实现
   * @return GameManager单例指针
   */
  static GameManager *getInstance();

  // ==================== 地图管理 ====================

  /**
   * 设置当前地图
   * @param mapPath 地图文件路径，如 "Map/Map2/map2.tmx"
   */
  void setCurrentMap(const std::string &mapPath);

  /**
   * 获取当前地图路径
   * @return 当前地图路径，如果未设置则返回空字符串
   */
  std::string getCurrentMap() const;

  /**
   * 清除当前地图设置
   */
  void clearCurrentMap();

  /**
   * 检查是否有地图设置
   */
  bool hasMapSet() const;

  // ==================== 玩家设置 ====================

  /**
   * 设置玩家移动速度
   * @param speed 移动速度 (范围: 100-300)
   */
  void setPlayerSpeed(float speed);

  /**
   * 获取玩家移动速度
   * @return 当前移动速度
   */
  float getPlayerSpeed() const;

  // ==================== 游戏状态 ====================

  /**
   * 重置所有设置为默认值
   */
  void resetToDefaults();

  // ==================== 调试支持 ====================

  /**
   * 打印当前状态（用于调试）
   */
  void logCurrentState() const;

private:
  // 私有构造函数（单例模式）
  GameManager();

  // 禁用拷贝和赋值
  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;

  // ==================== 成员变量 ====================

  // [ORIGINAL CODE - Before Singleton Pattern]
  // 原始代码在 GlobalVars.h 中使用全局变量：
  // extern std::string g_selectedMap;
  // extern float speed;
  //
  // [REFACTORED CODE - With Singleton Pattern]
  // 重构后将这些状态封装在单例类中：

  std::string _currentMap; // 替代 g_selectedMap
  float _playerSpeed;      // 替代 speed

  // 默认值常量
  static constexpr float DEFAULT_PLAYER_SPEED = 200.0f;
};

#endif // __GAME_MANAGER_H__

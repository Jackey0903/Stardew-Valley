/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : AnimationFlyweight.h
 * 文件功能      : 动画享元工厂，共享动画帧资源，避免重复加载
 * 作者          : 同学1
 * 更新日期      : 2025/12/16
 * 许可证        : MIT License
 * 重构说明      : Refactored with Flyweight Pattern
 ****************************************************************/

#ifndef __ANIMATION_FLYWEIGHT_H__
#define __ANIMATION_FLYWEIGHT_H__

#include "cocos2d.h"
#include <string>
#include <unordered_map>

// ==================================================
// Refactored with Flyweight Pattern
// 享元模式：共享动画资源，减少内存占用
// ==================================================

/**
 * AnimationFlyweight - 动画享元工厂类
 *
 * 设计模式：Flyweight（享元模式）
 *
 * 问题：Player和Animal类各自加载相同的动画帧资源，造成内存浪费
 * 解决：使用享元模式，将动画资源集中管理，多个对象共享同一份动画数据
 *
 * 角色说明：
 * - FlyweightFactory: AnimationFlyweight类本身
 * - Flyweight: Animation对象（共享的内在状态）
 * - Client: Player, Animal等使用动画的类
 */
class AnimationFlyweight {
public:
  /**
   * 获取单例实例
   * @return AnimationFlyweight单例指针
   */
  static AnimationFlyweight *getInstance();

  /**
   * 获取共享动画对象（享元）
   * @param key 动画的唯一标识符，如 "Player_Walk_Left"
   * @return 共享的Animation对象，如果不存在则返回nullptr
   */
  cocos2d::Animation *getAnimation(const std::string &key);

  /**
   * 检查动画是否已加载
   * @param key 动画标识符
   * @return 是否存在
   */
  bool hasAnimation(const std::string &key) const;

  /**
   * 预加载玩家动画资源
   * 加载四个方向的行走动画并存入享元池
   */
  void preloadPlayerAnimations();

  /**
   * 预加载动物动画资源
   * @param animalName 动物名称，如 "chicken", "cow"
   */
  void preloadAnimalAnimations(const std::string &animalName);

  /**
   * 清理所有缓存的动画资源
   */
  void clearAllAnimations();

  /**
   * 获取当前缓存的动画数量
   */
  size_t getCachedAnimationCount() const { return _animationPool.size(); }

private:
  // 私有构造函数（单例模式）
  AnimationFlyweight() = default;
  ~AnimationFlyweight();

  // 禁用拷贝
  AnimationFlyweight(const AnimationFlyweight &) = delete;
  AnimationFlyweight &operator=(const AnimationFlyweight &) = delete;

  // 单例实例
  static AnimationFlyweight *_instance;

  // 享元池：存储共享的动画对象
  // key: 动画标识符 (如 "Player_Walk_Left")
  // value: Animation对象指针
  std::unordered_map<std::string, cocos2d::Animation *> _animationPool;

  /**
   * 创建行走动画
   * @param prefix 帧名前缀
   * @param frameCount 帧数量
   * @param interval 帧间隔时间
   * @return 创建的Animation对象
   */
  cocos2d::Animation *createWalkAnimation(const std::string &prefix,
                                          int frameCount,
                                          float interval = 0.1f);
};

#endif // __ANIMATION_FLYWEIGHT_H__

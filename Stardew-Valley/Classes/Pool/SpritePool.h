/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : SpritePool.h
 * 文件功能      : 精灵对象池，复用Sprite对象减少内存分配
 * 作者          : 同学3
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with Object Pool Pattern
 ****************************************************************/

#ifndef __SPRITE_POOL_H__
#define __SPRITE_POOL_H__

#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include <vector>

// ==================================================
// Refactored with Object Pool Pattern
// 对象池模式：复用对象，减少频繁创建/销毁的开销
// ==================================================

/**
 * SpritePool - 精灵对象池类
 *
 * 设计模式：Object Pool（对象池模式）
 *
 * 问题：Map2Scene 中频繁创建/销毁 Sprite 对象（作物、树木），
 *      导致内存分配开销大，可能造成内存碎片化
 *
 * 解决：使用对象池预创建和复用 Sprite 对象
 *
 * 角色说明：
 * - PooledObject: cocos2d::Sprite 对象
 * - ObjectPool: SpritePool 类本身
 * - Client: Map2Scene 等需要频繁创建精灵的类
 *
 * 使用方式：
 *   // 获取精灵（从池中获取或新建）
 *   auto sprite = SpritePool::getInstance()->acquire("Plants/Carrot_1.png");
 *
 *   // 使用完毕后归还到池中
 *   SpritePool::getInstance()->release(sprite);
 */
class SpritePool {
public:
  /**
   * 获取单例实例
   * @return SpritePool单例指针
   */
  static SpritePool *getInstance();

  /**
   * 从池中获取一个精灵对象
   * 如果池中有可用对象则复用，否则创建新对象
   * @param imagePath 精灵图片路径
   * @return Sprite对象指针
   */
  cocos2d::Sprite *acquire(const std::string &imagePath);

  /**
   * 将精灵对象归还到池中
   * 精灵会被重置状态并隐藏，等待下次复用
   * @param sprite 要归还的精灵对象
   * @param imagePath 精灵的图片路径（用于分类存储）
   */
  void release(cocos2d::Sprite *sprite, const std::string &imagePath);

  /**
   * 预加载指定数量的精灵到池中
   * @param imagePath 图片路径
   * @param count 预创建数量
   */
  void preload(const std::string &imagePath, int count);

  /**
   * 清空指定类型的对象池
   * @param imagePath 图片路径，如果为空则清空所有
   */
  void clear(const std::string &imagePath = "");

  /**
   * 获取池中可用对象数量
   * @param imagePath 图片路径，如果为空则返回总数
   */
  size_t getAvailableCount(const std::string &imagePath = "") const;

  /**
   * 获取当前活跃（被借出）的对象数量
   */
  size_t getActiveCount() const;

  /**
   * 打印池状态（调试用）
   */
  void logPoolStatus() const;

private:
  // 私有构造函数（结合单例模式）
  SpritePool();
  ~SpritePool();

  // 禁用拷贝
  SpritePool(const SpritePool &) = delete;
  SpritePool &operator=(const SpritePool &) = delete;

  /**
   * 重置精灵状态
   * 归还前调用，确保复用时状态干净
   */
  void resetSprite(cocos2d::Sprite *sprite);

  // ==================== 成员变量 ====================

  // 按图片路径分组的空闲对象池
  // key: 图片路径, value: 可用的Sprite列表
  std::unordered_map<std::string, std::vector<cocos2d::Sprite *>> _pool;

  // 活跃对象计数
  size_t _activeCount;
};

#endif // __SPRITE_POOL_H__

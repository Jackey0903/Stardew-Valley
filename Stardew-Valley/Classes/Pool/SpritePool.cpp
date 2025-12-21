/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : SpritePool.cpp
 * 文件功能      : 精灵对象池实现
 * 作者          : 同学3
 * 更新日期      : 2025/12/21
 * 许可证        : MIT License
 * 重构说明      : Refactored with Object Pool Pattern
 ****************************************************************/

#include "SpritePool.h"

USING_NS_CC;

// ==================================================
// Object Pool Pattern Implementation
// ==================================================

SpritePool *SpritePool::getInstance() {
  static SpritePool instance;
  return &instance;
}

SpritePool::SpritePool() : _activeCount(0) {
  CCLOG("[SpritePool] Object Pool instance created");
}

SpritePool::~SpritePool() {
  clear();
  CCLOG("[SpritePool] Object Pool destroyed");
}

cocos2d::Sprite *SpritePool::acquire(const std::string &imagePath) {
  Sprite *sprite = nullptr;

  // 检查池中是否有可用对象
  auto it = _pool.find(imagePath);
  if (it != _pool.end() && !it->second.empty()) {
    // [REFACTORED CODE - With Object Pool Pattern]
    // 从池中取出一个已有的对象
    sprite = it->second.back();
    it->second.pop_back();

    // 重新设置纹理（确保正确）
    sprite->setTexture(imagePath);
    sprite->setVisible(true);

    CCLOG("[SpritePool] Reused sprite from pool: %s (pool size: %zu)",
          imagePath.c_str(), it->second.size());
  } else {
    // [ORIGINAL CODE - Before Object Pool Pattern]
    // auto sprite = Sprite::create(imagePath);
    //
    // [REFACTORED CODE - With Object Pool Pattern]
    // 池中没有可用对象，创建新对象
    sprite = Sprite::create(imagePath);
    if (sprite) {
      sprite->retain(); // 保持引用，防止被自动释放
      CCLOG("[SpritePool] Created new sprite: %s", imagePath.c_str());
    }
  }

  if (sprite) {
    _activeCount++;
  }

  return sprite;
}

void SpritePool::release(cocos2d::Sprite *sprite,
                         const std::string &imagePath) {
  if (!sprite) {
    return;
  }

  // [ORIGINAL CODE - Before Object Pool Pattern]
  // sprite->removeFromParent();  // 直接销毁
  //
  // [REFACTORED CODE - With Object Pool Pattern]
  // 重置精灵状态
  resetSprite(sprite);

  // 从父节点移除但不销毁
  if (sprite->getParent()) {
    sprite->removeFromParent();
  }

  // 放回池中
  _pool[imagePath].push_back(sprite);
  _activeCount--;

  CCLOG("[SpritePool] Released sprite to pool: %s (pool size: %zu)",
        imagePath.c_str(), _pool[imagePath].size());
}

void SpritePool::preload(const std::string &imagePath, int count) {
  CCLOG("[SpritePool] Preloading %d sprites: %s", count, imagePath.c_str());

  for (int i = 0; i < count; i++) {
    Sprite *sprite = Sprite::create(imagePath);
    if (sprite) {
      sprite->retain();
      sprite->setVisible(false);
      _pool[imagePath].push_back(sprite);
    }
  }

  CCLOG("[SpritePool] Preloaded %zu sprites for: %s", _pool[imagePath].size(),
        imagePath.c_str());
}

void SpritePool::clear(const std::string &imagePath) {
  if (imagePath.empty()) {
    // 清空所有
    for (auto &pair : _pool) {
      for (auto sprite : pair.second) {
        sprite->release();
      }
      pair.second.clear();
    }
    _pool.clear();
    CCLOG("[SpritePool] Cleared all pools");
  } else {
    // 清空指定类型
    auto it = _pool.find(imagePath);
    if (it != _pool.end()) {
      for (auto sprite : it->second) {
        sprite->release();
      }
      it->second.clear();
      CCLOG("[SpritePool] Cleared pool: %s", imagePath.c_str());
    }
  }
}

size_t SpritePool::getAvailableCount(const std::string &imagePath) const {
  if (imagePath.empty()) {
    size_t total = 0;
    for (const auto &pair : _pool) {
      total += pair.second.size();
    }
    return total;
  } else {
    auto it = _pool.find(imagePath);
    return (it != _pool.end()) ? it->second.size() : 0;
  }
}

size_t SpritePool::getActiveCount() const { return _activeCount; }

void SpritePool::resetSprite(cocos2d::Sprite *sprite) {
  if (!sprite)
    return;

  // 重置常见属性
  sprite->setPosition(Vec2::ZERO);
  sprite->setScale(1.0f);
  sprite->setRotation(0.0f);
  sprite->setOpacity(255);
  sprite->setVisible(false);
  sprite->stopAllActions();

  // 移除所有子节点
  sprite->removeAllChildren();
}

void SpritePool::logPoolStatus() const {
  CCLOG("========== SpritePool Status ==========");
  CCLOG("  Active sprites: %zu", _activeCount);

  size_t totalAvailable = 0;
  for (const auto &pair : _pool) {
    CCLOG("  [%s]: %zu available", pair.first.c_str(), pair.second.size());
    totalAvailable += pair.second.size();
  }

  CCLOG("  Total available: %zu", totalAvailable);
  CCLOG("========================================");
}

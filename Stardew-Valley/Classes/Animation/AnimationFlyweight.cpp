/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : AnimationFlyweight.cpp
 * 文件功能      : 动画享元工厂实现
 * 作者          : 同学1
 * 更新日期      : 2025/12/16
 * 许可证        : MIT License
 * 重构说明      : Refactored with Flyweight Pattern
 ****************************************************************/

#include "AnimationFlyweight.h"

USING_NS_CC;

// ==================================================
// Refactored with Flyweight Pattern
// ==================================================

// 单例实例初始化
AnimationFlyweight *AnimationFlyweight::_instance = nullptr;

AnimationFlyweight::~AnimationFlyweight() { clearAllAnimations(); }

AnimationFlyweight *AnimationFlyweight::getInstance() {
  if (!_instance) {
    _instance = new AnimationFlyweight();
  }
  return _instance;
}

cocos2d::Animation *AnimationFlyweight::getAnimation(const std::string &key) {
  auto it = _animationPool.find(key);
  if (it != _animationPool.end()) {
    return it->second;
  }
  CCLOG("[AnimationFlyweight] Animation not found: %s", key.c_str());
  return nullptr;
}

bool AnimationFlyweight::hasAnimation(const std::string &key) const {
  return _animationPool.find(key) != _animationPool.end();
}

void AnimationFlyweight::preloadPlayerAnimations() {
  CCLOG("[AnimationFlyweight] Preloading player animations...");

  // 加载精灵帧缓存（plist文件）
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
      "Action/Walk_Left.plist");
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
      "Action/Walk_Right.plist");
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
      "Action/Walk_Up.plist");
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
      "Action/Walk_Down.plist");

  // 创建并缓存四个方向的行走动画（享元对象）
  const std::vector<std::string> directions = {"Left", "Right", "Up", "Down"};

  for (const auto &dir : directions) {
    std::string key = "Player_Walk_" + dir;
    std::string framePrefix = "Action/Walk_" + dir + "_";

    if (!hasAnimation(key)) {
      auto anim = createWalkAnimation(framePrefix, 2, 0.1f);
      if (anim) {
        _animationPool[key] = anim;
        CCLOG("[AnimationFlyweight] Cached animation: %s", key.c_str());
      }
    }
  }

  CCLOG("[AnimationFlyweight] Player animations loaded. Total cached: %zu",
        _animationPool.size());
}

void AnimationFlyweight::preloadAnimalAnimations(
    const std::string &animalName) {
  CCLOG("[AnimationFlyweight] Preloading animal animations for: %s",
        animalName.c_str());

  std::string basePath = "Animal/" + animalName + "/" + animalName;

  // 加载精灵帧缓存
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(basePath +
                                                           "_Down.plist");
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(basePath +
                                                           "_Up.plist");
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(basePath +
                                                           "_Left.plist");
  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(basePath +
                                                           "_Right.plist");

  // 创建并缓存四个方向的行走动画
  const std::vector<std::string> directions = {"Down", "Up", "Left", "Right"};

  for (const auto &dir : directions) {
    std::string key = animalName + "_Walk_" + dir;
    std::string framePrefix = animalName + "_" + dir + "_";

    if (!hasAnimation(key)) {
      auto anim = createWalkAnimation(framePrefix, 2, 0.2f);
      if (anim) {
        _animationPool[key] = anim;
        CCLOG("[AnimationFlyweight] Cached animation: %s", key.c_str());
      }
    }
  }
}

void AnimationFlyweight::clearAllAnimations() {
  for (auto &pair : _animationPool) {
    if (pair.second) {
      pair.second->release();
    }
  }
  _animationPool.clear();
  CCLOG("[AnimationFlyweight] All animations cleared.");
}

cocos2d::Animation *
AnimationFlyweight::createWalkAnimation(const std::string &prefix,
                                        int frameCount, float interval) {
  Vector<SpriteFrame *> frames;

  for (int i = 1; i <= frameCount; i++) {
    std::string frameName = prefix + std::to_string(i) + ".png";
    auto frame =
        SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
    if (frame) {
      frames.pushBack(frame);
    } else {
      CCLOG("[AnimationFlyweight] Frame not found: %s", frameName.c_str());
    }
  }

  if (frames.empty()) {
    CCLOG("[AnimationFlyweight] No frames loaded for prefix: %s",
          prefix.c_str());
    return nullptr;
  }

  auto animation = Animation::createWithSpriteFrames(frames, interval);
  animation->retain(); // 保持引用，防止被自动释放

  return animation;
}

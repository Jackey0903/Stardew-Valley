# 星露谷物语 - 设计模式重构报告

## 同学3：创建型设计模式重构

**作者**: 同学3  
**日期**: 2024年12月21日  
**负责模式**: Singleton（单例模式）、Object Pool（对象池模式）

---

# 目录

1. [项目简介与重构动机](#一项目简介与重构动机)
2. [设计模式应用](#二设计模式应用)
   - 2.1 [Singleton 单例模式](#21-singleton-单例模式)
   - 2.2 [Object Pool 对象池模式](#22-object-pool-对象池模式)
3. [完整系统设计](#三完整系统设计)
4. [其他重要问题与解决方案](#四其他重要问题与解决方案)
5. [文件变更清单](#五文件变更清单)
6. [总结](#六总结)

---

# 一、项目简介与重构动机

## 1.1 项目背景

**星露谷物语（Stardew Valley）** 是基于 Cocos2d-x 游戏引擎开发的2D像素风格农场模拟游戏。项目采用 C++14 标准开发，实现了玩家控制、动物系统、地图交互、背包管理等核心功能。

### 技术栈

| 组件 | 技术 |
|-----|------|
| 游戏引擎 | Cocos2d-x 4.0 |
| 编程语言 | C++14 |
| 构建系统 | CMake 3.6+ |
| 平台支持 | Windows, macOS, iOS, Android |

## 1.2 重构动机

在对现有代码进行审查后，发现以下主要问题：

### 问题1：全局变量分散，难以管理

```cpp
// GlobalVars.h - 使用 extern 全局变量
extern std::string g_selectedMap;
extern float speed;

// Player.cpp - 直接访问全局变量
extern std::string g_selectedMap;
extern float speed;

if (g_selectedMap == "Map/Map2/map2.tmx") { ... }
newLoc.x -= speed * delta;
```

- **影响**: 全局状态分散在多个文件中，难以追踪和管理
- **量化**: 4个文件使用 extern 声明，依赖关系不明确

### 问题2：频繁创建销毁对象，性能开销大

```cpp
// Map2Scene.cpp - 每次种植都创建新对象
void Map2Scene::plantCropAt(const Vec2& locationInMap) {
    auto crop = Sprite::create(cropImages[cropIndex]); // 每次 new
    _crops.push_back(crop);
}

void Map2Scene::harvestCropAt(const Vec2& locationInMap) {
    crop->removeFromParent(); // 每次 delete
    _crops.erase(it);
}
```

- **影响**: 频繁的内存分配/释放导致性能开销
- **量化**: 种植/收获操作每次都涉及对象创建/销毁

### 问题3：违反 SOLID 设计原则

| 违反的原则 | 具体表现 |
|-----------|---------|
| 单一职责原则 (SRP) | GlobalVars 仅存储变量，无法控制访问 |
| 开放封闭原则 (OCP) | 添加新全局状态需修改多处 extern 声明 |
| 依赖倒置原则 (DIP) | 直接依赖具体的全局变量 |

## 1.3 重构目标

1. **应用 Singleton 模式**: 集中管理游戏全局状态
2. **应用 Object Pool 模式**: 复用 Sprite 对象，减少内存分配
3. **提升代码质量**: 遵循 SOLID 原则，提高可维护性

---

# 二、设计模式应用

## 2.1 Singleton 单例模式

### 2.1.1 模式概述

**单例模式（Singleton Pattern）** 是一种创建型设计模式，确保一个类只有一个实例，并提供一个全局访问点。

#### 模式结构

| 角色 | 职责 |
|-----|------|
| **Singleton** | 定义一个静态方法 getInstance() 返回唯一实例 |
| **Client** | 通过 getInstance() 访问单例对象 |

#### 适用场景

- 需要全局唯一的对象（如配置管理器、日志系统）
- 需要控制对共享资源的访问
- 需要延迟初始化的全局对象

### 2.1.2 问题分析

#### 原始代码问题

**GlobalVars.h**:
```cpp
// 问题：使用 extern 全局变量
extern std::string g_selectedMap;
extern float speed;
```

**Player.cpp 第26-27行**:
```cpp
extern std::string g_selectedMap;
extern float speed;
```

**GameScene.cpp 第9行**:
```cpp
extern std::string g_selectedMap;
```

**BackpackScene.cpp 第15行**:
```cpp
extern float speed;
```

#### 代码异味分析

| 异味类型 | 位置 | 描述 |
|---------|------|------|
| **Global Data** | GlobalVars.h | 全局变量分散 |
| **Shotgun Surgery** | 多文件 | 修改需更新多处 extern |
| **Hidden Dependencies** | Player.cpp 等 | 隐式依赖全局变量 |

### 2.1.3 UML类图对比

#### 重构前类图

![重构前类图 - Singleton](UML/Singleton_Before_Class.png)

**问题说明**:
1. 全局变量定义在 GlobalVars.h
2. 多个类直接使用 extern 访问
3. 依赖关系不明确

#### 重构后类图

![重构后类图 - Singleton](UML/Singleton_After_Class.png)

**改进说明**:
1. GameManager 单例类集中管理所有全局状态
2. 客户端通过 getInstance() 访问
3. 提供明确的 getter/setter 方法

### 2.1.4 代码对比

#### 头文件声明

**重构前 (GlobalVars.h):**
```cpp
#ifndef __GLOBAL_VARS_H__
#define __GLOBAL_VARS_H__

#include <string>

extern std::string g_selectedMap;
extern float speed;

#endif
```

**重构后 (GameManager.h):**
```cpp
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <string>

class GameManager {
public:
  static GameManager *getInstance();

  // 地图管理
  void setCurrentMap(const std::string &mapPath);
  std::string getCurrentMap() const;
  void clearCurrentMap();
  bool hasMapSet() const;

  // 玩家设置
  void setPlayerSpeed(float speed);
  float getPlayerSpeed() const;

  // 游戏状态
  void resetToDefaults();
  void logCurrentState() const;

private:
  GameManager();
  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;

  std::string _currentMap;
  float _playerSpeed;
  static constexpr float DEFAULT_PLAYER_SPEED = 200.0f;
};

#endif
```

#### Player.cpp 使用方式

**重构前:**
```cpp
extern std::string g_selectedMap;
extern float speed;

bool Player::init() {
    if (g_selectedMap == "" || g_selectedMap == "Map/Map3/map3.tmx") {
        _playerSprite->setPosition(...);
    }
}

void Player::updatePosition(float delta) {
    if (_isMovingLeft)
        newLoc.x -= speed * delta;
}
```

**重构后:**
```cpp
#include "Manager/GameManager.h"

bool Player::init() {
    std::string currentMap = GameManager::getInstance()->getCurrentMap();
    if (currentMap == "" || currentMap == "Map/Map3/map3.tmx") {
        _playerSprite->setPosition(...);
    }
}

void Player::updatePosition(float delta) {
    float playerSpeed = GameManager::getInstance()->getPlayerSpeed();
    if (_isMovingLeft)
        newLoc.x -= playerSpeed * delta;
}
```

#### BackpackScene.cpp 速度调节

**重构前:**
```cpp
extern float speed;

void BackpackScene::onSpeedSliderChanged(Ref* sender, ui::Slider::EventType type) {
    float percent = _speedSlider->getPercent();
    speed = 100 + (percent / 100.0f) * 200;
    CCLOG("移动速度: %.2f", speed);
}
```

**重构后:**
```cpp
#include "Manager/GameManager.h"

void BackpackScene::onSpeedSliderChanged(Ref* sender, ui::Slider::EventType type) {
    float percent = _speedSlider->getPercent();
    float newSpeed = 100 + (percent / 100.0f) * 200;
    GameManager::getInstance()->setPlayerSpeed(newSpeed);
    CCLOG("移动速度: %.2f", newSpeed);
}
```

### 2.1.5 新增类完整定义

#### GameManager.h

```cpp
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <string>

/**
 * GameManager - 游戏管理器单例类
 * 设计模式：Singleton（单例模式）
 */
class GameManager {
public:
  static GameManager *getInstance();

  // 地图管理
  void setCurrentMap(const std::string &mapPath);
  std::string getCurrentMap() const;
  void clearCurrentMap();
  bool hasMapSet() const;

  // 玩家设置
  void setPlayerSpeed(float speed);
  float getPlayerSpeed() const;

  // 游戏状态
  void resetToDefaults();
  void logCurrentState() const;

private:
  GameManager();
  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;

  std::string _currentMap;
  float _playerSpeed;
  static constexpr float DEFAULT_PLAYER_SPEED = 200.0f;
};

#endif
```

### 2.1.6 理论依据与必要性

#### 设计原则遵循

| 原则 | 重构前 | 重构后 | 改进 |
|-----|-------|-------|------|
| **单一职责** | 变量分散存储 | GameManager集中管理 | ✅ 职责集中 |
| **开放封闭** | 添加变量需修改多处 | 只需修改GameManager | ✅ 扩展开放 |
| **依赖倒置** | 依赖具体全局变量 | 依赖GameManager接口 | ✅ 降低耦合 |

#### 单例模式必要性

1. **集中管理**: 所有全局状态集中在一个类中
2. **访问控制**: 通过 getter/setter 控制访问和验证
3. **线程安全**: C++11 静态局部变量保证线程安全初始化
4. **可测试性**: 便于通过 Mock 进行单元测试

### 2.1.7 实际效益分析

| 指标 | 重构前 | 重构后 | 改进幅度 |
|-----|-------|-------|---------|
| extern 声明数量 | 8处 | 0处 | **100%消除** |
| 依赖关系可见性 | 隐式 | 显式 | **清晰依赖** |
| 添加新状态工作量 | 修改4+文件 | 修改1个类 | **75%减少** |
| 值范围验证 | 无 | 有 | **增加安全性** |

---

## 2.2 Object Pool 对象池模式

### 2.2.1 模式概述

**对象池模式（Object Pool Pattern）** 是一种创建型设计模式，通过预创建和复用对象来减少频繁创建/销毁对象的开销。

#### 模式结构

| 角色 | 职责 |
|-----|------|
| **ObjectPool** | 管理对象的创建、借出和归还 |
| **PooledObject** | 被池化的对象 |
| **Client** | 从池中获取和归还对象 |

#### 适用场景

- 对象创建代价大
- 对象生命周期短但使用频繁
- 需要限制同时存在的对象数量

### 2.2.2 问题分析

#### 原始代码问题

**Map2Scene.cpp plantCropAt() 第360-409行**:
```cpp
void Map2Scene::plantCropAt(const Vec2& locationInMap) {
    // 问题：每次种植都创建新的Sprite对象
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);
    _tiledMap->addChild(crop, 15);
    _crops.push_back(crop);
}
```

**Map2Scene.cpp harvestCropAt() 第411-457行**:
```cpp
void Map2Scene::harvestCropAt(const Vec2& locationInMap) {
    for (auto it = _crops.begin(); it != _crops.end(); ++it) {
        auto crop = *it;
        if (boundingBox.containsPoint(locationInMap)) {
            // 问题：直接销毁对象
            crop->removeFromParent();
            _crops.erase(it);
            break;
        }
    }
}
```

#### 代码异味分析

| 异味类型 | 位置 | 描述 | 严重程度 |
|---------|------|------|---------|
| **Eager Creation** | plantCropAt | 每次创建新对象 | 🟡 中 |
| **Eager Destruction** | harvestCropAt | 每次销毁对象 | 🟡 中 |
| **Memory Churn** | 多处 | 频繁分配/释放内存 | 🔴 高 |

### 2.2.3 UML类图对比

#### 重构前类图

![重构前类图 - Object Pool](UML/ObjectPool_Before_Class.png)

**问题说明**:
1. Map2Scene 直接调用 Sprite::create()
2. 收获时直接调用 removeFromParent()
3. 无对象复用机制

#### 重构后类图

![重构后类图 - Object Pool](UML/ObjectPool_After_Class.png)

**改进说明**:
1. SpritePool 管理所有 Sprite 对象
2. 按图片路径分组池化
3. 提供 acquire/release API

### 2.2.4 代码对比

#### 种植作物

**重构前:**
```cpp
void Map2Scene::plantCropAt(const Vec2& locationInMap) {
    // 每次创建新对象
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);
    _tiledMap->addChild(crop, 15);
    _crops.push_back(crop);
}
```

**重构后:**
```cpp
void Map2Scene::plantCropAt(const Vec2& locationInMap) {
    // 从对象池获取（复用或新建）
    auto crop = SpritePool::getInstance()->acquire(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);
    crop->setVisible(true);
    _tiledMap->addChild(crop, 15);
    _crops.push_back(crop);
}
```

#### 收获作物

**重构前:**
```cpp
void Map2Scene::harvestCropAt(const Vec2& locationInMap) {
    for (auto it = _crops.begin(); it != _crops.end(); ++it) {
        auto crop = *it;
        if (boundingBox.containsPoint(locationInMap)) {
            // 直接销毁对象
            crop->removeFromParent();
            _crops.erase(it);
            break;
        }
    }
}
```

**重构后:**
```cpp
void Map2Scene::harvestCropAt(const Vec2& locationInMap) {
    for (auto it = _crops.begin(); it != _crops.end(); ++it) {
        auto crop = *it;
        if (boundingBox.containsPoint(locationInMap)) {
            // 归还到对象池（复用而非销毁）
            SpritePool::getInstance()->release(crop, cropImages[cropIndex]);
            _crops.erase(it);
            break;
        }
    }
}
```

### 2.2.5 新增类完整定义

#### SpritePool.h

```cpp
#ifndef __SPRITE_POOL_H__
#define __SPRITE_POOL_H__

#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include <vector>

/**
 * SpritePool - 精灵对象池类
 * 设计模式：Object Pool（对象池模式）
 */
class SpritePool {
public:
  static SpritePool *getInstance();

  // 从池中获取一个精灵对象
  cocos2d::Sprite *acquire(const std::string &imagePath);

  // 将精灵对象归还到池中
  void release(cocos2d::Sprite *sprite, const std::string &imagePath);

  // 预加载指定数量的精灵到池中
  void preload(const std::string &imagePath, int count);

  // 清空对象池
  void clear(const std::string &imagePath = "");

  // 获取池中可用对象数量
  size_t getAvailableCount(const std::string &imagePath = "") const;

  // 获取当前活跃对象数量
  size_t getActiveCount() const;

  // 打印池状态
  void logPoolStatus() const;

private:
  SpritePool();
  ~SpritePool();
  SpritePool(const SpritePool &) = delete;
  SpritePool &operator=(const SpritePool &) = delete;

  void resetSprite(cocos2d::Sprite *sprite);

  std::unordered_map<std::string, std::vector<cocos2d::Sprite *>> _pool;
  size_t _activeCount;
};

#endif
```

### 2.2.6 理论依据与必要性

#### 设计原则遵循

| 原则 | 重构前 | 重构后 | 改进 |
|-----|-------|-------|------|
| **单一职责** | Map2Scene管理对象生命周期 | SpritePool专门管理 | ✅ 职责分离 |
| **开放封闭** | 修改创建逻辑需改Map2Scene | 只需修改SpritePool | ✅ 扩展开放 |
| **依赖倒置** | 直接依赖Sprite::create | 依赖SpritePool接口 | ✅ 降低耦合 |

#### 对象池模式必要性

1. **性能优化**: 减少频繁的内存分配/释放
2. **资源复用**: 对象可被多次使用
3. **内存稳定**: 减少内存碎片化
4. **可监控**: 便于追踪对象使用情况

### 2.2.7 实际效益分析

| 指标 | 重构前 | 重构后 | 改进幅度 |
|-----|-------|-------|---------|
| Sprite::create() 调用 | 每次1次 | 仅首次 | **显著减少** |
| 内存分配频率 | 每次操作 | 仅池不足时 | **显著减少** |
| 对象销毁次数 | 每次收获 | 0 | **100%消除** |
| 内存使用监控 | 无 | 有 | **增加可观测性** |

---

# 三、完整系统设计

## 3.1 两种模式的协同

| 模式 | 类型 | 解决的问题 | 影响的类 |
|-----|------|-----------|---------|
| **Singleton** | 创建型 | 全局状态集中管理 | Player, GameScene, BackpackScene, MapScene |
| **Object Pool** | 创建型 | 对象复用减少内存开销 | Map2Scene, SpritePool |

两种创建型模式协同工作：
- **Singleton** 优化了全局状态的访问和管理
- **Object Pool** 优化了频繁创建对象的性能问题
- 两者共同提升了代码质量和运行性能

---

# 四、其他重要问题与解决方案

## 4.1 线程安全

### 问题描述

GameManager 和 SpritePool 作为单例，需要考虑线程安全。

### 解决方案

```cpp
// C++11 保证静态局部变量初始化是线程安全的
GameManager* GameManager::getInstance() {
    static GameManager instance;
    return &instance;
}
```

## 4.2 向后兼容性

### 问题描述

需要保持现有代码的功能不变。

### 解决方案

1. **保留注释**: 在修改处添加原始代码的注释
2. **渐进式迁移**: GlobalVars 可以暂时保留，逐步迁移
3. **回退机制**: 必要时可快速恢复

## 4.3 对象池内存管理

### 问题描述

需要正确管理池中对象的内存。

### 解决方案

```cpp
// 获取时 retain，确保对象不被自动释放
sprite->retain();

// 归还时重置状态
void SpritePool::resetSprite(Sprite* sprite) {
    sprite->setPosition(Vec2::ZERO);
    sprite->setScale(1.0f);
    sprite->setVisible(false);
    sprite->stopAllActions();
    sprite->removeAllChildren();
}

// 清理时 release
void SpritePool::clear() {
    for (auto& pair : _pool) {
        for (auto sprite : pair.second) {
            sprite->release();
        }
    }
    _pool.clear();
}
```

## 4.4 值范围验证

### 问题描述

全局变量无验证，可能设置无效值。

### 解决方案

```cpp
void GameManager::setPlayerSpeed(float speed) {
    // 添加范围检查
    if (speed < 100.0f) speed = 100.0f;
    else if (speed > 300.0f) speed = 300.0f;
    _playerSpeed = speed;
    CCLOG("[GameManager] Player speed set to: %.2f", _playerSpeed);
}
```

---

# 五、文件变更清单

## 5.1 新增文件

| 文件路径 | 描述 | 设计模式 | 代码行数 |
|---------|------|---------|---------|
| `Classes/Manager/GameManager.h` | 游戏管理器单例头文件 | Singleton | ~60行 |
| `Classes/Manager/GameManager.cpp` | 游戏管理器单例实现 | Singleton | ~100行 |
| `Classes/Pool/SpritePool.h` | 精灵对象池头文件 | Object Pool | ~70行 |
| `Classes/Pool/SpritePool.cpp` | 精灵对象池实现 | Object Pool | ~120行 |

## 5.2 修改文件

| 文件路径 | 修改内容 | 设计模式 |
|---------|---------|---------|
| `Classes/Player/Player.cpp` | 使用GameManager替代extern变量 | Singleton |
| `Classes/Scene/GameScene.cpp` | 使用GameManager替代extern变量 | Singleton |
| `Classes/Scene/BackpackScene.cpp` | 使用GameManager设置速度 | Singleton |
| `Classes/Scene/MapScene.cpp` | 使用GameManager替代extern变量 | Singleton |
| `CMakeLists.txt` | 添加新源文件 | - |

---

# 六、总结

## 6.1 重构成果

| 方面 | 改进 |
|-----|------|
| **代码质量** | 消除全局变量，集中状态管理 |
| **性能优化** | 对象复用减少内存分配 |
| **可维护性** | 依赖关系明确，代码结构清晰 |
| **可扩展性** | 添加新状态或对象类型更容易 |
| **可测试性** | 单例可Mock，便于单元测试 |

## 6.2 设计模式价值

| 模式 | 价值体现 |
|-----|---------|
| **Singleton** | 全局状态集中管理，提供统一访问点 |
| **Object Pool** | 对象复用，减少内存分配开销 |

## 6.3 后续建议

1. **状态持久化**: GameManager 可添加保存/加载游戏设置功能
2. **池大小限制**: SpritePool 可添加最大容量限制
3. **预热机制**: 游戏启动时预创建常用对象

---

**文档结束**

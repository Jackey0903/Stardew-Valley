# 同学1 设计模式重构 - PlantUML 类图代码

本文档包含所有 UML 图的 PlantUML 代码，可直接在 PlantUML 工具中渲染。

---

## 一、Flyweight 享元模式

### 1.1 重构前类图

```plantuml
@startuml Flyweight_Before

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 14
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE
skinparam class {
    BackgroundColor #F5F5F5
    BorderColor #333333
    ArrowColor #333333
}

title 重构前类图 - 动画资源管理

package "Cocos2d-x Framework" #DDDDDD {
    class SpriteFrameCache <<Singleton>> {
        - static instance : SpriteFrameCache*
        + {static} getInstance() : SpriteFrameCache*
        + addSpriteFramesWithFile(plist : string) : void
        + getSpriteFrameByName(name : string) : SpriteFrame*
    }
    
    class Animation {
        + createWithSpriteFrames(frames, delay) : Animation*
    }
    
    class Animate {
        + create(animation) : Animate*
    }
}

package "Game Classes" #FFFFCC {
    class Player {
        - _playerSprite : Sprite*
        - _currentDirection : string
        --
        + init() : bool
        + startWalkingAnimation(direction : string) : void
        + stopWalkingAnimation() : void
        ..问题代码..
        <color:red>每次init()都加载plist文件</color>
        <color:red>每次播放动画都创建新Animation</color>
    }
    
    class Animal {
        - _animalSprite : Sprite*
        - animalName : string
        --
        + init(area : Rect) : bool
        + loadAnimations() : void
        + startWalkingAnimation(dir : Direction) : void
        ..问题代码..
        <color:red>重复加载与Player相同资源</color>
        <color:red>每个实例独立加载</color>
    }
}

Player ..> SpriteFrameCache : <<直接依赖>>
Player ..> Animation : <<每次创建新对象>>
Animal ..> SpriteFrameCache : <<直接依赖>>
Animal ..> Animation : <<每次创建新对象>>

note right of Player
  <b>问题1</b>: 每个Player实例
  都执行4次addSpriteFramesWithFile()
  
  <b>问题2</b>: startWalkingAnimation()
  每次都创建新的Animation对象
end note

note right of Animal
  <b>问题3</b>: 与Player相同的
  动画加载逻辑完全重复
end note

@enduml
```

---

### 1.2 重构后类图

```plantuml
@startuml Flyweight_After

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 14
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE
skinparam class {
    BackgroundColor #F5F5F5
    BorderColor #333333
    ArrowColor #333333
}
skinparam stereotypeCBackgroundColor #98FB98

title 重构后类图 - Flyweight 享元模式

package "Cocos2d-x Framework" #DDDDDD {
    class SpriteFrameCache <<Singleton>> {
        + {static} getInstance() : SpriteFrameCache*
        + addSpriteFramesWithFile(plist) : void
        + getSpriteFrameByName(name) : SpriteFrame*
    }
    
    class Animation <<Flyweight>> {
        + createWithSpriteFrames(frames, delay) : Animation*
        + retain() : void
    }
}

package "Design Pattern - Flyweight" #E8F5E9 {
    class AnimationFlyweight <<Singleton>> <<FlyweightFactory>> {
        - {static} _instance : AnimationFlyweight*
        - _animationPool : map<string, Animation*>
        ==
        + {static} getInstance() : AnimationFlyweight*
        + getAnimation(key : string) : Animation*
        + hasAnimation(key : string) : bool
        + preloadPlayerAnimations() : void
        + preloadAnimalAnimations(name : string) : void
        + clearAllAnimations() : void
        + getCachedAnimationCount() : size_t
        --
        - createWalkAnimation(prefix, count, interval) : Animation*
    }
}

package "Game Classes" #FFFFCC {
    class Player <<Client>> {
        - _playerSprite : Sprite*
        - _currentDirection : string
        --
        + init() : bool
        + startWalkingAnimation(direction : string) : void
        + stopWalkingAnimation() : void
    }
    
    class Animal <<Client>> {
        - _animalSprite : Sprite*
        - animalName : string
        --
        + init(area : Rect) : bool
        + loadAnimations() : void
        + startWalkingAnimation(dir : Direction) : void
    }
}

AnimationFlyweight --> SpriteFrameCache : 内部使用
AnimationFlyweight "1" *-- "*" Animation : 管理缓存

Player --> AnimationFlyweight : 获取共享动画
Animal --> AnimationFlyweight : 获取共享动画

note top of AnimationFlyweight
  <b>享元工厂</b>
  1. 单例模式确保全局唯一
  2. _animationPool 作为享元池
  3. Animation对象被retain()保持引用
  4. 多个Client共享同一份Animation
end note

note bottom of Player
  <b>改进后</b>
  只需调用:
  AnimationFlyweight::getInstance()
    ->getAnimation("Player_Walk_Left")
end note

@enduml
```

---

### 1.3 重构前序列图

```plantuml
@startuml Flyweight_Sequence_Before

!theme cerulean
skinparam sequenceMessageAlign center
skinparam backgroundColor #FEFEFE

title 重构前序列图 - 动画播放流程

actor User
participant ":Player" as Player
participant ":SpriteFrameCache" as SFC
participant ":Animation" as Anim
participant ":Animate" as Animate

== Player 初始化 ==

User -> Player : create()
activate Player

Player -> SFC : addSpriteFramesWithFile("Walk_Left.plist")
Player -> SFC : addSpriteFramesWithFile("Walk_Right.plist")
Player -> SFC : addSpriteFramesWithFile("Walk_Up.plist")
Player -> SFC : addSpriteFramesWithFile("Walk_Down.plist")

note right of Player #FFCCCC
  <b>问题</b>: 每个Player实例
  都执行4次文件加载
end note

deactivate Player

== 玩家开始移动 (每次都执行) ==

User -> Player : startWalkingAnimation("Left")
activate Player

loop i = 1 to 2
    Player -> SFC : getSpriteFrameByName("Walk_Left_" + i)
    SFC --> Player : SpriteFrame*
end

Player -> Anim : createWithSpriteFrames(frames, 0.1f)
note right of Anim #FFCCCC
  <b>问题</b>: 每次移动
  都创建新Animation对象
end note
Anim --> Player : Animation*

Player -> Animate : create(animation)
Animate --> Player : Animate*

Player -> Player : runAction(RepeatForever)

deactivate Player

@enduml
```

---

### 1.4 重构后序列图

```plantuml
@startuml Flyweight_Sequence_After

!theme cerulean
skinparam sequenceMessageAlign center
skinparam backgroundColor #FEFEFE

title 重构后序列图 - Flyweight 模式

actor User
participant ":Player" as Player
participant ":AnimationFlyweight" as AF
participant ":SpriteFrameCache" as SFC
participant ":Animation" as Anim

== Player 初始化 ==

User -> Player : create()
activate Player

Player -> AF : getInstance()
activate AF

Player -> AF : preloadPlayerAnimations()

note right of AF #CCFFCC
  <b>首次加载时</b>
  加载并缓存所有动画
end note

AF -> SFC : addSpriteFramesWithFile(...)
AF -> Anim : createWithSpriteFrames(...)
Anim --> AF : Animation*
AF -> Anim : retain()
note right of Anim #CCFFCC
  Animation被缓存
  不会被自动释放
end note

AF --> Player : 预加载完成

deactivate AF
deactivate Player

== 玩家开始移动 ==

User -> Player : startWalkingAnimation("Left")
activate Player

Player -> AF : getAnimation("Player_Walk_Left")
activate AF

note right of AF #CCFFCC
  从缓存直接返回
  无需创建新对象
end note

AF --> Player : Animation* (已缓存)
deactivate AF

Player -> Player : Animate::create(animation)
Player -> Player : runAction(RepeatForever)

deactivate Player

@enduml
```

---

## 二、Adapter 适配器模式

### 2.1 重构前类图

```plantuml
@startuml Adapter_Before

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 14
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE
skinparam class {
    BackgroundColor #F5F5F5
    BorderColor #333333
    ArrowColor #333333
}

title 重构前类图 - 输入处理系统

package "Cocos2d-x Framework" #DDDDDD {
    class EventListenerKeyboard {
        + onKeyPressed : function
        + onKeyReleased : function
        + create() : EventListenerKeyboard*
    }
    
    class "ui::Button" as Button {
        + create(image) : Button*
        + addTouchEventListener(callback) : void
        + setPosition(pos) : void
        + setScale(scale) : void
    }
}

package "Game Classes" #FFFFCC {
    class Player {
        - _btnUp : Button*
        - _btnDown : Button*
        - _btnLeft : Button*
        - _btnRight : Button*
        - _btnBackpack : Button*
        - _btnMap : Button*
        - _keyboardListener : EventListenerKeyboard*
        - _isMovingUp : bool
        - _isMovingDown : bool
        - _isMovingLeft : bool
        - _isMovingRight : bool
        - _isMoving : bool
        ==
        + init() : bool
        + <color:red>initTouchControls()</color> : void
        + <color:red>onKeyPressed(keyCode, event)</color> : void
        + <color:red>onKeyReleased(keyCode, event)</color> : void
        + startWalkingAnimation(direction) : void
        + stopWalkingAnimation() : void
        + openBackpack() : void
        + openMapScene() : void
    }
}

Player --> EventListenerKeyboard : 创建和管理
Player --> Button : 创建6个按钮

note right of Player #FFCCCC
  <b>问题列表</b>
  ━━━━━━━━━━━━
  1. 6个Button成员变量
  2. 键盘监听器成员
  3. initTouchControls() ~150行
  4. onKeyPressed() ~50行
  5. onKeyReleased() ~35行
  ━━━━━━━━━━━━
  <b>重复代码约400行</b>
  
  键盘和触摸处理逻辑
  几乎完全相同！
end note

@enduml
```

---

### 2.2 重构后类图

```plantuml
@startuml Adapter_After

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 14
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE
skinparam class {
    BackgroundColor #F5F5F5
    BorderColor #333333
    ArrowColor #333333
}
skinparam stereotypeCBackgroundColor #98FB98

title 重构后类图 - Adapter 适配器模式

package "Cocos2d-x Framework" #DDDDDD {
    class EventListenerKeyboard <<Adaptee>> {
        + onKeyPressed : function
        + onKeyReleased : function
    }
    
    class "ui::Button" as Button <<Adaptee>> {
        + addTouchEventListener(callback) : void
    }
}

package "Design Pattern - Adapter" #E3F2FD {
    interface IInputHandler <<Target>> {
        + {abstract} onMoveStart(direction : string) : void
        + {abstract} onMoveStop(direction : string) : void
        + {abstract} onAction(action : string) : void
    }
    
    class KeyboardInputAdapter <<Adapter>> {
        - _handler : IInputHandler*
        - _listener : EventListenerKeyboard*
        ==
        + KeyboardInputAdapter(handler : IInputHandler*)
        + setupListener(node : Node*) : void
        + removeListener() : void
        --
        - onKeyPressed(keyCode, event) : void
        - onKeyReleased(keyCode, event) : void
        - keyCodeToDirection(keyCode) : string
        - keyCodeToAction(keyCode) : string
    }
    
    class TouchInputAdapter <<Adapter>> {
        - _handler : IInputHandler*
        - _btnUp : Button*
        - _btnDown : Button*
        - _btnLeft : Button*
        - _btnRight : Button*
        - _btnBackpack : Button*
        - _btnMap : Button*
        ==
        + TouchInputAdapter(handler : IInputHandler*)
        + createButtons(parent, x, y, radius) : void
        --
        - setupDirectionButton(btn, direction) : void
        - setupActionButton(btn, action) : void
    }
}

package "Game Classes" #FFFFCC {
    class Player <<Client>> {
        - _keyboardAdapter : KeyboardInputAdapter*
        - _touchAdapter : TouchInputAdapter*
        ==
        + init() : bool
        + initInputAdapters() : void
        ..IInputHandler实现..
        + onMoveStart(direction : string) : void
        + onMoveStop(direction : string) : void
        + onAction(action : string) : void
    }
}

IInputHandler <|.. Player : <<implements>>
KeyboardInputAdapter --> IInputHandler : adapts to
TouchInputAdapter --> IInputHandler : adapts to
KeyboardInputAdapter --> EventListenerKeyboard : wraps
TouchInputAdapter --> Button : wraps
Player --> KeyboardInputAdapter : uses
Player --> TouchInputAdapter : uses

note top of IInputHandler #CCFFCC
  <b>Target 接口</b>
  定义统一的输入处理方法
  无论键盘还是触摸，
  最终都调用这三个方法
end note

note right of KeyboardInputAdapter #CCFFCC
  <b>Adapter 1</b>
  将 KeyCode 转换为
  direction/action字符串
end note

note right of TouchInputAdapter #CCFFCC
  <b>Adapter 2</b>
  将触摸事件转换为
  onMoveStart/Stop调用
end note

note bottom of Player #CCFFCC
  <b>改进后</b>
  只需实现3个接口方法
  代码从~400行减少到~50行
end note

@enduml
```

---

### 2.3 重构前序列图

```plantuml
@startuml Adapter_Sequence_Before

!theme cerulean
skinparam sequenceMessageAlign center
skinparam backgroundColor #FEFEFE

title 重构前序列图 - 输入处理 (键盘)

actor User
participant ":Player" as Player
participant ":EventDispatcher" as ED
participant ":Sprite" as Sprite

== 初始化 ==

User -> Player : init()
activate Player

Player -> Player : initTouchControls()
note right #FFCCCC
  ~150行按钮创建代码
  每个按钮都有类似的
  事件处理lambda
end note

create EventListenerKeyboard
Player -> EventListenerKeyboard : create()
Player -> Player : 绑定 onKeyPressed
Player -> Player : 绑定 onKeyReleased
Player -> ED : addEventListenerWithSceneGraphPriority()

deactivate Player

== 用户按下 W 键 ==

User -> EventListenerKeyboard : 按下 W
EventListenerKeyboard -> Player : onKeyPressed(KEY_W, event)
activate Player

Player -> Player : _isMoving = true
Player -> Player : _isMovingUp = true
Player -> Player : _currentDirection = "Up"
Player -> Sprite : setTexture("Stand_Up.png")
Player -> Player : startWalkingAnimation("Up")

note right #FFCCCC
  此逻辑在触摸处理中
  完全重复一遍！
end note

deactivate Player

@enduml
```

---

### 2.4 重构后序列图

```plantuml
@startuml Adapter_Sequence_After

!theme cerulean
skinparam sequenceMessageAlign center
skinparam backgroundColor #FEFEFE

title 重构后序列图 - Adapter 模式

actor User
participant ":Player" as Player
participant ":KeyboardInputAdapter" as KA
participant ":TouchInputAdapter" as TA
participant ":IInputHandler" as IH

== 初始化 ==

User -> Player : init()
activate Player

Player -> Player : initInputAdapters()

create KA
Player -> KA : new KeyboardInputAdapter(this)
Player -> KA : setupListener(this)

create TA
Player -> TA : new TouchInputAdapter(this)
Player -> TA : createButtons(...)

note right of Player #CCFFCC
  仅需4行代码
  替代原来~200行
end note

deactivate Player

== 用户按下 W 键 ==

User -> KA : 键盘事件 (KEY_W)
activate KA

KA -> KA : keyCodeToDirection(KEY_W)
note right : 返回 "Up"

KA -> IH : onMoveStart("Up")
note right #CCFFCC
  统一接口调用
  无论键盘还是触摸
end note
deactivate KA

activate Player
note over Player : Player 实现了 IInputHandler

Player -> Player : _isMovingUp = true
Player -> Player : 更新状态和动画

deactivate Player

== 用户点击触摸按钮 ==

User -> TA : 触摸上按钮
activate TA

TA -> IH : onMoveStart("Up")
note right #CCFFCC
  与键盘调用完全相同！
end note

deactivate TA

activate Player
Player -> Player : 相同的处理逻辑
deactivate Player

@enduml
```

---

### 2.5 扩展性示例 - 添加手柄支持

```plantuml
@startuml Adapter_Extension

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 14
skinparam backgroundColor #FEFEFE

title 扩展性示例 - 添加手柄支持

interface IInputHandler <<Target>> {
    + onMoveStart(direction) : void
    + onMoveStop(direction) : void
    + onAction(action) : void
}

class KeyboardInputAdapter <<Adapter>> {
    - _handler : IInputHandler*
}

class TouchInputAdapter <<Adapter>> {
    - _handler : IInputHandler*
}

class GamepadInputAdapter <<Adapter>> #90EE90 {
    - _handler : IInputHandler*
    - _gamepad : Gamepad*
    ==
    + GamepadInputAdapter(handler)
    + setupGamepad() : void
    --
    - onButtonPressed(button) : void
    - onJoystickMoved(x, y) : void
}

class Player <<Client>> {
    - _keyboardAdapter : KeyboardInputAdapter*
    - _touchAdapter : TouchInputAdapter*
    - _gamepadAdapter : GamepadInputAdapter*
}

IInputHandler <|.. Player
KeyboardInputAdapter --> IInputHandler
TouchInputAdapter --> IInputHandler
GamepadInputAdapter --> IInputHandler

Player --> KeyboardInputAdapter
Player --> TouchInputAdapter
Player --> GamepadInputAdapter

note bottom of GamepadInputAdapter #90EE90
  <b>新增适配器</b>
  只需创建新类
  不修改Player代码
  
  符合<b>开放封闭原则</b>
end note

@enduml
```

---

## 三、综合类图

### 3.1 完整重构后系统类图

```plantuml
@startuml Complete_System

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 12
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE
skinparam packageStyle rectangle

title 星露谷物语 - 同学1重构后系统类图

package "Flyweight Pattern" #E8F5E9 {
    class AnimationFlyweight <<Singleton>> {
        - _animationPool : map<string, Animation*>
        + getInstance() : AnimationFlyweight*
        + getAnimation(key) : Animation*
        + preloadPlayerAnimations()
        + preloadAnimalAnimations(name)
    }
}

package "Adapter Pattern" #E3F2FD {
    interface IInputHandler {
        + onMoveStart(direction)
        + onMoveStop(direction)
        + onAction(action)
    }
    
    class KeyboardInputAdapter {
        - _handler : IInputHandler*
        + setupListener(node)
    }
    
    class TouchInputAdapter {
        - _handler : IInputHandler*
        + createButtons(parent, ...)
    }
}

package "Game Entities" #FFFDE7 {
    class Player {
        - _keyboardAdapter
        - _touchAdapter
        + init() : bool
        + onMoveStart(direction)
        + onMoveStop(direction)
        + onAction(action)
        + startWalkingAnimation(direction)
    }
    
    class Animal {
        - animalName : string
        + init(area) : bool
        + startWalkingAnimation(dir)
    }
}

IInputHandler <|.. Player
KeyboardInputAdapter --> IInputHandler
TouchInputAdapter --> IInputHandler
Player --> KeyboardInputAdapter
Player --> TouchInputAdapter

Player --> AnimationFlyweight : 获取动画
Animal --> AnimationFlyweight : 获取动画

note "两种结构型模式协同工作\n提升代码质量和可维护性" as N1

@enduml
```

---

## 四、使用说明

### PlantUML 渲染方式

1. **在线渲染**: 访问 https://www.plantuml.com/plantuml/
2. **VS Code 插件**: 安装 "PlantUML" 插件
3. **本地渲染**: 安装 Java + Graphviz + plantuml.jar

### 导出为图片

```bash
# 命令行导出
java -jar plantuml.jar diagrams.puml -tpng

# 导出为SVG (矢量图,推荐)
java -jar plantuml.jar diagrams.puml -tsvg
```

# 同学2 设计模式重构 - PlantUML 类图代码

本文档包含所有 UML 图的 PlantUML 代码，可直接在 PlantUML 工具中渲染。

---

## 一、State 状态模式

### 1.1 重构前类图

```plantuml
@startuml State_Before

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

title 重构前类图 - 玩家状态管理

package "Game Classes" #FFFFCC {
    class Player {
        - _isMoving : bool
        - _isMovingLeft : bool
        - _isMovingRight : bool
        - _isMovingUp : bool
        - _isMovingDown : bool
        - _currentDirection : string
        --
        + onKeyPressed(keyCode, event) : void
        + onKeyReleased(keyCode, event) : void
        + update(delta) : void
        ..问题代码..
        <color:red>大量布尔标志位管理状态</color>
        <color:red>onKeyPressed中包含复杂Switch-Case</color>
    }
}

note right of Player
  <b>问题</b>:
  1. 状态逻辑分散在各个布尔变量中
  2. 难以扩展新的状态（如攻击、受伤）
  3. update() 中包含大量 if-else 判断
end note

@enduml
```

---

### 1.2 重构后类图

```plantuml
@startuml State_After

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

title 重构后类图 - State 状态模式

package "Game Classes" #FFFFCC {
    class Player <<Context>> {
        - _state : PlayerState*
        --
        + changeState(newState : PlayerState*) : void
        + handleInput(input : InputData) : void
        + update(delta) : void
        + move(x, y) : void
    }
}

package "Design Pattern - State" #E8F5E9 {
    abstract class PlayerState <<State>> {
        # _player : Player*
        --
        + {abstract} handleInput(input : InputData) : void
        + {abstract} update(delta) : void
        + {abstract} enter() : void
        + {abstract} exit() : void
    }

    class IdleState <<ConcreteState>> {
        + handleInput(input) : void
        + update(delta) : void
    }

    class MovingState <<ConcreteState>> {
        - _direction : Vec2
        --
        + handleInput(input) : void
        + update(delta) : void
    }
}

Player o-> PlayerState : 维护当前状态
PlayerState <|-- IdleState
PlayerState <|-- MovingState

note right of PlayerState
  <b>状态接口</b>
  定义了所有状态共有的行为
end note

note bottom of Player
  <b>改进后</b>
  Player 将行为委托给 _state 处理
  消除了大量的 if-else 和标志位
end note

@enduml
```

---

### 1.3 重构前序列图

```plantuml
@startuml State_Sequence_Before

!theme cerulean
skinparam sequenceMessageAlign center
skinparam backgroundColor #FEFEFE

title 重构前序列图 - 输入处理与状态更新

actor User
participant ":Player" as Player

== 用户按下按键 ==

User -> Player : onKeyPressed(KEY_W)
activate Player

Player -> Player : _isMoving = true
Player -> Player : _isMovingUp = true
Player -> Player : _currentDirection = "Up"
Player -> Player : startWalkingAnimation()

deactivate Player

== 游戏循环 Update ==

loop Every Frame
    User -> Player : update(delta)
    activate Player
    
    Player -> Player : check _isMovingUp
    alt _isMovingUp is true
        Player -> Player : y += speed * delta
    end
    
    deactivate Player
end

@enduml
```

---

### 1.4 重构后序列图

```plantuml
@startuml State_Sequence_After

!theme cerulean
skinparam sequenceMessageAlign center
skinparam backgroundColor #FEFEFE

title 重构后序列图 - State 模式

actor User
participant ":Player" as Player
participant ":IdleState" as Idle
participant ":MovingState" as Moving

== 初始化 ==

User -> Player : init()
activate Player
create Idle
Player -> Idle : new()
Player -> Player : changeState(Idle)
deactivate Player

== 用户按下按键 ==

User -> Player : onKeyPressed(KEY_W)
activate Player

Player -> Idle : handleInput(KEY_W)
activate Idle

create Moving
Idle -> Moving : new(Direction.UP)
Idle -> Player : changeState(Moving)
deactivate Idle

Player -> Moving : enter()
activate Moving
Moving -> Player : startWalkingAnimation("Up")
deactivate Moving

deactivate Player

== 游戏循环 Update ==

loop Every Frame
    User -> Player : update(delta)
    activate Player
    
    Player -> Moving : update(delta)
    activate Moving
    Moving -> Player : move(0, speed)
    deactivate Moving
    
    deactivate Player
end

@enduml
```

---

## 二、Observer 观察者模式

### 2.1 重构前类图

```plantuml
@startuml Observer_Before

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 14
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE

title 重构前类图 - 事件处理

package "Game Classes" #FFFFCC {
    class Player {
        + openBackpack() : void
        + openMapScene() : void
    }
    
    class BackpackScene {
        + {static} createScene() : Scene*
    }
    
    class MapScene {
        + {static} createScene() : Scene*
    }
}

Player ..> BackpackScene : <<直接耦合>> 直接调用创建场景
Player ..> MapScene : <<直接耦合>> 直接调用创建场景

note right of Player
  <b>问题</b>:
  Player 直接依赖具体场景类
  当需要添加新的交互反馈（如音效、成就）时
  必须修改 Player 类
end note

@enduml
```

---

### 2.2 重构后类图

```plantuml
@startuml Observer_After

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 14
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE
skinparam stereotypeCBackgroundColor #98FB98

title 重构后类图 - Observer 观察者模式

package "Design Pattern - Observer" #E3F2FD {
    interface EventObserver <<Observer>> {
        + {abstract} onEvent(eventType : EventType, data : void*) : void
    }
    
    class EventManager <<Subject>> <<Singleton>> {
        - _observers : Map<EventType, List<EventObserver*>>
        --
        + {static} getInstance() : EventManager*
        + addObserver(type, observer) : void
        + removeObserver(type, observer) : void
        + notifyObservers(type, data) : void
    }
}

package "Game Classes" #FFFFCC {
    class Player {
        ...
        + openBackpack()
    }
    
    class SceneManager <<ConcreteObserver>> {
        + onEvent(eventType, data) : void
    }
    
    class AudioSystem <<ConcreteObserver>> {
        + onEvent(eventType, data) : void
    }
}

Player ..> EventManager : 发送事件
SceneManager ..|> EventObserver
AudioSystem ..|> EventObserver
EventManager o-> EventObserver : 通知

note top of EventManager
  <b>事件中心</b>
  解耦事件发送者(Player)
  和接收者(SceneManager等)
end note

@enduml
```

---

### 2.3 重构后序列图

```plantuml
@startuml Observer_Sequence_After

!theme cerulean
skinparam sequenceMessageAlign center
skinparam backgroundColor #FEFEFE

title 重构后序列图 - 事件分发

actor User
participant ":Player" as Player
participant ":EventManager" as EM
participant ":SceneManager" as SM
participant ":AudioSystem" as Audio

== 系统初始化 ==

SM -> EM : addObserver(OPEN_BACKPACK, this)
Audio -> EM : addObserver(OPEN_BACKPACK, this)

== 触发事件 ==

User -> Player : 按下背包键 (B)
activate Player

Player -> EM : notifyObservers(OPEN_BACKPACK)
activate EM

loop 遍历观察者
    EM -> SM : onEvent(OPEN_BACKPACK)
    activate SM
    SM -> SM : 切换到背包场景
    deactivate SM
    
    EM -> Audio : onEvent(OPEN_BACKPACK)
    activate Audio
    Audio -> Audio : 播放背包音效
    deactivate Audio
end

deactivate EM
deactivate Player

@enduml
```

---

## 三、完整系统类图

```plantuml
@startuml Complete_System_Student2

!theme cerulean
skinparam classAttributeIconSize 0
skinparam classFontSize 12
skinparam classFontName Microsoft YaHei
skinparam backgroundColor #FEFEFE
skinparam packageStyle rectangle

title 星露谷物语 - 同学2重构后系统类图

package "State Pattern" #E8F5E9 {
    class PlayerState {
        + handleInput()
        + update()
    }
    class IdleState
    class MovingState
    
    PlayerState <|-- IdleState
    PlayerState <|-- MovingState
}

package "Observer Pattern" #E3F2FD {
    class EventManager <<Singleton>> {
        + notifyObservers()
    }
    interface EventObserver {
        + onEvent()
    }
    
    EventManager o-> EventObserver
}

package "Game Entities" #FFFDE7 {
    class Player {
        - _state : PlayerState*
        + changeState()
    }
    
    class SceneManager
    class AudioSystem
}

Player o-> PlayerState
Player ..> EventManager : 使用

SceneManager ..|> EventObserver
AudioSystem ..|> EventObserver

@enduml
```

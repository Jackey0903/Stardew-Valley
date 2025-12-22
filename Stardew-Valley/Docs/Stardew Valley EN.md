# Stardew Valley Project Documentation



### Group:No.17

| 胡浩杰 | 2351493 | 15058073343 | 2351493@tongji.edu.cn |
| ------ | ------- | ----------- | --------------------- |
| 许君达 | 2351436 | 13336928179 | 2351436@tongji.edu.cn |
| 卜天   | 2350939 | 15947513567 | 2350939@tongji.edu.cn |
| 曹津硕 | 2353127 | 15568008329 | 2353127@tongji.edu.cn |
| 胡正华 | 2353741 | 19117172189 | 2353741@tongji.edu.cn |



## I. Project Overview

### 1.1 Project Background

With the rise of simulation and management games such as *Stardew Valley* and *Harvest Moon*, this genre has attracted many players with its relaxing, free-form gameplay and rich system content. The core gameplay of simulation/management titles is to continuously develop a personal farm and experience a complete rural life through farm management, resource collection, and character progression. These games emphasize long-term planning and management strategy, while also providing a high degree of freedom and immersion.

Against this backdrop, this project aims to develop a farm simulation and management game similar to *Stardew Valley*. In the original version, the project implemented the basic farm management gameplay, including crop planting and harvesting, animal husbandry, resource gathering, and character ability progression. At the same time, the game introduced a community interaction and quest system: players can build relationships with residents in town, complete commissioned tasks, and participate in various community activities, enriching the overall content and gameplay layers.

In addition, the project supports map exploration and adventure gameplay. Players can explore areas such as forests, mountains, and caves to gather minerals and rare resources. Through the integrated design of the farm management, community interaction, and exploration systems, players can experience the fun of running a farm, growing their character, and exploring freely in a countryside world full of life.

### 1.2 Project Introduction

This project is a farm simulation and management game developed with the Cocos2d-x 3.17.2 engine, inspired by classic titles such as *Stardew Valley* and *Harvest Moon*. Centered around farm life, players cultivate crops, raise animals, gather resources, and participate in community activities to continuously develop their farm and improve character abilities, interacting with residents and the environment in the game world. The game not only recreates the core gameplay of classic farm simulation titles, but also blends exploration and progression elements, offering a relaxing and immersive experience.

Key features include:

- A diverse farm management system: supports planting and harvesting multiple crops, as well as raising animals such as cows, sheep, and pigs. Players can expand farm scale and profits through reasonable layout planning and management strategies.
- A community interaction and quest system: players can build relationships with town residents, accept and complete various commissioned tasks, and participate in community activities, enhancing the interactivity and lived-in feeling of the game world.
- Exploration and resource gathering: the game provides multiple explorable areas, including forests, mountains, and caves. During exploration, players can mine minerals and collect rare resources, enriching gameplay depth.
- Character progression and equipment: the game includes a character skill growth mechanism. As the game progresses, players can improve character stats and switch equipment via the backpack interface to strengthen overall capabilities.
- UI and controls experience: the game adopts clear and intuitive controls and functional interfaces, combined with basic audio feedback, improving immersion and playability.

The goal of this project is to provide players with a farm simulation game that combines both management fun and exploration experiences. Through rich system design and diverse gameplay content, it builds a virtual countryside world full of life and freedom, enabling players to gain sustained achievement and enjoyment throughout the game.

#### Game Screenshots

![Main Game Interface](1.png)

![Farm Scene](2.png)

![Character Interaction](3.png)

![Exploration Scene](4.png)

![Backpack System](5.png)

![Skill System](6.png)

![Gameplay](7.png)

### 1.3 Main Features

- **Farm Management**

  - Players can plow, plant, and harvest on the farm, supporting multiple crop types such as carrots and pumpkins.

  - The farm system supports basic operations such as watering, fertilizing, and harvesting, forming one of the core gameplay loops.

  - Supports planting and cutting down trees to obtain basic resources needed for farm operations.

- **Community Interaction**

  - Players can build relationships with town residents, including friendship and romance, and can talk and interact with their partner at home.

  - The game supports accepting and completing commissions posted by residents, such as collecting specific items or helping repair buildings, to drive progression and unlock content.

- **Exploration and Adventure**

  - Players can explore areas around the farm, including forests, mountains, and mysterious caves.

  - During exploration, players can mine minerals and collect rare gems in caves to obtain important resources.

- **Character Growth and Skills**

  - The game provides a skill tree system with multiple progression directions such as movement speed and strength. As skills level up, the character’s capabilities gradually improve.

  - Supports item management and equipment switching in the backpack interface to enhance overall character ability and gameplay experience.

## II. Refactoring with Structural Patterns

### 2.1 Refactoring with the Flyweight Pattern

#### 2.1.1 Pattern Overview

The **Flyweight Pattern** is a structural design pattern that uses sharing to efficiently support the reuse of large numbers of fine-grained objects, reducing memory usage and improving performance.

**Pattern Structure**

| Role                  | Responsibility                           |
| --------------------- | ---------------------------------------- |
| **Flyweight**         | Defines the interface for shared objects |
| **ConcreteFlyweight** | Implements the shared concrete object    |
| **FlyweightFactory**  | Creates and manages flyweight objects    |
| **Client**            | Uses flyweight objects                   |

**Applicable Scenarios**

- The system has a large number of similar objects
- Most of an object’s state can be externalized
- A pooling mechanism is needed to reuse objects

#### 2.1.2 Problem Analysis

Before refactoring, the animation resource management for characters had the following issues:

**Issue 1: Scattered resource loading and repeated object creation**

- The `Player` class and the `Animal` class each load the same animation resources, such as the plist files used for walking animations.
- Each instance calls `SpriteFrameCache::addSpriteFramesWithFile` during initialization, lacking a unified resource management mechanism, causing repeated resource loading and memory waste.

```cpp
// Similar code exists in both Player.cpp / Animal.cpp
SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Action/Walk_Left.plist");
SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Action/Walk_Right.plist");
SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Action/Walk_Up.plist");
SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Action/Walk_Down.plist");
```

**Issue 2: Frequent animation object creation with high performance overhead**

- During character movement, each time an animation is triggered, new `Animation` and `Animate` objects are created.
- The animation frame data is logically identical, but it is not cached and reused, introducing unnecessary creation/destruction overhead.

```cpp
void Player::startWalkingAnimation(const std::string &direction) {
    ...
    // Issue: a new Animation object is created on every call
    auto walkAnimation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(walkAnimation);
    _playerSprite->runAction(RepeatForever::create(animate));
}
```

**Issue 3: Unclear responsibilities and high coupling**

- `Player` and `Animal` directly depend on `SpriteFrameCache`, and are responsible for both animation resource loading and animation playback logic.
- Animation management logic is scattered across multiple classes, violating the Single Responsibility Principle and making future maintenance and extension harder.

##### Why Refactoring Is Necessary

In summary, the original animation system has obvious problems in resource management and object creation: animation resource loading is scattered with no unified management; animation objects are created frequently, causing memory waste and performance loss; and character classes take on too much animation-management responsibility, increasing coupling.

To address these issues, a structural refactor of the animation system is necessary. By **sharing reusable objects and centralizing management of identical state data**, the Flyweight pattern can effectively reduce repeated object creation, lower memory usage, and improve overall system performance, making it well suited for animation resource management in this project.

------

#### 2.1.3 UML Class Diagram Comparison

##### Before Refactoring

![Before Refactoring - Flyweight](UML/Flyweight_Before_Class.png)

##### After Refactoring

![After Refactoring - Flyweight](UML/Flyweight_After_Class.png)

------

#### 2.1.4 Refactoring Steps

To solve the issues above, this project refactors the animation system using the Flyweight pattern as follows:

1. **Introduce the flyweight factory class `AnimationFlyweight`**

Define the `AnimationFlyweight` class as the flyweight factory to centrally manage and cache animation objects. Internally, it maintains an animation pool that stores already-created `Animation` objects.

```cpp
class AnimationFlyweight {
public:
  // Get singleton instance
  static AnimationFlyweight *getInstance();
  // Get shared animation object (flyweight)
  cocos2d::Animation *getAnimation(const std::string &key);
  // Check whether an animation is loaded
  bool hasAnimation(const std::string &key) const;
  // Preload player animation resources
  void preloadPlayerAnimations();
  // Preload animal animation resources
  void preloadAnimalAnimations(const std::string &animalName);
  // Clear all cached animation resources
  void clearAllAnimations();
  // Get current number of cached animations
  size_t getCachedAnimationCount() const { return _animationPool.size(); }

private:
  AnimationFlyweight() = default;
  ~AnimationFlyweight();
  // Disable copy
  AnimationFlyweight(const AnimationFlyweight &) = delete;
  AnimationFlyweight &operator=(const AnimationFlyweight &) = delete;
  static AnimationFlyweight *_instance;
  // Flyweight pool: stores shared animation objects
  std::unordered_map<std::string, cocos2d::Animation *> _animationPool;
  // Create walking animation
  cocos2d::Animation *createWalkAnimation(const std::string &prefix,int frameCount,float interval = 0.1f);
};
```

2. **Centralize animation resource preloading and management**

Move animation loading logic originally scattered in `Player` and `Animal` into `AnimationFlyweight`. Preload resources at game startup or on first use.

```cpp
// Player.cpp (only load once)
#include "Animation/AnimationFlyweight.h"
...
AnimationFlyweight::getInstance()->preloadPlayerAnimations();
```

3. **Fetch shared animation objects through the flyweight factory**

When playing animations, no longer create `Animation` objects directly. Instead, retrieve cached animation instances from `AnimationFlyweight` using an animation identifier.

```cpp
void Player::startWalkingAnimation(const std::string &direction) {
    ...
    // Fetch shared Animation from the flyweight pool
    std::string animKey = "Player_Walk_" + direction;
    auto animation = AnimationFlyweight::getInstance()->getAnimation(animKey);

    if (animation) {
        _playerSprite->runAction(RepeatForever::create(Animate::create(animation)));
    }
}
```

4. **Update character classes to use flyweight objects**

`Player` and `Animal` are only responsible for retrieving animations from the flyweight factory and executing playback logic, no longer caring about the concrete creation process, thereby reducing coupling among classes.

------

#### 2.1.5 Improvements Achieved

By introducing the Flyweight pattern, the animation system is significantly improved in multiple aspects:

- **Reduced memory usage**: animations are cached and reused, avoiding repeated creation of identical `Animation` instances.
- **Improved runtime performance**: reduces frequent creation and destruction of animation objects, lowering overhead.
- **Clearer responsibilities**: animation resource management is centralized in `AnimationFlyweight`, while character classes focus on business logic.
- **Better maintainability and extensibility**: adding or modifying animation resources only requires updating the flyweight factory rather than multiple character classes.
- **Easier unified management and debugging**: centralized control simplifies preloading, releasing, and troubleshooting.

------

### 2.2 Refactoring with the Adapter Pattern

#### 2.2.1 Pattern Overview

The **Adapter Pattern** is a structural design pattern that converts the interface of a class into another interface that clients expect, enabling classes with incompatible interfaces to work together.

**Pattern Structure**

| Role        | Responsibility                                      |
| ----------- | --------------------------------------------------- |
| **Target**  | Defines the interface expected by the client        |
| **Adapter** | Converts the Adaptee interface into the Target one  |
| **Adaptee** | The existing interface being adapted                |
| **Client**  | The class that uses the Target interface            |

**Applicable Scenarios**

- You want to use an existing class, but its interface does not match requirements
- You need a reusable class that works with unrelated classes
- You need to unify the interfaces of multiple classes

#### 2.2.2 Problem Analysis

Before refactoring, the input-handling module had the following issues:

**Issue 1: Repeated input logic and severe code duplication**

- Player movement and interaction logic were implemented separately in the **keyboard input handler** and the **touch input handler**.
- Both input methods contained a large amount of identical movement checks and state updates, resulting in hundreds of duplicated lines.

```cpp
// Keyboard input handling
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
    _isMoving = true;
    // Movement logic
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
        if (!_isMovingLeft) {
            _isMovingLeft = true;
            _currentTexture = "Action/Stand_Left.png";
            _playerSprite->setTexture(_currentTexture);
            _currentDirection = "Left";
            startWalkingAnimation(_currentDirection);
        }
        break;
    // ... other directions are similar ...
    case EventKeyboard::KeyCode::KEY_B:
        openBackpack();
        break;
    case EventKeyboard::KeyCode::KEY_M:
        openMapScene();
        break;
    }
}
```

```cpp
// Touch input handling
void Player::initTouchControls() {
    _btnUp = ui::Button::create("../Resources/KEYS/U.png");
    _btnUp->setPosition(Vec2(KEYS_CENTER_X, KEYS_CENTER_Y + KEYS_RADIUS));
    _btnUp->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::BEGAN) {
            // Exactly the same logic as keyboard handling!
            if (!_isMovingUp) {
                _isMovingUp = true;
                _isMoving = true;
                _currentTexture = "Action/Stand_Up.png";
                _playerSprite->setTexture(_currentTexture);
                _currentDirection = "Up";
                startWalkingAnimation(_currentDirection);
            }
        }
    });
    this->addChild(_btnUp);
    // ... there are 5 more buttons, each with similar code ...
}
```

**Issue 2: The `Player` class has too many responsibilities and becomes bloated**

- The `Player` class handled character state, animation control, and concrete keyboard/touch input processing.
- It contained multiple button objects, listeners, and a large amount of input-related code, violating the Single Responsibility Principle.

**Issue 3: Poor extensibility**

- Current input logic is strongly coupled to specific input methods (keyboard, touch).
- Adding a new input method (e.g., gamepad) would require modifying the `Player` class directly, increasing maintenance cost and risk.

##### Why Refactoring Is Necessary

Overall, the original input system suffered from severe code duplication, unclear class responsibilities, and poor extensibility. Input logic was scattered across multiple functions, increasing maintenance difficulty and hindering future feature expansion.

To solve the incompatible interface and redundancy problems, a structural refactor of the input system is necessary. The Adapter pattern can unify different input interfaces via encapsulation, so the client depends only on a unified interface. This eliminates duplicate code, reduces coupling, and improves extensibility, making it highly suitable for this project’s input module.

------

#### 2.2.3 UML Class Diagram Comparison

##### Before Refactoring

![Before Refactoring - Adapter](UML/Adapter_Before_Class.png)

##### After Refactoring

![After Refactoring - Adapter](UML/Adapter_After_Class.png)

------

#### 2.2.4 Refactoring Steps

To address the issues above, this project refactors the input system using the Adapter pattern as follows:

1. **Define a unified input interface**

Define the input handler interface `IInputHandler`, describing the behaviors expected by the player: move start, move stop, and functional actions.

```cpp
class IInputHandler {
public:
  virtual ~IInputHandler() = default;
  // Move start event
  virtual void onMoveStart(const std::string &direction) = 0;
  // Move stop event
  virtual void onMoveStop(const std::string &direction) = 0;
  // Action event (open backpack, map, etc.)
  virtual void onAction(const std::string &action) = 0;
};

/**
 * KeyboardInputAdapter - keyboard input adapter
 * Adapts Cocos2d keyboard events to the IInputHandler interface
 */
class KeyboardInputAdapter {
public:
  explicit KeyboardInputAdapter(IInputHandler *handler);

  void setupListener(cocos2d::Node *node);
  void removeListener();

private:
  IInputHandler *_handler;
  cocos2d::EventListenerKeyboard *_listener;

  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                    cocos2d::Event *event);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                     cocos2d::Event *event);

  std::string keyCodeToDirection(cocos2d::EventKeyboard::KeyCode keyCode);
  std::string keyCodeToAction(cocos2d::EventKeyboard::KeyCode keyCode);
};

/**
 * TouchInputAdapter - touch input adapter
 * Adapts touch button events to the IInputHandler interface
 */
class TouchInputAdapter {
public:
  explicit TouchInputAdapter(IInputHandler *handler);

  void createButtons(cocos2d::Node *parent, float centerX, float centerY,
                     float radius);

private:
  IInputHandler *_handler;

  cocos2d::ui::Button *_btnUp;
  cocos2d::ui::Button *_btnDown;
  cocos2d::ui::Button *_btnLeft;
  cocos2d::ui::Button *_btnRight;
  cocos2d::ui::Button *_btnBackpack;
  cocos2d::ui::Button *_btnMap;

  void setupDirectionButton(cocos2d::ui::Button *btn,
                            const std::string &direction);
  void setupActionButton(cocos2d::ui::Button *btn, const std::string &action);
};

#endif // __INPUT_ADAPTER_H__
```

2. **Implement concrete input adapters**

For different input methods, implement corresponding adapter classes to convert low-level input events into `IInputHandler` calls.

**(1) Keyboard input adapter `KeyboardInputAdapter`**: maps Cocos2d-x keyboard events to unified movement/action commands and forwards them to `IInputHandler`.

```cpp
class KeyboardInputAdapter {
public:
  explicit KeyboardInputAdapter(IInputHandler *handler);

  void setupListener(cocos2d::Node *node);
  void removeListener();

private:
  IInputHandler *_handler;
  cocos2d::EventListenerKeyboard *_listener;

  void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,
                    cocos2d::Event *event);
  void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
                     cocos2d::Event *event);

  std::string keyCodeToDirection(cocos2d::EventKeyboard::KeyCode keyCode);
  std::string keyCodeToAction(cocos2d::EventKeyboard::KeyCode keyCode);
};
```

------

**(2) Touch input adapter `TouchInputAdapter`**: converts touch button events into unified direction/action commands, avoiding large amounts of button logic inside the `Player` class.

```
class TouchInputAdapter {
public:
  explicit TouchInputAdapter(IInputHandler *handler);

  void createButtons(cocos2d::Node *parent, float centerX, float centerY,
                     float radius);

private:
  IInputHandler *_handler;

  cocos2d::ui::Button *_btnUp;
  cocos2d::ui::Button *_btnDown;
  cocos2d::ui::Button *_btnLeft;
  cocos2d::ui::Button *_btnRight;
  cocos2d::ui::Button *_btnBackpack;
  cocos2d::ui::Button *_btnMap;

  void setupDirectionButton(cocos2d::ui::Button *btn,
                            const std::string &direction);
  void setupActionButton(cocos2d::ui::Button *btn, const std::string &action);
};
```

The adapter is responsible for converting low-level input events into unified interface calls.

```cpp
_keyboardAdapter = new KeyboardInputAdapter(this);
_touchAdapter = new TouchInputAdapter(this);
```

3. **Refactor `Player` to use the unified interface**

The `Player` class no longer directly handles concrete input events. Instead, it implements `IInputHandler` and only processes unified input behaviors, simplifying the class structure.

```cpp
#include "Input/InputAdapter.h"

class Player : public cocos2d::Node, public IInputHandler {
private:
    KeyboardInputAdapter* _keyboardAdapter;
    TouchInputAdapter* _touchAdapter;

public:
    // IInputHandler implementation
    void onMoveStart(const std::string& dir) override;
    void onMoveStop(const std::string& dir) override;
    void onAction(const std::string& action) override;

    void initInputAdapters();
};
```

4. **Unify the input initialization flow**

Consolidate the previously scattered and verbose input initialization code into a single unified method, reducing duplicated logic in the `Player` class.

```cpp
void Player::initInputAdapters() {
    _keyboardAdapter = new KeyboardInputAdapter(this);
    _keyboardAdapter->setupListener(this);

    _touchAdapter = new TouchInputAdapter(this);
    _touchAdapter->createButtons(this, KEYS_CENTER_X, KEYS_CENTER_Y, KEYS_RADIUS);
}
```

------

#### 2.2.5 Improvements Achieved

By introducing the Adapter pattern, the input system is significantly improved in multiple aspects:

- **Eliminated duplicated code**: keyboard and touch input share unified input handling logic, greatly reducing duplication.
- **Simplified class structure**: the `Player` class no longer directly manages multiple input methods, making responsibilities clearer.
- **Improved extensibility**: adding a new input method only requires implementing a new adapter class without changing the `Player` core logic.
- **Reduced system coupling**: `Player` depends only on the input interface, not on specific input implementations.
- **Improved maintainability and consistency**: with unified input logic, behavior changes only need to be made once, avoiding inconsistencies.

------

## III. Refactoring with Behavioral Patterns

### 3.1 Refactoring with the State Pattern

#### 3.1.1 Pattern Overview

The **State Pattern** is a behavioral design pattern that lets an object change its behavior when its internal state changes, making it appear as if the object “switched classes”. By encapsulating state-specific behavior into independent state classes, it replaces complex conditionals with polymorphism and improves extensibility and maintainability.

**Pattern Structure**

| Role              | Responsibility                                               |
| ----------------- | ------------------------------------------------------------ |
| **State**         | Defines the state interface and declares behavior methods    |
| **ConcreteState** | Implements behaviors for a specific state                    |
| **Context**       | Holds the State object and delegates behavior to current state |

**Applicable Scenarios**

- Object behavior changes with state
- The implementation contains many conditionals to decide behavior
- State transitions are complex and expected to grow

#### 3.1.2 Problem Analysis

Before refactoring, player movement state management mainly had the following issues:

**Issue 1: Using boolean flags to manage state, with logic scattered**

- `Player` used multiple boolean variables to represent “moving” and directional movement, spreading state across fields and multiple methods.
- As new features are added, more flags and branches accumulate, increasing complexity rapidly.

```cpp
class Player {
private:
    bool _isMoving;
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _isMovingUp;
    bool _isMovingDown;
};
```

**Issue 2: Repeated conditionals and high maintenance cost**

- Direction checks, state switches, animation control, and movement updates were intertwined.
- Changing one behavior often required touching multiple condition blocks, increasing inconsistency risk.

```cpp
void Player::onMoveStart(const std::string& direction) {
    if (direction == "Left") _isMovingLeft = true;
    else if (direction == "Right") _isMovingRight = true;
    else if (direction == "Up") _isMovingUp = true;
    else if (direction == "Down") _isMovingDown = true;
    _isMoving = true;
}
```

**Issue 3: Adding new states requires modifying `Player` in multiple places**

- Adding new states (running/swimming/attacking) tends to require adding fields and modifying input handling and per-frame update logic in multiple places.
- This violates the Open-Closed Principle and increases regression risk.

##### Why Refactoring Is Necessary

In summary, the original approach implemented state using boolean flags and conditionals, leading to scattered logic, low readability, and high extension cost. With the State pattern, state behavior is moved into independent state classes and selected via polymorphism, so new states can be added by introducing new classes rather than modifying large conditional blocks.

------

#### 3.1.3 UML Class Diagram Comparison

##### Before Refactoring

![Before Refactoring - State](UML/State_Before_Class.png)

##### After Refactoring

![After Refactoring - State](UML/State_After_Class.png)

------

#### 3.1.4 Refactoring Steps

To address the issues above, this project refactors player state management using the State pattern as follows:

1. **Define the state interface `IPlayerState`**

Define a unified state interface that encapsulates state behavior and describes transition entry points through `handleMoveStart/handleMoveStop`.

```cpp
class IPlayerState {
public:
    virtual ~IPlayerState() = default;

    virtual void enter(Player* player) = 0;
    virtual void update(Player* player, float delta) = 0;
    virtual void exit(Player* player) = 0;

    virtual IPlayerState* handleMoveStart(Player* player,
                                          const std::string& direction) = 0;
    virtual IPlayerState* handleMoveStop(Player* player,
                                         const std::string& direction) = 0;
};
```

2. **Implement concrete state classes (`IdleState` / `WalkingState`)**

Split mutually exclusive states such as idle and walking into concrete state classes: `IdleState` handles non-moving behavior, while `WalkingState` handles movement and animation behavior.

```cpp
class IdleState : public IPlayerState {
public:
    static IdleState* getInstance();

    void enter(Player* player) override;
    void update(Player* player, float delta) override;
    void exit(Player* player) override;

    IPlayerState* handleMoveStart(Player* player, const std::string& direction) override;
    IPlayerState* handleMoveStop(Player* player, const std::string& direction) override;
};

class WalkingState : public IPlayerState {
public:
    static WalkingState* getInstance();

    void enter(Player* player) override;
    void update(Player* player, float delta) override;
    void exit(Player* player) override;

    IPlayerState* handleMoveStart(Player* player, const std::string& direction) override;
    IPlayerState* handleMoveStop(Player* player, const std::string& direction) override;
};
```

3. **Introduce `PlayerStateContext` to manage transitions**

Introduce a state context to hold the current state and centralize the enter/exit lifecycle calls, so state-switching logic does not spread across `Player`.

```cpp
class PlayerStateContext {
private:
    Player* _player;
    IPlayerState* _currentState;
public:
    PlayerStateContext();
    void setPlayer(Player* player);
    void changeState(IPlayerState* newState);

    void update(float delta);
    void handleMoveStart(const std::string& direction);
    void handleMoveStop(const std::string& direction);
};
```

4. **Delegate behavior to the context and state objects**

Delegate `onMoveStart/onMoveStop/update` to the context, so the current state decides behavior and transitions.

```cpp
void Player::update(float delta) {
    _stateContext.update(delta);
}

void Player::onMoveStart(const std::string& direction) {
    _stateContext.handleMoveStart(direction);
}

void Player::onMoveStop(const std::string& direction) {
    _stateContext.handleMoveStop(direction);
}
```

------

#### 3.1.5 Improvements Achieved

By introducing the State pattern, player state management is improved in multiple ways:

- **Fewer conditionals**: replaces large `if-else` blocks with polymorphism; state logic is centralized.
- **Clearer state representation**: state becomes an explicit object rather than scattered boolean flags.
- **Better extensibility**: new states can be added by introducing new state classes and wiring transitions.
- **Clearer responsibilities**: `Player` focuses on domain capabilities while `PlayerStateContext` centralizes switching.

------

### 3.2 Refactoring with the Observer Pattern

#### 3.2.1 Pattern Overview

The **Observer Pattern** is a behavioral design pattern that defines a one-to-many dependency between objects. When one object changes state, all of its dependents are notified and can update automatically. It is commonly used to build event-driven communication that reduces coupling between modules.

**Pattern Structure**

| Role                | Responsibility                                 |
| ------------------- | ---------------------------------------------- |
| **Subject**         | Maintains observer list and sends notifications |
| **Observer**        | Defines the update interface                    |
| **ConcreteObserver**| Implements concrete update behavior             |

**Applicable Scenarios**

- One change should notify multiple modules
- Modules should not call each other directly
- New responders should be addable without changing publishers

#### 3.2.2 Problem Analysis

Before refactoring, the game event handling logic had the following issues:

**Issue 1: Direct cross-component calls causing tight coupling**

- Modules called each other directly (scene ↔ player ↔ UI), making boundaries unclear.
- Publishers had to know concrete subscriber types and methods.

```cpp
void Player::addItemToBackpack(Item* item) {
    _backpack->addItem(item);
    if (_backpackScene) {
        _backpackScene->updateUI();
    }
}
```

**Issue 2: Adding new behaviors requires modifying multiple places**

- Adding a new response (audio, quest updates, analytics) often requires changing publisher logic.
- This creates chain edits and violates the Open-Closed Principle.

**Issue 3: Scattered event management, not conducive to system extension**

- Different events are handled in different scenes or objects, lacking a unified event management mechanism.
- As features grow, event dependency relationships become difficult to maintain.

##### Why Refactoring Is Necessary

Overall, the original event system suffered from tight coupling between event triggers and handlers, unclear class responsibilities, and insufficient extensibility. To improve maintainability and support future feature expansion, it is necessary to refactor the event communication mechanism.

By introducing a unified event hub, publishers only dispatch events without caring about concrete handling logic. Subscribers register as observers to events they care about, enabling decoupling and flexible extension. Therefore, it is well suited for event-driven communication in this project.

------

#### 3.2.3 UML Class Diagram Comparison

##### Before Refactoring

![Before Refactoring - Observer](UML/Observer_Before_Class.png)

##### After Refactoring

![After Refactoring - Observer](UML/Observer_After_Class.png)

------

#### 3.2.4 Refactoring Steps

To address the issues above, this project refactors the game event system using the Observer pattern as follows:

1. **Define the event object `GameEvent`**

Define a unified event payload type `GameEvent` with an extensible key-value data map, avoiding unsafe `void*` context passing.

```cpp
struct GameEvent {
    std::string type;
    std::unordered_map<std::string, std::string> data;

    GameEvent(const std::string& eventType) : type(eventType) {}
    void setData(const std::string& key, const std::string& value);
    std::string getData(const std::string& key) const;
};
```

2. **Define the observer interface `IEventObserver`**

Define a unified observer interface for receiving events through `onEvent`.

```cpp
class IEventObserver {
public:
    virtual ~IEventObserver() = default;
    virtual void onEvent(const GameEvent& event) = 0;
    virtual std::string getObserverName() const = 0;
};
```

3. **Implement the event manager `EventManager` (Subject)**

Introduce an event manager as a unified event center to handle observer registration, removal, and event notifications.
Internally, `EventManager` maintains a mapping from event types to observer lists.

```cpp
class EventManager {
public:
    static EventManager* getInstance();

    static const std::string EVENT_BACKPACK_UPDATE;
    static const std::string EVENT_PLAYER_MOVE;
    static const std::string EVENT_CROP_PLANT;
    static const std::string EVENT_CROP_HARVEST;
    static const std::string EVENT_NPC_INTERACT;

    void addObserver(const std::string& eventType, IEventObserver* observer);
    void removeObserver(const std::string& eventType, IEventObserver* observer);
    void dispatchEvent(const GameEvent& event);
};
```

4. **Replace direct calls with event dispatch**

Replace “call subscriber directly” with “dispatch event”, so subscribers decide how to react.

```cpp
void Player::addItem(Item* item) {
    _backpack->addItem(item);

    GameEvent event(EventManager::EVENT_BACKPACK_UPDATE);
    event.setData("item", item->getName());
    EventManager::getInstance()->dispatchEvent(event);
}
```

5. **Let concrete modules subscribe and respond**

Let UI, scenes, audio, and quest modules implement `IEventObserver` and subscribe to interested events, enabling one-to-many responses.

```cpp
class BackpackScene : public cocos2d::Scene, public IEventObserver {
public:
    void onEnter() override;
    void onEvent(const GameEvent& event) override;
    std::string getObserverName() const override;
};
```

------

#### 3.2.5 Improvements Achieved

By introducing the Observer pattern, the event system is significantly improved in multiple aspects:

- **Eliminated direct calls**: publishers and subscribers become independent, reducing coupling and cyclic dependencies.
- **Supports one-to-many reactions**: the same event can be handled by multiple modules (UI, audio, quest system, etc.).
- **Clearer event data**: `GameEvent` standardizes payload passing and avoids unsafe context.
- **Clearer boundaries**: communication is centralized in `EventManager`, simplifying extension and debugging.

------

## IV. Refactoring with Creational Patterns

### 4.1 Refactoring with the Singleton Pattern

#### 4.1.1 Pattern Overview

The **Singleton Pattern** is a creational design pattern that ensures a class has only one instance and provides a global access point to that instance. It is commonly used for globally unique resources or shared state managers (configuration, managers, resource hubs, etc.).

**Pattern Structure**

| Role          | Responsibility                                           |
| ------------- | -------------------------------------------------------- |
| **Singleton** | Provides `getInstance()` and controls unique instantiation |
| **Client**    | Accesses and uses the singleton through `getInstance()`  |

**Applicable Scenarios**

- A globally unique object is required (manager, configuration center)
- Shared state should be centralized rather than scattered global data
- Access to shared resources needs unified constraints and validation

#### 4.1.2 Problem Analysis

Before refactoring, parts of the game’s global state were scattered as `extern` variables across multiple files, causing the following issues:

**Issue 1: Scattered global variables and hidden dependencies**

- Variables such as `g_selectedMap` and `speed` were declared as `extern` in a header, then redeclared and used directly in multiple implementation files.
- Dependencies were not obvious, making it difficult to track and manage global state usage.

```cpp
// GlobalVars.h
extern std::string g_selectedMap;
extern float speed;
```

```cpp
// Player.cpp / GameScene.cpp, etc.
extern std::string g_selectedMap;
extern float speed;
```

**Issue 2: Large modification surface and high maintenance cost**

- Adding or changing global state often required updating declarations and usages across multiple files.
- This led to shotgun-surgery style changes and violated the Open-Closed Principle.

**Issue 3: No centralized access control and validation**

- Any module could write the variables directly, making it hard to enforce range checks, defaults, or reset logic.
- Invalid state became harder to prevent and debug.

##### Why Refactoring Is Necessary

Using `extern` global variables results in hidden dependencies, high maintenance cost, and poor control over state correctness. Refactoring to a Singleton centralizes state management behind a clear API, improves dependency visibility, and enables unified validation.

------

#### 4.1.3 UML Class Diagram Comparison

##### Before Refactoring

![Before Refactoring - Singleton](UML/Singleton_Before_Class.png)

##### After Refactoring

![After Refactoring - Singleton](UML/Singleton_After_Class.png)

------

#### 4.1.4 Refactoring Steps

To address the issues above, this project refactors global state management using the Singleton pattern as follows:

1. **Introduce the singleton manager `GameManager`**

Create `GameManager` as the single entry point for global state access. Expose state through getters/setters.

```cpp
class GameManager {
public:
    static GameManager* getInstance();

    void setCurrentMap(const std::string& mapPath);
    std::string getCurrentMap() const;

    void setPlayerSpeed(float speed);
    float getPlayerSpeed() const;

private:
    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    std::string _currentMap;
    float _playerSpeed;
};
```

2. **Move `extern` globals into `GameManager`**

Replace variables in `GlobalVars.h` with member fields inside `GameManager`, and set defaults and range validation where needed.

3. **Replace direct global access across modules**

Update `Player`, `GameScene`, `BackpackScene`, and `MapScene` to access global state through `GameManager::getInstance()`.

```cpp
std::string currentMap = GameManager::getInstance()->getCurrentMap();
float playerSpeed = GameManager::getInstance()->getPlayerSpeed();
```

4. **Centralize reset and debug logging (optional)**

Move reset-to-default and debug output logic into `GameManager` for unified maintenance and troubleshooting.

------

#### 4.1.5 Improvements Achieved

- **More explicit dependencies**: modules depend on a single, visible access point rather than hidden globals.
- **Lower maintenance cost**: adding or changing global state is localized to `GameManager`.
- **Controlled access**: setter validation reduces invalid state and improves robustness.
- **Clearer responsibilities**: business classes focus on domain logic; state management is centralized.

------

## V. Refactoring with Extra Pattern

### 5.1 Refactoring with the Object Pool Pattern

#### 5.1.1 Pattern Overview

The **Object Pool Pattern** is a creational design pattern that reduces performance overhead by pre-creating and reusing objects instead of creating and destroying them frequently. A pool “lends” objects to clients and reclaims them for reuse.

**Pattern Structure**

| Role           | Responsibility                                  |
| -------------- | ----------------------------------------------- |
| **ObjectPool** | Creates, lends, reclaims, and resets pooled objects |
| **Client**     | Acquires objects from the pool and releases them back |
| **PooledObject** | The object type being pooled and reused       |

**Applicable Scenarios**

- Objects are expensive to create/destroy (resource-heavy render objects)
- Object lifetimes are short but creation frequency is high (effects, crops)
- You want to reduce allocation spikes and memory churn

#### 5.1.2 Problem Analysis

Before refactoring, crop sprites were frequently created and destroyed during planting and harvesting, causing the following issues:

**Issue 1: Frequent object creation with performance overhead**

- Each planting operation created a new `Sprite` via `Sprite::create(...)`.
- During intensive interactions, this caused noticeable allocation pressure.

```cpp
void Map2Scene::plantCropAt(const Vec2& locationInMap) {
    auto crop = Sprite::create(cropImages[cropIndex]);
    _crops.push_back(crop);
}
```

**Issue 2: Frequent destruction causing memory churn**

- Harvesting removed sprites and erased them from containers, producing short lifetimes and frequent releases.
- This increases the risk of memory fragmentation and frame-time spikes.

```cpp
void Map2Scene::harvestCropAt(const Vec2& locationInMap) {
    crop->removeFromParent();
    _crops.erase(it);
}
```

##### Why Refactoring Is Necessary

In high-frequency interactions such as planting/harvesting, pooling sprites can reduce repeated allocation and deallocation, stabilize memory behavior, and improve performance consistency. Therefore, this project refactors crop sprite lifecycle management using the Object Pool pattern.

------

#### 5.1.3 UML Class Diagram Comparison

##### Before Refactoring

![Before Refactoring - Object Pool](UML/ObjectPool_Before_Class.png)

##### After Refactoring

![After Refactoring - Object Pool](UML/ObjectPool_After_Class.png)

------

#### 5.1.4 Refactoring Steps

To address the issues above, this project refactors crop sprite creation and reclamation using the Object Pool pattern as follows:

1. **Introduce the pool class `SpritePool`**

Create `SpritePool` to manage reusable `Sprite` instances grouped by image path, exposing `acquire/release` APIs.

```cpp
class SpritePool {
public:
    static SpritePool* getInstance();

    cocos2d::Sprite* acquire(const std::string& imagePath);
    void release(cocos2d::Sprite* sprite, const std::string& imagePath);

private:
    std::unordered_map<std::string, std::vector<cocos2d::Sprite*>> _pool;
};
```

2. **Acquire pooled objects when planting**

Replace `Sprite::create(...)` with `SpritePool::acquire(...)`, then initialize visibility and add to the scene.

```cpp
auto crop = SpritePool::getInstance()->acquire(cropImages[cropIndex]);
crop->setVisible(true);
_tiledMap->addChild(crop, 15);
_crops.push_back(crop);
```

3. **Release objects back to the pool when harvesting**

Replace “remove and destroy” with “reset and return”, so the object can be reused.

```cpp
SpritePool::getInstance()->release(crop, cropImages[cropIndex]);
_crops.erase(it);
```

4. **Reset object state consistently**

Centralize sprite reset logic inside `SpritePool` (position/scale/visibility/actions/children) to ensure reused objects do not leak old state.

------

#### 5.1.5 Improvements Achieved

- **Reduced allocation and deallocation**: reuse stabilizes memory and reduces churn in high-frequency interactions.
- **More stable runtime performance**: fewer creation/destruction spikes reduce frame-time variance.
- **Clearer responsibilities**: `SpritePool` manages lifecycle; scene logic stays focused on gameplay.
- **Easier monitoring and tuning**: pooling centralizes tracking of available/active objects for optimization.

------

## VI. AI Tool Assistance and Reflections

### 6.1 AI Tools Used

During the design pattern refactoring of this project, we used **Antigravity** (a VSCode extension integrating Gemini AI programming assistant) as an auxiliary tool. Antigravity can read project code directly within the IDE, analyze code structure, and generate or modify code based on our requirements.

It is important to emphasize that we positioned AI as an **auxiliary tool** rather than the decision-maker. All design decisions were made by team members through discussion; AI suggestions served only as references, and whether to adopt them was ultimately judged by us.

### 6.2 Specific Applications of AI Assistance

During the refactoring process, AI tools mainly provided help in the following areas:

#### 6.2.1 Code Problem Analysis

We provided existing code snippets to the AI and asked it to analyze the problems. For example, when analyzing the movement state management in `Player.cpp`, we found the code used multiple boolean variables (`_isMovingLeft`, `_isMovingRight`, etc.). The AI helped us identify this as a "scattered state management" code smell and suggested considering the State pattern for refactoring.

However, whether to adopt the State pattern and how to divide the state classes were decisions we made based on the actual project situation.

#### 6.2.2 Code Framework Generation

After determining the design, we asked the AI to generate code frameworks. For example, after deciding to create the `GameManager` singleton class, the AI helped generate the class header file and basic implementation. However, the generated code required our review and adjustment:

- Check whether Cocos2d-x APIs are correct (AI sometimes uses wrong versions)
- Add project-specific business logic
- Adjust code style to match project conventions

#### 6.2.3 Batch Code Modifications

When migrating global variables to the `GameManager` singleton, we needed to modify `extern` declarations and variable references across multiple files. The AI helped us quickly locate the places that needed modification and generate replacement code. This type of repetitive work was completed with AI assistance, saving significant time.

#### 6.2.4 Documentation and UML Diagram Generation

The AI assisted in generating the structure framework of project reports and PlantUML class diagram source code. However, the technical content in the reports required our verification for accuracy, and the UML diagrams also needed checking to ensure they correctly reflected the code structure.

### 6.3 Our Core Principles

Throughout the refactoring process, we adhered to the following principles:

**1. AI Provides Options, We Make Decisions**

The AI might suggest multiple design approaches, but choosing which one required judgment based on the actual project situation. For example, for the event system design, the AI suggested the Observer pattern, but the specific event type definitions, observer interface design, and other details were all determined by us based on game functionality requirements.

**2. Understand First, Then Use**

We did not directly use AI-generated code. Instead, we first understood its implementation logic and confirmed it aligned with design pattern principles before adopting it. If we did not understand a piece of code, we would look up resources or discuss it clearly rather than blindly using it.

### 6.4 Problems Encountered and Solutions

When using AI tools, we also encountered some problems:

| Problem | Specific Manifestation | Our Solution |
|---------|----------------------|--------------|
| API Version Differences | AI-generated code used non-existent Cocos2d-x methods | Consulted official documentation and corrected to proper APIs |
| Inconsistent Code Style | Generated naming and comment formats did not match project conventions | Manual adjustment to maintain consistent code style |
| Missing Edge Cases | Some special cases were not considered | Discovered during code review and added handling logic |
| Business Understanding Gaps | AI did not understand specific game mechanics, generating partially incorrect logic | Team members modified and refined based on business requirements |

These problems demonstrate that AI-generated code cannot be used directly and must go through careful review and testing.

### 6.5 Reflections and Summary

Through this project, we gained deeper insights into AI-assisted development.

Regarding the value of AI, we found that it can indeed accelerate repetitive work. For tasks like batch code modifications and framework code generation, AI can complete them quickly, saving significant time. At the same time, AI can provide design idea references, especially when we were not very familiar with certain design patterns—AI suggestions helped us understand the applicable scenarios and implementation approaches. Additionally, AI played a role in assisting with document and diagram generation, allowing us to focus more on core design and implementation work.

However, AI also has obvious limitations. It does not understand the overall architecture and business background of the project, and its suggestions are sometimes only locally optimal. AI sometimes uses incorrect APIs or outdated approaches, so generated code cannot be used directly. More importantly, AI cannot replace developers' understanding and judgment of code—final design decisions still need to be made by ourselves.

Through this practice, we also gained a lot. We learned how to reasonably utilize AI tools to improve development efficiency, recognized that AI is only an auxiliary means and cannot replace learning and thinking. At the same time, we also developed critical review capabilities for AI-generated content, no longer blindly trusting AI output.

In summary, we believe that **using AI reasonably but not depending on AI** is the right attitude. AI can help us complete work more efficiently, but design decisions, code quality, and the correctness of business logic are ultimately the responsibility of developers. This is also the principle we upheld throughout this project.

------

## VII. Additional Information and Project Reflections

This project was completed collaboratively by three team members, with each person responsible for refactoring two design patterns. Through this process, our greatest takeaway was truly understanding the value of design patterns—they exist not to show off technical skills, but to solve real problems. Before refactoring, our code ran fine, but making changes was painful: modifying one place meant having to change several others, and carelessness could easily introduce new bugs. After introducing design patterns, the code structure became clearer, and adding new features became simpler. We also learned that refactoring is not something that can be done all at once—it needs to be done step by step, changing only one module at a time, ensuring the game still runs properly after each change. In terms of team collaboration, we learned to communicate promptly and review each other's code, which not only helps find issues but also allows us to learn from each other's code. Overall, this project helped us take a step forward from "being able to write code" to "being able to write good code," and made us appreciate the importance of design and refactoring in software engineering. We thank our instructor for guidance and our team members for mutual support.

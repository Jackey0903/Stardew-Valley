# Stardew Valley Refactoring Project - Presentation Script

**Duration: Approximately 8 minutes**

---

## Opening (30 seconds)

Good afternoon, everyone. Today, I'm excited to present our team's project on **Design Pattern Refactoring for a Stardew Valley-inspired Game**. 

Our project demonstrates how we applied six classic design patterns to improve code quality, and how we reasonably utilized AI tools to assist our development process—while ensuring that all design decisions remained firmly in our hands.

---

## Part 1: Project Background (1.5 minutes)

Let me start with some background about our project.

We developed a **farm simulation game** inspired by popular titles like Stardew Valley and Harvest Moon. The game is built using **Cocos2d-x 3.17.2**, a widely-used open-source game engine, and written in **C++14**.

The core gameplay includes:
- **Farm management** – players can plant crops, raise animals, and harvest resources
- **Community interaction** – building relationships with town residents and completing quests
- **Exploration** – venturing into forests, mountains, and caves to gather rare materials
- **Character progression** – improving skills and managing equipment through a backpack system

However, as the codebase grew, we encountered several problems:
- **Code duplication** – similar logic was repeated across multiple classes
- **High coupling** – components were tightly interconnected, making changes risky
- **Poor extensibility** – adding new features required modifying many existing files
- **Scattered state management** – boolean flags and global variables were spread everywhere

These issues motivated us to undertake a systematic refactoring effort using design patterns.

---

## Part 2: Design Pattern Refactoring (4 minutes)

Our team of three students divided the work, each responsible for two design patterns. Let me walk you through what we implemented.

### Structural Patterns

**First, the Flyweight Pattern** for animation management.

Previously, our `Player` and `Animal` classes each loaded animation resources independently, creating duplicate `Animation` objects every time a character moved. This wasted memory and hurt performance.

We introduced an `AnimationFlyweight` factory class that caches and shares animation objects. Now, instead of creating new animations, characters retrieve shared instances from a central pool. This significantly reduced memory usage and improved frame rates during gameplay.

**Second, the Adapter Pattern** for input handling.

Our original code had separate implementations for keyboard and touch input, with nearly identical logic duplicated in both. The Player class was bloated with input-handling code.

We created a unified `IInputHandler` interface, then built `KeyboardInputAdapter` and `TouchInputAdapter` classes to convert platform-specific events into standardized commands. Now the Player class simply implements the interface, and adding new input methods—like gamepad support—only requires creating a new adapter.

### Behavioral Patterns

**Third, the State Pattern** for player movement.

The original implementation used five boolean variables to track whether the player was moving in each direction. The `update` function was filled with complex conditional checks.

We encapsulated these states into dedicated classes: `IdleState` and `WalkingState`, managed by a `PlayerStateContext`. State transitions are now explicit and handled through polymorphism. Adding new states like running or swimming would only require creating new state classes.

**Fourth, the Observer Pattern** for game events.

Previously, when something happened—like picking up an item—the Player class directly called methods on the BackpackScene to update the UI. This created tight coupling between unrelated components.

We implemented an `EventManager` as a central event hub. Publishers dispatch events without knowing who will handle them, and any component can subscribe to events they care about. This decoupling makes the system much more flexible and testable.

### Creational Patterns

**Fifth, the Singleton Pattern** for global state management.

We had `extern` global variables scattered across files for things like the current map and player speed. This made dependencies hidden and maintenance difficult.

We created a `GameManager` singleton that provides a single access point for global state, with proper getter and setter methods. All modules now access shared state through this centralized manager.

**Finally, the Object Pool Pattern** for sprite management.

During crop planting and harvesting, sprites were constantly created and destroyed, causing memory churn and occasional frame drops.

We implemented a `SpritePool` that maintains reusable sprite objects. When planting a crop, we acquire a sprite from the pool; when harvesting, we return it. This dramatically reduced allocation overhead and stabilized performance.

---

## Part 3: AI Tool Usage (1.5 minutes)

Now, let me discuss how we used AI tools in this project.

We used **Antigravity**, which is a VSCode extension that integrates Google's Gemini AI. It allowed us to interact with AI directly within our development environment.

Here's how we applied it:

**For code analysis**, we provided existing code snippets and asked the AI to identify problems. For example, it helped us recognize that our boolean-based state management was a code smell suitable for the State pattern.

**For framework generation**, after we decided on a design, we asked the AI to generate class skeletons. This saved time on boilerplate code.

**For batch modifications**, when migrating global variables to our singleton, the AI helped locate all the places that needed changes and generated replacement code.

**For documentation**, the AI assisted with report structure and UML diagram source code.

However, we want to emphasize our core principle: **AI provides options, we make decisions**.

We never used AI-generated code directly without review. We always verified that the logic was correct, the APIs existed in our Cocos2d-x version, and the code matched our project conventions. Several times, we found and fixed issues in the AI's output—incorrect API calls, missing edge cases, or style inconsistencies.

---

## Part 4: Reflections and Lessons Learned (30 seconds)

Through this project, we learned that AI tools can genuinely accelerate development—especially for repetitive tasks and getting started with unfamiliar patterns.

But AI has clear limitations. It doesn't understand our project's overall architecture or business requirements. It sometimes uses wrong APIs or produces code that doesn't quite fit.

Our key takeaway is this: **use AI reasonably, but don't depend on it**. AI is a powerful assistant, but design decisions, code quality, and correctness are ultimately our responsibility as developers.

---

## Closing (30 seconds)

In conclusion, our refactoring project successfully applied six design patterns—Flyweight, Adapter, State, Observer, Singleton, and Object Pool—to improve our game's code quality. We also gained valuable experience in using AI tools effectively while maintaining control over our development process.

Thank you for your attention. I'm glad to take any questions.

---

**Total estimated time: ~8 minutes**

**Word count: ~1,050 words**

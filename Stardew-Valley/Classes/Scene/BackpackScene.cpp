/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : BackpackScene.cpp
 * 文件功能      : 背包场景的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#include "BackpackScene.h"
#include "ui/CocosGUI.h" // 引入UI模块
USING_NS_CC;

extern float speed;  // 外部变量，用于表示移动速度

// 创建并返回背包场景
Scene* BackpackScene::createScene() {
    return BackpackScene::create();
}

// 初始化背包场景
bool BackpackScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 添加键盘事件监听器
    auto listener = EventListenerKeyboard::create();  // C++11特性：使用lambda函数作为回调
    listener->onKeyPressed = CC_CALLBACK_2(BackpackScene::onKeyPressed, this);  // 按键按下事件
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  // 为当前场景添加监听器

    _backpack = new Backpack();  // 初始化背包对象

    // 向背包中添加物品
    _backpack->addItem(new Item("斧子", "Axe.png", 1));
    _backpack->addItem(new Item("锄头", "Hoe.png", 1));
    _backpack->addItem(new Item("镐子", "Pickaxe.png", 1));
    _backpack->addItem(new Item("鱼竿", "Fishrod.png", 1));
    _backpack->addItem(new Item("烟花", "Fireworks.png", 1));
    _backpack->addItem(new Item("花朵", "Flower.png", 1));
    _backpack->addItem(new Item("草", "Grass.png", 1));
    _backpack->addItem(new Item("棍子", "Stick.png", 1));
    _backpack->addItem(new Item("石头", "Stone.png", 1));
    _backpack->addItem(new Item("木头", "Wood.png", 1));

    // 加载并设置背景图
    auto backgroundSprite = Sprite::create("background.png");  // C++11特性：使用智能指针（在Cocos2d中，资源管理通过智能指针）
    auto visibleSize = Director::getInstance()->getVisibleSize(); // 获取可见区域大小
    backgroundSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));  // 设置背景位置
    // 计算并设置背景缩放比例
    float scaleX = visibleSize.width / backgroundSprite->getContentSize().width;
    float scaleY = visibleSize.height / backgroundSprite->getContentSize().height;
    backgroundSprite->setScale(scaleX, scaleY);  // 进行缩放
    this->addChild(backgroundSprite, 0);  // 添加到场景中，层级为0（背景层）

    // 创建玩家精灵并设置位置
    _playerSprite = Sprite::create("Stand_Down.png");  // C++11特性：使用初始化列表
    _playerSprite->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height / 2));  // 设置精灵位置
    this->addChild(_playerSprite, 1);  // 添加玩家精灵到场景

    // 创建当前工具显示图标
    _currentToolSprite = Sprite::create("sword.png");  // 默认工具图像
    _currentToolSprite->setPosition(Vec2(135, 120));  // 设置位置
    _playerSprite->addChild(_currentToolSprite, 2);  // 将工具图标添加到玩家精灵下

    createMenu();  // 创建物品菜单
    createSkillTree();  // 创建技能树UI

    return true; 
}

// 创建物品菜单
void BackpackScene::createMenu() {
    auto backpack = getBackpack();
    if (!backpack) {
        CCLOG("背包未初始化或为空");
        return;
    }

    auto items = backpack->getItems();
    if (items.empty()) {
        CCLOG("背包中没有物品");
        return;
    }

    // 创建物品菜单
    auto menu = Menu::create(); 
    const int cols = 5;  
    const float padding = 10.0f; 
    Size itemSize(100, 100); 

    for (int i = 0; i < items.size(); ++i) {
        int row = i / cols; 
        int col = i % cols;

        // 计算物品在屏幕上的位置
        float x = 100 + col * (itemSize.width + padding); 
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float y = visibleSize.height - (row + 1) * (itemSize.height + padding); 

        // 创建菜单项，物品图标
        auto menuItem = MenuItemImage::create(
            items[i]->getImagePath(),
            items[i]->getImagePath(),
            [this, items, i](Ref* sender) {  // C++11特性：使用lambda函数处理事件
                CCLOG("点击了物品: %s", items[i]->getName().c_str());
                _selectedItem = items[i];  
                updatePlayerWithItem();  // 更新玩家手持物品
            });

        menuItem->setPosition(Vec2(x, y)); 
        menuItem->setScale(0.5f);

        menu->addChild(menuItem); 
    }

    menu->setPosition(Vec2::ZERO); 
    this->addChild(menu);  
}

// 创建技能树UI
void BackpackScene::createSkillTree() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建移动速度滑动条标签
    auto speedLabel = Label::createWithTTF("移动速度", "fonts/Marker Felt.ttf", 20);
    speedLabel->setPosition(Vec2(origin.x + visibleSize.width - 150, origin.y + 100));
    this->addChild(speedLabel, 40);

    // 创建移动速度滑动条
    _speedSlider = ui::Slider::create();
    _speedSlider->loadBarTexture("SliderBar.png");  
    _speedSlider->loadSlidBallTextures("Speed.png");  
    _speedSlider->loadProgressBarTexture("StartupLoadingBar.png");  
    _speedSlider->setPosition(Vec2(origin.x + visibleSize.width - 900, origin.y + 300));
    _speedSlider->setPercent(50);  // 设置默认百分比（对应速度为200）
    _speedSlider->addEventListener(CC_CALLBACK_2(BackpackScene::onSpeedSliderChanged, this));  // 事件监听
    this->addChild(_speedSlider, 40);

    // 创建人物大小滑动条标签
    auto sizeLabel = Label::createWithTTF("人物大小", "fonts/Marker Felt.ttf", 20);
    sizeLabel->setPosition(Vec2(origin.x + visibleSize.width - 300, origin.y + 100));
    this->addChild(sizeLabel, 40);

    // 创建人物大小滑动条
    _sizeSlider = ui::Slider::create();
    _sizeSlider->loadBarTexture("SliderBar.png"); 
    _sizeSlider->loadSlidBallTextures("Size.png"); 
    _sizeSlider->loadProgressBarTexture("StartupLoadingBar.png"); 
    _sizeSlider->setPosition(Vec2(origin.x + visibleSize.width - 900, origin.y + 100));
    _sizeSlider->setPercent(50);  // 设置默认百分比（对应人物大小1.0）
    _sizeSlider->addEventListener(CC_CALLBACK_2(BackpackScene::onSizeSliderChanged, this));  // 事件监听
    this->addChild(_sizeSlider, 40);
}

// 移动速度滑动条变化时的回调
void BackpackScene::onSpeedSliderChanged(Ref* sender, ui::Slider::EventType type) {
    if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        float percent = _speedSlider->getPercent();  // 获取滑动条百分比
        // 将百分比映射到100-300范围内
        speed = 100 + (percent / 100.0f) * 200;
        CCLOG("移动速度: %.2f", speed);
    }
}

// 人物大小滑动条变化时的回调
void BackpackScene::onSizeSliderChanged(Ref* sender, ui::Slider::EventType type) {
    if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        float percent = _sizeSlider->getPercent();  // 获取滑动条百分比
        // 将百分比映射到0.5-2.0范围内
        float size = 0.5f + (percent / 100.0f) * 1.5f;
        CCLOG("人物大小: %.2f", size);
        if (_playerSprite) {
            _playerSprite->setScale(size);  // 更新玩家精灵的大小
        }
    }
}

// 键盘按下事件，B键返回上一场景
void BackpackScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_B) {
        CCLOG("按下了 B 键");
        Director::getInstance()->popScene();  // 返回到上一场景
    }
}

// 获取背包对象
Backpack* BackpackScene::getBackpack() {
    return _backpack;  // 返回背包实例
}

// 更新玩家手持物品图标
void BackpackScene::updatePlayerWithItem() {
    if (_selectedItem) {
        // 更新玩家手上显示的物品图标
        _currentToolSprite->setTexture(_selectedItem->getImagePath());  // 更新物品图标
    }
}

/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : BackpackScene.cpp
 * 文件功能      : 背包场景的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/
#include "BackpackScene.h"

USING_NS_CC;

Scene* BackpackScene::createScene() {
    return BackpackScene::create();
}

bool BackpackScene::init() {
    if (!Scene::init()) {
        return false;
    }
    // 添加了键盘事件监听器
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(BackpackScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _backpack = new Backpack(); // 初始化背包 

    // 添加新的物品
    _backpack->addItem(new Item("斧子", "Axe.png", 1)); // 请确保图片路径正确
    _backpack->addItem(new Item("锄头", "Hoe.png", 1)); // 请确保图片路径正确
    _backpack->addItem(new Item("镐子", "Pickaxe.png", 1)); // 请确保图片路径正确
    _backpack->addItem(new Item("鱼竿", "Fishrod.png", 1)); // 请确保图片路径正确


   

    // 加载背景图
    auto backgroundSprite = Sprite::create("background.png"); // 替换为你的背景图文件名
    auto visibleSize = Director::getInstance()->getVisibleSize();
    backgroundSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    // 计算并设置背景缩放比例
    float scaleX = visibleSize.width / backgroundSprite->getContentSize().width;
    float scaleY = visibleSize.height / backgroundSprite->getContentSize().height;
    backgroundSprite->setScale(scaleX, scaleY); // 进行缩放

    // 将背景添加到场景中
    this->addChild(backgroundSprite, 0); // 添加在其他元素的下面（层级为0）


    // 创建角色精灵
    _playerSprite = Sprite::create("Stand_Down.png"); // 替换为你的角色图像
    _playerSprite->setPosition(Vec2(visibleSize.width * 0.75, visibleSize.height / 2)); // 右边位置
    this->addChild(_playerSprite, 1); // 添加到场景中

    // 创建当前手持物品显示
    _currentToolSprite = Sprite::create("sword.png"); // 替换为默认工具图像
    _currentToolSprite->setPosition(Vec2(visibleSize.width * 0.75+115, visibleSize.height / 2 +20)); // 角色上方位置
    this->addChild(_currentToolSprite, 2); // 添加到场景中

    createMenu(); // 创建物品网格
    return true;
}

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

    auto menu = Menu::create(); // 创建一个单独的菜单
    const int cols = 5;
    const float padding = 10.0f; // 间距
    Size itemSize(100, 100); // 假设每个物品的显示大小

    for (int i = 0; i < items.size(); ++i) {
        int row = i / cols;
        int col = i % cols;

        // 计算每个物品的显示位置
        float x = 100 + col * (itemSize.width + padding);
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float y = visibleSize.height - (row + 1) * (itemSize.height + padding);

        auto menuItem = MenuItemImage::create(
            items[i]->getImagePath(),
            items[i]->getImagePath(),
            [this, items, i](Ref* sender) {
                CCLOG("点击了物品: %s", items[i]->getName().c_str());
                _selectedItem = items[i]; // 保存选中的物品
                updatePlayerWithItem(); // 更新角色手上显示的物品
            });

        menuItem->setPosition(Vec2(x, y));
        menuItem->setScale(0.5f); // 根据需要缩放

        menu->addChild(menuItem); // 添加项到菜单
    }

    menu->setPosition(Vec2::ZERO); // 菜单的原点设置为 (0, 0)
    this->addChild(menu); // 添加到场景中
}


void BackpackScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_B) {
        CCLOG("按下了 B 键");
        Director::getInstance()->popScene(); // 返回到上一个场景
    }
}

Backpack* BackpackScene::getBackpack() {
    return _backpack; // 返回背包实例
}

void BackpackScene::updatePlayerWithItem() {
    if (_selectedItem) {
        // 更新角色手持物品图
        _currentToolSprite->setTexture(_selectedItem->getImagePath()); // 更新为选中的物品图像
    }
}

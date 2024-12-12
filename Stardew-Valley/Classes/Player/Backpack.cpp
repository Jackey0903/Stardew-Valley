/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Backpack.cpp
 * 文件功能      : 背包类实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/
#include "Backpack.h"
#include "cocos2d.h"

USING_NS_CC;

// 背包构造函数
Backpack::Backpack() {
    // 初始化背包，添加一个剑
    Item* sword = new Item("Sword", "sword.png", 1);
    addItem(sword);
}

// 析构函数，清理内存
Backpack::~Backpack() {
    for (auto& item : _items) {
        delete item; // 确保删除背包中的物品以释放内存
    }
    _items.clear(); // 清空物品列表
}

// 添加物品到背包
void Backpack::addItem(Item* item) {
    if (item) {
        _items.push_back(item);
    }
}

// 获取背包中的物品列表
const std::vector<Item*>& Backpack::getItems() const {
    return _items;
}

// 列出背包中的物品
void Backpack::listItems() const {
    for (const auto& item : _items) {
        CCLOG("物品: %s, 数量: %d", item->getName().c_str(), item->getQuantity());
    }
}

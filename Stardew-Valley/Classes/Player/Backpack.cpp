/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Backpack.cpp
 * 文件功能      : 背包类的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/
#include "Backpack.h"
#include "cocos2d.h"

// 可以根据需求在此进行其他必要的包含和使用声明
USING_NS_CC;

// 背包构造函数
Backpack::Backpack() {
    // 初始化背包，添加一个斧头和两个锄头
    Item* sword = new Item( "sword","sword.png",1);
    //Item* hoe1 = new Item("锄头", "hoe.png", 1); // 假设 hoe.png 是锄头的图像路径
    //Item* hoe2 = new Item("锄头", "hoe.png", 1); // 添加第二个锄头

    // 将物品添加到背包
    addItem(sword);
    //addItem(hoe1);
    //addItem(hoe2);
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



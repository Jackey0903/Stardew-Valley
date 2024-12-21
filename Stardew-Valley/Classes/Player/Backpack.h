/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Backpack.h
 * 文件功能      : 背包类头文件
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __BACKPACK_H__
#define __BACKPACK_H__

#include <vector>
#include "Item.h"  // 引入Item类头文件

 // 背包类，用于管理背包中的物品
class Backpack
{
public:
    // 构造函数，初始化背包
    Backpack();

    // 析构函数，释放背包中物品的资源
    ~Backpack();

    // 向背包中添加物品
    void addItem(Item* item);

    // 获取背包中所有物品的列表
    const std::vector<Item*>& getItems() const;

    // 列出背包中所有物品的信息
    void listItems() const;

private:
    std::vector<Item*> _items;
};

#endif // __BACKPACK_H__

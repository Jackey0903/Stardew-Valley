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
#include "Item.h"

class Backpack
{
public:
    Backpack();
    ~Backpack();

    void addItem(Item* item);
    const std::vector<Item*>& getItems() const;
    void listItems() const;

private:
    std::vector<Item*> _items;
};

#endif // __BACKPACK_H__

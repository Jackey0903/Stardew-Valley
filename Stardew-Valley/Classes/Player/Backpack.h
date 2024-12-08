/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Backpack.h
 * 文件功能      : 背包类的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/
#ifndef __BACKPACK_H__
#define __BACKPACK_H__

#include "cocos2d.h"
#include <vector>
#include <string>

class Item {
public:
    Item(const std::string& name, const std::string& imagePath, int quantity)
        : _name(name), _imagePath(imagePath), _quantity(quantity) {
    }

    std::string getName() const { return _name; }
    std::string getImagePath() const { return _imagePath; }
    int getQuantity() const { return _quantity; }

private:
    std::string _name;
    std::string _imagePath;
    int _quantity;
};

class Backpack {
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

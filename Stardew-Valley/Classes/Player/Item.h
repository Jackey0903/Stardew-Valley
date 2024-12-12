/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Item.h
 * 文件功能      : 物品类头文件
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/
#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>

class Item
{
public:
    Item(const std::string& name, const std::string& imagePath, int quantity);
    ~Item();

    std::string getName() const;
    std::string getImagePath() const;
    int getQuantity() const;

    void setQuantity(int quantity);

private:
    std::string _name;
    std::string _imagePath;
    int _quantity;
};

#endif // __ITEM_H__

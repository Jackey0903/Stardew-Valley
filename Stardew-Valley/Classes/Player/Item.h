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
    // 构造函数，初始化物品的名称、图片路径和数量
    Item(const std::string& name, const std::string& imagePath, int quantity);

    // 析构函数
    ~Item();

    // 获取物品名称
    std::string getName() const;

    // 获取物品的图片路径
    std::string getImagePath() const;

    // 获取物品的数量
    int getQuantity() const;

    // 设置物品的数量
    void setQuantity(int quantity);

private:
    std::string _name;

    std::string _imagePath;

    int _quantity;
};

#endif // __ITEM_H__

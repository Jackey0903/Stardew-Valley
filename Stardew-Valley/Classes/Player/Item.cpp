/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Item.cpp
 * 文件功能      : 物品类实现
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/06
 * 许可证        : MIT License
 ****************************************************************/

#include "Item.h"

// 获取物品名称
std::string Item::getName() const
{
    return _name;
}

// 获取物品图片路径
std::string Item::getImagePath() const
{
    return _imagePath;
}

// 获取物品数量
int Item::getQuantity() const
{
    return _quantity;
}

// 设置物品数量
void Item::setQuantity(int quantity)
{
    _quantity = quantity;
}

// 重载输出流操作符的实现
std::ostream& operator<<(std::ostream& os, const Item& item)
{
    os << "Item(Name: " << item.getName()
        << ", ImagePath: " << item.getImagePath()
        << ", Quantity: " << item.getQuantity() << ")";
    return os;
}
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
#include <iostream>

class Item {
public:
    Item(const std::string& name, const std::string& imagePath, int quantity)
        : _name(name), _imagePath(imagePath), _quantity(quantity) {
    }

    std::string getName() const;
    std::string getImagePath() const;
    int getQuantity() const;

    void setQuantity(int quantity);

    // 模板成员函数用于打印属性
    template <typename T>
    void printAttribute(const std::string& attributeName, const T& value) const {
        std::cout << "Attribute [" << attributeName << "] = " << value << std::endl;
    }

    // 嵌套的模板类
    template <typename T>
    class Tag {
    public:
        Tag(const std::string& key, const T& value)
            : _key(key), _value(value) {
        }

        void display() const {
            std::cout << "Tag [" << _key << "] = " << _value << std::endl;
        }

    private:
        std::string _key;
        T _value;
    };

    // 重载输出流操作符
    friend std::ostream& operator<<(std::ostream& os, const Item& item);

private:
    std::string _name;
    std::string _imagePath;
    int _quantity;
};

#endif  // __ITEM_H__

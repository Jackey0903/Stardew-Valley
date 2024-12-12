/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Item.cpp
 * �ļ�����      : ��Ʒ��ʵ��
 * ����          : ���ƽܣ����������ܽ�˶
 * ��������      : 2024/12/06
 * ���֤        : MIT License
 ****************************************************************/
#include "Item.h"

 // ���캯��
Item::Item(const std::string& name, const std::string& imagePath, int quantity)
    : _name(name), _imagePath(imagePath), _quantity(quantity)
{
}

// ��������
Item::~Item()
{
}

// ��ȡ��Ʒ����
std::string Item::getName() const
{
    return _name;
}

// ��ȡ��ƷͼƬ·��
std::string Item::getImagePath() const
{
    return _imagePath;
}

// ��ȡ��Ʒ����
int Item::getQuantity() const
{
    return _quantity;
}

// ������Ʒ����
void Item::setQuantity(int quantity)
{
    _quantity = quantity;
}

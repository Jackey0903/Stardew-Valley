/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Backpack.h
 * �ļ�����      : ������ͷ�ļ�
 * ����          : ���ƽ�
 * ��������      : 2024/12/06
 * ���֤        : MIT License
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

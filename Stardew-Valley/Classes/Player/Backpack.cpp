/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Backpack.cpp
 * �ļ�����      : ������ʵ��
 * ����          : ���ƽ�
 * ��������      : 2024/12/06
 * ���֤        : MIT License
 ****************************************************************/
#include "Backpack.h"
#include "cocos2d.h"

USING_NS_CC;

// �������캯��
Backpack::Backpack() {
    // ��ʼ�����������һ����
    Item* sword = new Item("Sword", "sword.png", 1);
    addItem(sword);
}

// ���������������ڴ�
Backpack::~Backpack() {
    for (auto& item : _items) {
        delete item; // ȷ��ɾ�������е���Ʒ���ͷ��ڴ�
    }
    _items.clear(); // �����Ʒ�б�
}

// �����Ʒ������
void Backpack::addItem(Item* item) {
    if (item) {
        _items.push_back(item);
    }
}

// ��ȡ�����е���Ʒ�б�
const std::vector<Item*>& Backpack::getItems() const {
    return _items;
}

// �г������е���Ʒ
void Backpack::listItems() const {
    for (const auto& item : _items) {
        CCLOG("��Ʒ: %s, ����: %d", item->getName().c_str(), item->getQuantity());
    }
}

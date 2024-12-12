/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Item.h
 * �ļ�����      : ��Ʒ��ͷ�ļ�
 * ����          : ���ƽܣ����������ܽ�˶
 * ��������      : 2024/12/06
 * ���֤        : MIT License
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

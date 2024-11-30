#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include <string>
#include <vector>

USING_NS_CC;

class Player {
public:
    // ���캯������������
    Player();
    ~Player();

    // �������
    std::string getName() const;
    void setName(const std::string& name);

    int getHealth() const;
    void setHealth(int health);

    int getEnergy() const;
    void setEnergy(int energy);

    int getMoney() const;
    void setMoney(int money);

    // ��������
    void addItem(const std::string& item);
    void removeItem(const std::string& item);
    const std::vector<std::string>& getInventory() const;

    // ũ������
    void plantCrop(const std::string& cropType);
    void feedAnimal(const std::string& animalType);

    // �ƶ����
    void move(float deltaX, float deltaY);
    Vec2 getPosition() const; // ��ȡ���λ��

    // �������״̬
    void update(float deltaTime);

private:
    std::string _name;                  // �������
    int _health;                        // ��ҽ���ֵ
    int _energy;                        // �������ֵ
    int _money;                         // ��ҽ�Ǯ
    std::vector<std::string> _inventory; // ��������
    Vec2 _position;                     // ���λ��
};

#endif // __PLAYER_H__


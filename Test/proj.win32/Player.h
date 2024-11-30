#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include <string>
#include <vector>

USING_NS_CC;

class Player {
public:
    // 构造函数和析构函数
    Player();
    ~Player();

    // 玩家属性
    std::string getName() const;
    void setName(const std::string& name);

    int getHealth() const;
    void setHealth(int health);

    int getEnergy() const;
    void setEnergy(int energy);

    int getMoney() const;
    void setMoney(int money);

    // 背包管理
    void addItem(const std::string& item);
    void removeItem(const std::string& item);
    const std::vector<std::string>& getInventory() const;

    // 农场交互
    void plantCrop(const std::string& cropType);
    void feedAnimal(const std::string& animalType);

    // 移动相关
    void move(float deltaX, float deltaY);
    Vec2 getPosition() const; // 获取玩家位置

    // 更新玩家状态
    void update(float deltaTime);

private:
    std::string _name;                  // 玩家名字
    int _health;                        // 玩家健康值
    int _energy;                        // 玩家能量值
    int _money;                         // 玩家金钱
    std::vector<std::string> _inventory; // 背包数组
    Vec2 _position;                     // 玩家位置
};

#endif // __PLAYER_H__


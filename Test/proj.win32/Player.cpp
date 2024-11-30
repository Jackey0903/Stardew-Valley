#include "Player.h"

// 构造函数初始化默认值
Player::Player()
    : _name("Player"), _health(100), _energy(100), _money(1000), _position(0, 0) { // 初始化位置为 (0,0)
}

// 析构函数
Player::~Player() {}

// 获取玩家名字
std::string Player::getName() const {
    return _name;
}

// 设置玩家名字
void Player::setName(const std::string& name) {
    _name = name;
}

// 获取玩家健康值
int Player::getHealth() const {
    return _health;
}

// 设置玩家健康值
void Player::setHealth(int health) {
    _health = health;
}

// 获取玩家能量值
int Player::getEnergy() const {
    return _energy;
}

// 设置玩家能量值
void Player::setEnergy(int energy) {
    _energy = energy;
}

// 获取玩家金钱
int Player::getMoney() const {
    return _money;
}

// 设置玩家金钱
void Player::setMoney(int money) {
    _money = money;
}

// 添加物品到背包
void Player::addItem(const std::string& item) {
    _inventory.push_back(item);
}

// 从背包中移除物品
void Player::removeItem(const std::string& item) {
    auto it = std::remove(_inventory.begin(), _inventory.end(), item);
    if (it != _inventory.end()) {
        _inventory.erase(it, _inventory.end());
    }
}

// 获取背包中的物品
const std::vector<std::string>& Player::getInventory() const {
    return _inventory;
}

// 种植作物
void Player::plantCrop(const std::string& cropType) {
    // 实现种植逻辑
}

// 喂养动物
void Player::feedAnimal(const std::string& animalType) {
    // 实现喂养逻辑
}

// 移动玩家
void Player::move(float deltaX, float deltaY) {
    _position.x += deltaX;
    _position.y += deltaY;

    // 根据需要可以添加边界检测，防止玩家移出可见区域
}

// 获取玩家位置
Vec2 Player::getPosition() const {
    return _position;
}

// 更新玩家状态
void Player::update(float deltaTime) {
    // 例如，消耗能量或其他状态变化
    _energy -= deltaTime * 1; // 简单示例：每秒消耗1点能量
}


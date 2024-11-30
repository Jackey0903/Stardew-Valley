#include "Player.h"

// ���캯����ʼ��Ĭ��ֵ
Player::Player()
    : _name("Player"), _health(100), _energy(100), _money(1000), _position(0, 0) { // ��ʼ��λ��Ϊ (0,0)
}

// ��������
Player::~Player() {}

// ��ȡ�������
std::string Player::getName() const {
    return _name;
}

// �����������
void Player::setName(const std::string& name) {
    _name = name;
}

// ��ȡ��ҽ���ֵ
int Player::getHealth() const {
    return _health;
}

// ������ҽ���ֵ
void Player::setHealth(int health) {
    _health = health;
}

// ��ȡ�������ֵ
int Player::getEnergy() const {
    return _energy;
}

// �����������ֵ
void Player::setEnergy(int energy) {
    _energy = energy;
}

// ��ȡ��ҽ�Ǯ
int Player::getMoney() const {
    return _money;
}

// ������ҽ�Ǯ
void Player::setMoney(int money) {
    _money = money;
}

// �����Ʒ������
void Player::addItem(const std::string& item) {
    _inventory.push_back(item);
}

// �ӱ������Ƴ���Ʒ
void Player::removeItem(const std::string& item) {
    auto it = std::remove(_inventory.begin(), _inventory.end(), item);
    if (it != _inventory.end()) {
        _inventory.erase(it, _inventory.end());
    }
}

// ��ȡ�����е���Ʒ
const std::vector<std::string>& Player::getInventory() const {
    return _inventory;
}

// ��ֲ����
void Player::plantCrop(const std::string& cropType) {
    // ʵ����ֲ�߼�
}

// ι������
void Player::feedAnimal(const std::string& animalType) {
    // ʵ��ι���߼�
}

// �ƶ����
void Player::move(float deltaX, float deltaY) {
    _position.x += deltaX;
    _position.y += deltaY;

    // ������Ҫ������ӱ߽��⣬��ֹ����Ƴ��ɼ�����
}

// ��ȡ���λ��
Vec2 Player::getPosition() const {
    return _position;
}

// �������״̬
void Player::update(float deltaTime) {
    // ���磬��������������״̬�仯
    _energy -= deltaTime * 1; // ��ʾ����ÿ������1������
}


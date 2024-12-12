#include "Crop.h"

USING_NS_CC;

Crop::Crop(const std::string& cropType, const Vec2& position)
    : _cropType(cropType), _position(position), _growthTime(5.0f), _elapsedTime(0.0f), _state(CropState::PLANTED) {
}

bool Crop::init() {
    _cropSprite = Sprite::create(_cropType + "_seedling.png"); // 初始种子图像
    _cropSprite->setPosition(_position);
    this->addChild(_cropSprite);

    schedule([=](float dt) { update(dt); }, "crop_update_key"); // 注册更新函数
    return true;
}

void Crop::update(float dt) {
    if (_state == CropState::PLANTED) {
        _elapsedTime += dt;

        if (_elapsedTime >= _growthTime) {
            _state = CropState::HARVESTABLE;
            _cropSprite->setTexture(_cropType + "_ripe.png"); // 更新到成熟图像
        }
    }
}

void Crop::harvest() {
    if (_state == CropState::HARVESTABLE) {
        // 在此处添加收获的逻辑，例如给玩家增加物品
        _state = CropState::DEAD;
        _cropSprite->removeFromParent(); // 收获后移除作物
    }
}

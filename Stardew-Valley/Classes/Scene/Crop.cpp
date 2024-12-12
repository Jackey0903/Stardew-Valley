#include "Crop.h"

USING_NS_CC;

Crop::Crop(const std::string& cropType, const Vec2& position)
    : _cropType(cropType), _position(position), _growthTime(5.0f), _elapsedTime(0.0f), _state(CropState::PLANTED) {
}

bool Crop::init() {
    _cropSprite = Sprite::create(_cropType + "_seedling.png"); // ��ʼ����ͼ��
    _cropSprite->setPosition(_position);
    this->addChild(_cropSprite);

    schedule([=](float dt) { update(dt); }, "crop_update_key"); // ע����º���
    return true;
}

void Crop::update(float dt) {
    if (_state == CropState::PLANTED) {
        _elapsedTime += dt;

        if (_elapsedTime >= _growthTime) {
            _state = CropState::HARVESTABLE;
            _cropSprite->setTexture(_cropType + "_ripe.png"); // ���µ�����ͼ��
        }
    }
}

void Crop::harvest() {
    if (_state == CropState::HARVESTABLE) {
        // �ڴ˴�����ջ���߼�����������������Ʒ
        _state = CropState::DEAD;
        _cropSprite->removeFromParent(); // �ջ���Ƴ�����
    }
}

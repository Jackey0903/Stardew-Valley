#ifndef __CROP_H__
#define __CROP_H__

#include "cocos2d.h"

USING_NS_CC;

class Crop : public Node {
public:
    enum class CropState {
        PLANTED,
        GROWING,
        HARVESTABLE,
        DEAD
    };

    Crop(const std::string& cropType, const Vec2& position);

    virtual bool init();
    void update(float dt); // ���ڸ�������״̬
    void harvest(); // �ջ�����
    CropState getState() const { return _state; }
    // ���Ĭ�Ϲ��캯��
    Crop() : _cropType(""), _position(Vec2::ZERO), _state(CropState::PLANTED), _growthTime(0), _elapsedTime(0), _cropSprite(nullptr) {}

    CREATE_FUNC(Crop);

private:
    std::string _cropType; // ��������
    Vec2 _position; // ����λ��
    CropState _state; // ��ǰ״̬
    float _growthTime; // ��������ʱ��
    float _elapsedTime; // ������ʱ��

    Sprite* _cropSprite; // ����ľ���
};

#endif // __CROP_H__

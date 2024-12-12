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
    void update(float dt); // 用于更新作物状态
    void harvest(); // 收获作物
    CropState getState() const { return _state; }
    // 添加默认构造函数
    Crop() : _cropType(""), _position(Vec2::ZERO), _state(CropState::PLANTED), _growthTime(0), _elapsedTime(0), _cropSprite(nullptr) {}

    CREATE_FUNC(Crop);

private:
    std::string _cropType; // 作物种类
    Vec2 _position; // 作物位置
    CropState _state; // 当前状态
    float _growthTime; // 生长所需时间
    float _elapsedTime; // 已生长时间

    Sprite* _cropSprite; // 作物的精灵
};

#endif // __CROP_H__

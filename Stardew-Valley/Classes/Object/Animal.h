#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "cocos2d.h"

class Animal : public cocos2d::Node
{
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    static Animal* create(const cocos2d::Rect& area,const std::string _name);
    virtual bool init(const cocos2d::Rect& area);

    void setMovementSpeed(float speed) { _speed = speed; }
    void update(float dt) override;
private:
    cocos2d::Sprite* _animalSprite = nullptr;
    cocos2d::Rect _moveArea;
    Direction _currentDir;
    float _speed;
    float _changeDirTime; // 下次改变方向的时间计时
	std::string animalName;
    void loadAnimations();
    void startWalkingAnimation(Direction dir);
    void stopWalkingAnimation();
    void chooseRandomDirection();
    bool isOutOfArea(const cocos2d::Vec2& pos);
};

#endif // __ANIMAL_H__

#include "Animal.h"
USING_NS_CC;

Animal* Animal::create(const cocos2d::Rect& area)
{
    Animal* ret = new(std::nothrow) Animal();
    if (ret && ret->init(area))
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Animal::init(const cocos2d::Rect& area)
{
    if (!Node::init())
        return false;

    _moveArea = area;
    _speed = 50.0f; // �ƶ��ٶȣ�����/��
    _changeDirTime = 0.0f;

    // ���ض�����Դ
    loadAnimations();

    // �������ﾫ�鲢���ڷ�Χ�ڵ����λ��
    _animalSprite = Sprite::create("Animal/Cow_Stand_Down.png"); // ��ʼ��ͼ���ɻ���Idle��ͼ
	_animalSprite->setScale(0.2f);
    this->addChild(_animalSprite);

    float startX = RandomHelper::random_real(area.getMinX(), area.getMaxX());
    float startY = RandomHelper::random_real(area.getMinY(), area.getMaxY());
    _animalSprite->setPosition(Vec2(startX, startY));

    // ��ʼ�������
    chooseRandomDirection();

    this->scheduleUpdate();

    return true;
}

void Animal::loadAnimations()
{
    // �����ѽ� Animal_Up.plist, Animal_Down.plist, Animal_Left.plist, Animal_Right.plist ����Resources��
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animal/Cow_Up.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animal/Cow_Down.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animal/Cow_Left.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Animal/Cow_Right.plist");
}

void Animal::chooseRandomDirection()
{
    int d = RandomHelper::random_int(0, 3);
    _currentDir = static_cast<Direction>(d);
    startWalkingAnimation(_currentDir);
    // �´θı䷽��ļ��,���1~3��
    _changeDirTime = RandomHelper::random_real(1.0f, 3.0f);
}

void Animal::startWalkingAnimation(Direction dir)
{
    Vector<SpriteFrame*> frames;
    std::string directionStr;
    switch (dir)
    {
        case Direction::UP:
            directionStr = "Cow_Up_";
            break;
        case Direction::DOWN:
            directionStr = "Cow_Down_";
            break;
        case Direction::LEFT:
            directionStr = "Cow_Left_";
            break;
        case Direction::RIGHT:
            directionStr = "Cow_Right_";
            break;
    }

    frames.clear();
    for (int i = 1; i <= 2; i++)
    {
        std::string frameName = directionStr + std::to_string(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
            frames.pushBack(frame);
        else
            CCLOG("Frame not found: %s", frameName.c_str());
    }

    if (!frames.empty())
    {
        auto anim = Animation::createWithSpriteFrames(frames, 0.2f);
        auto animate = Animate::create(anim);
        _animalSprite->stopAllActions();
        _animalSprite->runAction(RepeatForever::create(animate));
    }
    else
    {
        CCLOG("No frames found for direction %s", directionStr.c_str());
        // ������ΪStand��ͼ
    }
}


void Animal::stopWalkingAnimation()
{
    _animalSprite->stopAllActions();
    // ���л��ɾ�ֹ��ͼ����"Stand_Down.png"
}

bool Animal::isOutOfArea(const Vec2& pos)
{
    return !_moveArea.containsPoint(pos);
}

void Animal::update(float dt)
{
    if (!_animalSprite) return;

    Vec2 pos = _animalSprite->getPosition();
    // ���ݵ�ǰ�����ƶ�
    Vec2 velocity(0, 0);
    switch (_currentDir)
    {
        case Direction::UP:
            velocity.y = _speed;
            break;
        case Direction::DOWN:
            velocity.y = -_speed;
            break;
        case Direction::LEFT:
            velocity.x = -_speed;
            break;
        case Direction::RIGHT:
            velocity.x = _speed;
            break;
    }

    Vec2 newPos = pos + velocity * dt;

    // �����λ�ó��˷�Χ�򵽴ﻻ�����ʱ�䣬��ı䷽��
    _changeDirTime -= dt;
    if (isOutOfArea(newPos) || _changeDirTime <= 0.0f)
    {
        // ���ֵ�ǰpos��������Χ
        newPos.x = clampf(newPos.x, _moveArea.getMinX(), _moveArea.getMaxX());
        newPos.y = clampf(newPos.y, _moveArea.getMinY(), _moveArea.getMaxY());
        _animalSprite->setPosition(newPos);

        // �ı䷽��
        chooseRandomDirection();
    }
    else
    {
        _animalSprite->setPosition(newPos);
    }
}

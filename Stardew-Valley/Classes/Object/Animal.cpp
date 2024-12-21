#include "Animal.h"
USING_NS_CC;
using std::string;
Animal* Animal::create(const cocos2d::Rect& area,const std::string _name)
{
    Animal* ret = new(std::nothrow) Animal();
	ret->animalName = _name;
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
    _speed = 50.0f; // 移动速度，像素/秒
    _changeDirTime = 0.0f;

    // 加载动画资源
    loadAnimations();

    // 创建动物精灵并放在范围内的随机位置
    _animalSprite = Sprite::create(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Stand_Down.png")); // 初始贴图，可换成Idle贴图
	_animalSprite->setScale(0.2f);
    this->addChild(_animalSprite);

    float startX = RandomHelper::random_real(area.getMinX(), area.getMaxX());
    float startY = RandomHelper::random_real(area.getMinY(), area.getMaxY());
    _animalSprite->setPosition(Vec2(startX, startY));

    // 初始方向随机
    chooseRandomDirection();

    this->scheduleUpdate();

    return true;
}

void Animal::loadAnimations()
{
    // 假设已将 Animal_Up.plist, Animal_Down.plist, Animal_Left.plist, Animal_Right.plist 放入Resources中
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Down.plist"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Up.plist"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Left.plist"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Right.plist"));
}

void Animal::chooseRandomDirection()
{
    int d = RandomHelper::random_int(0, 3);
    _currentDir = static_cast<Direction>(d);
    startWalkingAnimation(_currentDir);
    // 下次改变方向的间隔,随机1~3秒
    _changeDirTime = RandomHelper::random_real(1.0f, 3.0f);
}

void Animal::startWalkingAnimation(Direction dir)
{
    Vector<SpriteFrame*> frames;
    std::string directionStr;
    switch (dir)
    {
        case Direction::UP:
            directionStr = animalName + static_cast<string>("_Up_");
            break;
        case Direction::DOWN:
            directionStr = animalName + static_cast<string>("_Down_");
            break;
        case Direction::LEFT:
            directionStr = animalName + static_cast<string>("_Left_");
            break;
        case Direction::RIGHT:
            directionStr = animalName + static_cast<string>("_Right_");
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
        // 可设置为Stand贴图
    }
}


void Animal::stopWalkingAnimation()
{
    _animalSprite->stopAllActions();
    // 可切换成静止贴图，如"Stand_Down.png"
}

bool Animal::isOutOfArea(const Vec2& pos)
{
    return !_moveArea.containsPoint(pos);
}

void Animal::update(float dt)
{
    if (!_animalSprite) return;

    Vec2 pos = _animalSprite->getPosition();
    // 根据当前方向移动
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

    // 如果新位置出了范围或到达换方向的时间，则改变方向
    _changeDirTime -= dt;
    if (isOutOfArea(newPos) || _changeDirTime <= 0.0f)
    {
        // 保持当前pos不超出范围
        newPos.x = clampf(newPos.x, _moveArea.getMinX(), _moveArea.getMaxX());
        newPos.y = clampf(newPos.y, _moveArea.getMinY(), _moveArea.getMaxY());
        _animalSprite->setPosition(newPos);

        // 改变方向
        chooseRandomDirection();
    }
    else
    {
        _animalSprite->setPosition(newPos);
    }
}

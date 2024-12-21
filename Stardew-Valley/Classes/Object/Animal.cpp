/****************************************************************
  * 项目名        : Stardew-Valley
  * 文件名        : Animal.cpp
  * 文件功能      : Animal类的实现
  * 作者          : 曹津硕、胡浩杰
  * 更新日期      : 2024/12/17
  * 许可证        : MIT License
****************************************************************/

#include "Animal.h"
// 使用 cocos2d 命名空间
USING_NS_CC;
// 使用标准命名空间中的 string
using std::string;

// 创建 Animal 对象的静态方法
Animal* Animal::create(const cocos2d::Rect& area, const std::string _name)
{
    // 使用 nothrow 操作符防止内存分配异常
    Animal* ret = new(std::nothrow) Animal();
    // 设置动物的名称
    ret->animalName = _name;
    // 初始化对象
    if (ret && ret->init(area))
    {
        // 设置为自动释放
        ret->autorelease();
        return ret;
    }
    // 如果初始化失败，释放内存并返回空指针
    delete ret;
    return nullptr;
}

// 初始化 Animal 对象
bool Animal::init(const cocos2d::Rect& area)
{
    // 调用父类的初始化方法
    if (!Node::init())
        return false;

    _moveArea = area;
    _speed = 50.0f;
    _changeDirTime = 0.0f;

    // 加载动画资源
    loadAnimations();

    // 创建动物精灵并放置在移动区域内的随机位置
    _animalSprite = Sprite::create(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Stand_Down.png"));
    _animalSprite->setScale(0.2f);
    this->addChild(_animalSprite);

    // 随机设置精灵的初始位置
    float startX = RandomHelper::random_real(area.getMinX(), area.getMaxX());
    float startY = RandomHelper::random_real(area.getMinY(), area.getMaxY());
    _animalSprite->setPosition(Vec2(startX, startY));

    // 随机选择初始移动方向
    chooseRandomDirection();

    // 注册更新方法
    this->scheduleUpdate();

    return true;
}

// 加载动物的动画资源
void Animal::loadAnimations()
{
    // 加载各个方向的动画帧缓存
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Down.plist"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Up.plist"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Left.plist"));
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(static_cast<string>("Animal/") + animalName + static_cast<string>("/") + animalName + static_cast<string>("_Right.plist"));
}

// 随机选择一个移动方向
void Animal::chooseRandomDirection()
{
    // 随机生成 0 到 3 之间的整数，对应四个方向
    int d = RandomHelper::random_int(0, 3);
    _currentDir = static_cast<Direction>(d);
    // 开始对应方向的行走动画
    startWalkingAnimation(_currentDir);
    // 随机设置下次改变方向的时间间隔，范围为 1 到 3 秒
    _changeDirTime = RandomHelper::random_real(1.0f, 3.0f);
}

// 开始行走动画
void Animal::startWalkingAnimation(Direction dir)
{
    // 创建一个存储动画帧的向量
    Vector<SpriteFrame*> frames;
    // 定义方向的字符串名称
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
    // 加载对应方向的动画帧
    for (int i = 1; i <= 2; i++)
    {
        // 构造帧的名称
        std::string frameName = directionStr + std::to_string(i) + ".png";
        // 从缓存中获取 SpriteFrame
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
            frames.pushBack(frame);
        else
            CCLOG("未找到帧：%s", frameName.c_str());
    }

    // 如果成功加载了动画帧
    if (!frames.empty())
    {
        // 创建动画，帧间隔为 0.2 秒
        auto anim = Animation::createWithSpriteFrames(frames, 0.2f);
        // 创建动画动作
        auto animate = Animate::create(anim);
        // 停止精灵的所有动作
        _animalSprite->stopAllActions();
        // 运行循环的动画动作
        _animalSprite->runAction(RepeatForever::create(animate));
    }
    else
    {
        CCLOG("方向 %s 未找到任何帧", directionStr.c_str());
    }
}

// 停止行走动画
void Animal::stopWalkingAnimation()
{
    // 停止精灵的所有动作
    _animalSprite->stopAllActions();
}

// 判断位置是否超出移动区域
bool Animal::isOutOfArea(const Vec2& pos)
{
    // 如果位置不在移动区域内，返回 true
    return !_moveArea.containsPoint(pos);
}

// 每帧更新方法
void Animal::update(float dt)
{
    // 如果精灵不存在，直接返回
    if (!_animalSprite) return;

    // 获取当前精灵的位置
    Vec2 pos = _animalSprite->getPosition();
    // 根据当前方向设置速度
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

    // 计算新的位置
    Vec2 newPos = pos + velocity * dt;

    // 减少改变方向的剩余时间
    _changeDirTime -= dt;
    // 如果新位置超出移动区域，或者到达了改变方向的时间
    if (isOutOfArea(newPos) || _changeDirTime <= 0.0f)
    {
        // 将位置限制在移动区域内
        newPos.x = clampf(newPos.x, _moveArea.getMinX(), _moveArea.getMaxX());
        newPos.y = clampf(newPos.y, _moveArea.getMinY(), _moveArea.getMaxY());
        // 更新精灵位置
        _animalSprite->setPosition(newPos);

        // 随机选择新的移动方向
        chooseRandomDirection();
    }
    else
    {
        // 更新精灵位置
        _animalSprite->setPosition(newPos);
    }
}
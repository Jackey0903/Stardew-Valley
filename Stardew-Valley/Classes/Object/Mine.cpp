// Mine.cpp
#include "Mine.h"

USING_NS_CC;

Mine* Mine::createWithPosition(const Vec2& position, const std::string& spriteFrameName)
{
    Mine* ret = new(std::nothrow) Mine();
    if (ret && ret->initWithFile(spriteFrameName))
    {
        ret->autorelease();
        ret->setPosition(position);
        return ret;
    }
    delete ret;
    return nullptr;
}

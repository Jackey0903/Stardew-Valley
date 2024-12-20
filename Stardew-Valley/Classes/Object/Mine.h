// Mine.h
#pragma once
#include "cocos2d.h"

class Mine : public cocos2d::Sprite
{
public:
    static Mine* createWithPosition(const cocos2d::Vec2& position, const std::string& spriteFrameName);
};

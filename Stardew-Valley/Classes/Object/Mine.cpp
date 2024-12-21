/****************************************************************
  * 项目名        : Stardew-Valley
  * 文件名        : Mine.cpp
  * 文件功能      : Mine类的实现
  * 作者          : 胡浩杰
  * 更新日期      : 2024/12/18
  * 许可证        : MIT License
****************************************************************/

#include "Mine.h"

// 使用 cocos2d 命名空间
USING_NS_CC;

// 静态方法，使用指定位置和精灵帧名称创建 Mine 对象
Mine* Mine::createWithPosition(const Vec2& position, const std::string& spriteFrameName)
{
    // 使用 C++11 的 nothrow 操作符，防止内存分配失败时抛出异常
    Mine* ret = new(std::nothrow) Mine();
    if (ret && ret->initWithFile(spriteFrameName))
    {
        // 设置自动释放
        ret->autorelease();
        // 设置对象位置
        ret->setPosition(position);
        return ret;
    }
    // 初始化失败，释放内存并返回空指针
    delete ret;
    return nullptr;
}

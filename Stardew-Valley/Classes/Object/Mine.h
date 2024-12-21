/****************************************************************
  * 项目名        : Stardew-Valley
  * 文件名        : Mine.h
  * 文件功能      : Mine类的实现
  * 作者          : 胡浩杰
  * 更新日期      : 2024/12/18
  * 许可证        : MIT License
****************************************************************/

#include "cocos2d.h"

// 定义 Mine 类，继承自 cocos2d::Sprite
class Mine : public cocos2d::Sprite
{
public:
    // 静态方法，使用指定的位置和精灵帧名称创建 Mine 对象
    static Mine* createWithPosition(const cocos2d::Vec2& position, const std::string& spriteFrameName);
};

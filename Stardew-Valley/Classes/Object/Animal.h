/****************************************************************
  * 项目名        : Stardew-Valley
  * 文件名        : Animal.h
  * 文件功能      : Animal类的实现
  * 作者          : 曹津硕、胡浩杰
  * 更新日期      : 2024/12/17
  * 许可证        : MIT License
****************************************************************/

#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "cocos2d.h"

// 定义 Animal 类，继承自 cocos2d::Node
class Animal : public cocos2d::Node
{
public:
    // 使用 C++11 的 enum class 定义移动方向
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    // 静态创建函数，接收移动区域和动物名称作为参数
    static Animal* create(const cocos2d::Rect& area, const std::string _name);
    // 初始化函数
    virtual bool init(const cocos2d::Rect& area);

    // 设置移动速度
    void setMovementSpeed(float speed) { _speed = speed; }
    // 更新函数，每帧调用
    void update(float dt) override;
private:
    cocos2d::Sprite* _animalSprite = nullptr; // 动物精灵，初始化为 nullptr（C++11 特性）
    cocos2d::Rect _moveArea;                  // 移动区域
    Direction _currentDir;                    // 当前移动方向
    float _speed;                             // 移动速度
    float _changeDirTime;                     // 下次改变方向的时间计时
    std::string animalName;                   // 动物名称

    void loadAnimations();                           // 加载动画资源
    void startWalkingAnimation(Direction dir);       // 开始行走动画
    void stopWalkingAnimation();                     // 停止行走动画
    void chooseRandomDirection();                    // 随机选择移动方向
    bool isOutOfArea(const cocos2d::Vec2& pos);      // 判断位置是否超出移动区域
};

#endif // __ANIMAL_H__

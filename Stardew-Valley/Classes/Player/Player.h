/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Player.h
 * 文件功能      : 玩家类的头文件
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/20
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
class Backpack;  // 前向声明背包类

// 玩家类，继承自cocos2d的Node类
class Player : public cocos2d::Node
{
public:
    // 创建一个Player对象，返回一个Player指针
    static Player* create();

    // 初始化函数，重载Node类的init方法
    virtual bool init() override;

    // 键盘按下事件处理函数
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 键盘释放事件处理函数
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 更新函数，每帧调用，用于更新玩家的位置
    void update(float delta) override;

    // 获取玩家精灵的指针
    cocos2d::Sprite* getPlayerSprite() const { return _playerSprite; }

    // 碰撞检测，传入MapLayer指针，用于判断是否与墙壁碰撞
    bool isCollidingWithWall(const cocos2d::Rect& playerRect);

    // 开始玩家的行走动画
    void startWalkingAnimation(const std::string& direction);

    // 停止玩家的行走动画
    void stopWalkingAnimation();

    // 打开背包场景
    void openBackpack();

    // 打开地图场景
    void openMapScene();

    // 设置当前地图和初始位置
    void setTiledMap(cocos2d::TMXTiledMap* tiledMap);

    // 设置玩家在地图中的初始位置
    void setInitPositionMap(const cocos2d::Vec2& initMapPosition);

private:
    // 玩家是否正在移动
    bool _isMoving;
    // 玩家各方向的移动状态
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _isMovingUp;
    bool _isMovingDown;

    // 标记玩家是否在背包场景或地图场景中
    bool _isInBackpackScene;
    bool _isInMapScene;

    // 玩家精灵对象，用于显示玩家
    cocos2d::Sprite* _playerSprite;

    // 当前玩家的方向和纹理
    std::string _currentDirection;
    std::string _currentTexture;

    // 背包对象，用于管理玩家的物品
    Backpack* _backpack;

    // 当前地图的指针
    cocos2d::TMXTiledMap* _tiledMap;

    // 初始地图位置
    cocos2d::Vec2 initPositionMap;

    // 键盘事件监听器，C++11特性：智能指针管理事件监听器
    cocos2d::EventListenerKeyboard* _keyboardListener;

    // 可视化玩家碰撞矩形，便于调试碰撞检测
    cocos2d::DrawNode* playerDrawNode;
};

#endif // __PLAYER_H__

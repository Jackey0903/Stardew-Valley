/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Player.h
 * 文件功能      : 玩家类的头文件
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
class Backpack;

class Player : public cocos2d::Node // 改为继承 Node
{
public:
    static Player* create();

    virtual bool init() override;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void update(float delta) override;
    cocos2d::Sprite* getPlayerSprite() const { return _playerSprite; }
    // 碰撞检测传入MapLayer指针
    bool isCollidingWithWall(const cocos2d::Rect& playerRect);

    void startWalkingAnimation(const std::string& direction);
    void stopWalkingAnimation();

    void openBackpack();
    void openMapScene();

    // 设置地图和初始位置
    void setTiledMap(cocos2d::TMXTiledMap* tiledMap) { _tiledMap = tiledMap; }
    void setInitPositionMap(const cocos2d::Vec2& pos) { initPositionMap = pos; }


private:
    bool _isMoving;
    bool _isMovingLeft;
    bool _isMovingRight;
    bool _isMovingUp;
    bool _isMovingDown;

    bool _isInBackpackScene;
    bool _isInMapScene;
    cocos2d::Sprite* _playerSprite;
    std::string _currentDirection;
    std::string _currentTexture;

    Backpack* _backpack;
    cocos2d::TMXTiledMap* _tiledMap;    // 地图指针
    cocos2d::Vec2 initPositionMap;      // 地图初始位置

    cocos2d::Vector<cocos2d::Sprite*> trees;
    int woodCount;
};

#endif // __PLAYER_H__

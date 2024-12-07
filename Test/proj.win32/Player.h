
#include "cocos2d.h"

class Player : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // 实现键盘事件处理函数
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void Player::update(float delta);
    // 动画
    void Player::startWalkingAnimation(const std::string& direction);
    void Player::stopWalkingAnimation();
    bool isCollisionWithWall(const cocos2d::Vec2& nextPosition);
    // implement the "static create()" method manually
    CREATE_FUNC(Player);

private:
    // 定义精灵的成员变量
    cocos2d::Sprite* _playerSprite;
    // 动画
    cocos2d::Animation* _walkAnimation;
    std::string _currentDirection;
    //// 添加背景精灵成员变量
    //cocos2d::Sprite* _backgroundSprite;
    // 当前精灵方向的纹理名称
    std::string _currentTexture;
    // 添加成员变量以跟踪移动状态
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
    bool _isMovingUp = false;
    bool _isMovingDown = false;
    // 添加成员变量来跟踪长按状态
    bool _isMoving = false;
    cocos2d::TMXTiledMap* _tiledMap; // 声明瓦片地图成员变量

    cocos2d::TMXLayer* _wallLayer;    // 墙壁图层
};

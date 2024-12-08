/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : Player.h
 * 文件功能      : 玩家类的功能实现
 * 作者          : 胡浩杰，胡正华，曹津硕
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/
#include "cocos2d.h"
#include "Backpack.h" 
#include "Scene/BackpackScene.h"

class Player : public cocos2d::Scene

{
public:

	cocos2d::TMXTiledMap* _tiledMap; // 声明瓦片地图成员变量




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
    bool isCollidingWithWall(const cocos2d::Vec2& position, cocos2d::TMXLayer* wallLayer);
    // implement the "static create()" method manually
    CREATE_FUNC(Player);
    void openBackpack(); // 打开背包
    Player::~Player() {
        delete _backpack;
    }
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
    bool _isInBackpackScene = false;
    Backpack* _backpack; // 背包数据实例
    bool isCollidingWithWall(const cocos2d::Rect& playerRect);


    std::vector<cocos2d::Rect> _wallRects; // 存储墙体矩形
};

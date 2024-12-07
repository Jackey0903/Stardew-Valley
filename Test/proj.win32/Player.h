
#include "cocos2d.h"

class Player : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // ʵ�ּ����¼�������
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void Player::update(float delta);
    // ����
    void Player::startWalkingAnimation(const std::string& direction);
    void Player::stopWalkingAnimation();
    bool isCollisionWithWall(const cocos2d::Vec2& nextPosition);
    // implement the "static create()" method manually
    CREATE_FUNC(Player);

private:
    // ���徫��ĳ�Ա����
    cocos2d::Sprite* _playerSprite;
    // ����
    cocos2d::Animation* _walkAnimation;
    std::string _currentDirection;
    //// ��ӱ��������Ա����
    //cocos2d::Sprite* _backgroundSprite;
    // ��ǰ���鷽�����������
    std::string _currentTexture;
    // ��ӳ�Ա�����Ը����ƶ�״̬
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
    bool _isMovingUp = false;
    bool _isMovingDown = false;
    // ��ӳ�Ա���������ٳ���״̬
    bool _isMoving = false;
    cocos2d::TMXTiledMap* _tiledMap; // ������Ƭ��ͼ��Ա����

    cocos2d::TMXLayer* _wallLayer;    // ǽ��ͼ��
};

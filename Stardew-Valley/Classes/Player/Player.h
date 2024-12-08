/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Player.h
 * �ļ�����      : �����Ĺ���ʵ��
 * ����          : ���ƽܣ����������ܽ�˶
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/
#include "cocos2d.h"
#include "Backpack.h" 
#include "Scene/BackpackScene.h"

class Player : public cocos2d::Scene

{
public:

	cocos2d::TMXTiledMap* _tiledMap; // ������Ƭ��ͼ��Ա����




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
    bool isCollidingWithWall(const cocos2d::Vec2& position, cocos2d::TMXLayer* wallLayer);
    // implement the "static create()" method manually
    CREATE_FUNC(Player);
    void openBackpack(); // �򿪱���
    Player::~Player() {
        delete _backpack;
    }
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
    bool _isInBackpackScene = false;
    Backpack* _backpack; // ��������ʵ��
    bool isCollidingWithWall(const cocos2d::Rect& playerRect);


    std::vector<cocos2d::Rect> _wallRects; // �洢ǽ�����
};

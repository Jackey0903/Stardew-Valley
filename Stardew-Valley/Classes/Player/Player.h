/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : Player.h
 * �ļ�����      : ������ͷ�ļ�
 * ����          : ���ƽܣ����������ܽ�˶
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
class Backpack;

class Player : public cocos2d::Node // ��Ϊ�̳� Node
{
public:
    static Player* create();

    virtual bool init() override;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void update(float delta) override;

    // ��ײ��⴫��MapLayerָ��
    bool isCollidingWithWall(const cocos2d::Rect& playerRect);

    void startWalkingAnimation(const std::string& direction);
    void stopWalkingAnimation();

    void openBackpack();
    void openMapScene();

    // ���õ�ͼ�ͳ�ʼλ��
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

    std::string _currentDirection;
    std::string _currentTexture;

    cocos2d::Sprite* _playerSprite;
    Backpack* _backpack;
    cocos2d::TMXTiledMap* _tiledMap;    // ��ͼָ��
    cocos2d::Vec2 initPositionMap;      // ��ͼ��ʼλ��
};

#endif // __PLAYER_H__

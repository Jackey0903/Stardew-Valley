/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : BackpackScene.h
 * �ļ�����      : ���������Ĺ���ʵ��
 * ����          : ���ƽ�
 * ��������      : 2024/12/06
 * ���֤        : MIT License
 ****************************************************************/
#ifndef BACKPACKSCENE_H
#define BACKPACKSCENE_H

#include "cocos2d.h"
#include "../Player/Backpack.h" // ����������

class BackpackScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void createMenu(); // �����˵�������ʾ��Ʒ
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // �޸������ռ�
    CREATE_FUNC(BackpackScene);
    Backpack* getBackpack(); // ������ȡ�����ķ���
    cocos2d::Sprite* _playerSprite;           // ��ɫ����
	cocos2d::Sprite* _currentToolSprite;	  // ��ǰ����
    Item* _selectedItem = nullptr;            // ��ǰѡ�е���Ʒ
    void BackpackScene::updatePlayerWithItem();

private:
    Backpack* _backpack; // ˽�г�Ա���������ڴ洢����ʵ��
};


#endif // BACKPACKSCENE_H

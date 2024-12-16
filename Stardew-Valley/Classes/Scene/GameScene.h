#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <string>
#include "Map1Scene.h"
class Player;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(GameScene);

    void createNPC();
    void showDialogue();
    void updateNPCPosition();
    void update(float delta);
    void initEventListeners();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void plantCropAt(const cocos2d::Vec2& locationInMap, cocos2d::TMXTiledMap* tiledMap);
    void harvestCropAt(const cocos2d::Vec2& locationInMap);
    void onMouseDown(cocos2d::Event* event);

private:
    std::string currentMap;
    Player* _player; // �ó�Ա��������player, ��ֹͨ��children����
    cocos2d::Sprite* _npc; // NPC�����ָ��
    bool _isPlanting;
    std::vector<cocos2d::Sprite*> _crops; // �洢���������
    void loadMapLayer(int mapIndex); // ���ز�ͬ��ͼ
    MapLayer* currentMapLayer;
};

#endif

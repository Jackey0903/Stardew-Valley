/****************************************************************
 * ��Ŀ��        : Stardew-Valley
 * �ļ���        : MapScene.h
 * �ļ�����      : ��ͼ������ͷ�ļ�
 * ����          : 
 * ��������      : 2024/12/07
 * ���֤        : MIT License
 ****************************************************************/
#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "MapLayer.h"
#include "Player/Player.h"

class MapScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // �������¼����л���ѡ�еĵ�ͼ
    void onMapItemClicked(cocos2d::Ref* sender);

    CREATE_FUNC(MapScene);

private:
    std::vector<std::string> mapNames;
    size_t currentMapIndex;
    cocos2d::Menu* mapMenu;
    MapLayer* mapLayer;  // ��ͼ��
    Player* player;      // ���
    cocos2d::Vec2 playerPosition; // ���λ��
};

#endif // __MAP_SCENE_H__

// BaseMapScene.h
#ifndef __BASE_MAP_SCENE_H__
#define __BASE_MAP_SCENE_H__

#include "cocos2d.h"
#include <string>
#include <vector>

class Player;
class MapLayer;

class BaseMapScene : public cocos2d::Scene
{
public:
    virtual bool init() override;
    virtual void onEnter() override;

    CREATE_FUNC(BaseMapScene);
    void loadCollisionObjects(const std::string& layerName); // ��������
    bool isCollidingWithCollisionLayer(const cocos2d::Rect& playerRect) const; // ��������
protected:
    std::string currentMap;
    Player* _player;
    MapLayer* _mapLayer;
    cocos2d::TMXTiledMap* _tiledMap;
    std::vector<cocos2d::Rect> _collisionRects; // �����ײ����
    void loadMap(const std::string& mapFile, float scaleFactor);
    void initPlayer();



};

#endif

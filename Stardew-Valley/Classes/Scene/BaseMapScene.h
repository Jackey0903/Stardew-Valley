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
    void loadMap(const std::string& mapFile, float scaleFactor);
    void initPlayer();
protected:
    std::string currentMap;
    Player* _player;
    MapLayer* _mapLayer;
    cocos2d::TMXTiledMap* _tiledMap;

};

#endif

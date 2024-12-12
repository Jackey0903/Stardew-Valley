/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapScene.h
 * 文件功能      : 地图场景类头文件
 * 作者          : 
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
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

    // 处理点击事件，切换到选中的地图
    void onMapItemClicked(cocos2d::Ref* sender);

    CREATE_FUNC(MapScene);

private:
    std::vector<std::string> mapNames;
    size_t currentMapIndex;
    cocos2d::Menu* mapMenu;
    MapLayer* mapLayer;  // 地图层
    Player* player;      // 玩家
    cocos2d::Vec2 playerPosition; // 玩家位置
};

#endif // __MAP_SCENE_H__

/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapLayer.h
 * 文件功能      : 地图层类头文件
 * 作者          : 
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/
#ifndef __MAP_LAYER_H__
#define __MAP_LAYER_H__

#include "cocos2d.h"

class MapLayer : public cocos2d::Layer
{
public:
    virtual bool init();

    // 加载地图
    void loadMap(const std::string& mapFile);

    // 获取墙体矩形
    std::vector<cocos2d::Rect> getWallRects() const { return _wallRects; }

    // 获取当前地图
    cocos2d::TMXTiledMap* getTMXTiledMap() const { return currentMap; }

    CREATE_FUNC(MapLayer);

private:
    cocos2d::TMXTiledMap* currentMap;
    std::string currentMapName;
    std::vector<cocos2d::Rect> _wallRects;
};

#endif // __MAP_LAYER_H__

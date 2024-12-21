/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : BaseMapScene.h
 * 文件功能      : 场景基类的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/21
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __BASE_MAP_SCENE_H__      
#define __BASE_MAP_SCENE_H__

#include "cocos2d.h"               // 引入 Cocos2d 引擎基础类
#include <string>                  // 引入 C++ STL字符串类
#include <vector>                  // 引入 C++ STL容器类

class Player;                     // 前向声明 Player 类
class MapLayer;                   // 前向声明 MapLayer 类

// BaseMapScene 继承自 Cocos2d::Scene
class BaseMapScene : public cocos2d::Scene
{
public:
    virtual bool init() override;

    virtual void onEnter() override;

    CREATE_FUNC(BaseMapScene);

    // 加载地图方法，接收地图文件路径和缩放比例
    void loadMap(const std::string& mapFile, float scaleFactor);

    // 初始化玩家角色方法
    void initPlayer();

protected:
    std::string currentMap;         // 当前地图的文件路径
    Player* _player;                // 玩家指针
    MapLayer* _mapLayer;            // 地图层指针
    cocos2d::TMXTiledMap* _tiledMap; // Tiled 地图对象指针
};

#endif // __BASE_MAP_SCENE_H__

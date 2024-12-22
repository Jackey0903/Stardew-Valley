/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapScene.h
 * 文件功能      : 地图场景类头文件
 * 作者          : 胡浩杰,曹津硕
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "MapLayer.h"
#include "Player/Player.h"
#include <vector>        // 引入标准库STL的vector容器
#include <string>        // 引入标准库STL的string类

 // 使用cocos2d命名空间
class MapScene : public cocos2d::Scene
{
public:
    // 创建场景的静态方法
    static cocos2d::Scene* createScene();

    // 初始化方法，重写基类的init
    virtual bool init() override; // C++11特性：override关键字

    // 使用Cocos2d的CREATE_FUNC宏创建MapScene实例
    CREATE_FUNC(MapScene);

private:
    std::vector<std::string> mapNames; // 使用C++11的std::vector容器
    size_t currentMapIndex;            // 当前地图索引，使用C++11的size_t类型
    cocos2d::Menu* mapMenu;            // 地图菜单
    MapLayer* mapLayer;                // 地图层
    Player* player;                    // 玩家
    cocos2d::Vec2 playerPosition;      // 玩家位置
};

#endif // __MAP_SCENE_H__

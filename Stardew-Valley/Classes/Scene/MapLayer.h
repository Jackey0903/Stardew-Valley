/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapLayer.h
 * 文件功能      : 地图层类头文件
 * 作者          : 胡浩杰、曹津硕
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/

#ifndef __MAP_LAYER_H__
#define __MAP_LAYER_H__

#include "cocos2d.h"
#include <vector>        // 引入标准库STL的vector容器
#include <string>        // 引入标准库STL的string类

 // 使用cocos2d命名空间
class MapLayer : public cocos2d::Layer
{
public:
    // 初始化方法，重写基类的init
    virtual bool init() override;

    // 加载地图并解析碰撞图层
    void loadMap(const std::string& mapFile, float scaleFactor);

    // 获取墙体矩形
    // 使用C++11的成员函数内联定义
    std::vector<cocos2d::Rect> getWallRects() const { return _wallRects; }

    // 获取当前地图
    // 使用C++11的成员函数内联定义
    cocos2d::TMXTiledMap* getTMXTiledMap() const { return currentMap; }

    // 使用Cocos2d的CREATE_FUNC宏创建MapLayer实例
    CREATE_FUNC(MapLayer);

    cocos2d::DrawNode* collisionDrawNode; // 用于可视化碰撞矩形

private:
    cocos2d::TMXTiledMap* currentMap;             // 当前地图对象
    std::string currentMapName;                   // 当前地图名称
    std::vector<cocos2d::Rect> _wallRects;        // 存储墙体矩形

};

#endif // __MAP_LAYER_H__

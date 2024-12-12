/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : MapLayer.cpp
 * 文件功能      : 地图层类实现
 * 作者          : 
 * 更新日期      : 2024/12/07
 * 许可证        : MIT License
 ****************************************************************/
#include "MapLayer.h"
#include "cocos2d.h"

USING_NS_CC;

bool MapLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    currentMap = nullptr;
    currentMapName = "";
    _wallRects.clear();

    return true;
}

void MapLayer::loadMap(const std::string& mapFile)
{
    CCLOG("尝试加载地图: %s", mapFile.c_str());

    // 移除当前地图
    if (currentMap)
    {
        this->removeChild(currentMap, true);
        currentMap = nullptr;
        CCLOG("移除旧地图");
    }

    // 加载新地图
    currentMap = TMXTiledMap::create(mapFile);

    if (currentMap)
    {
        this->addChild(currentMap, 0);

        currentMapName = mapFile;
        CCLOG("地图加载成功: %s", mapFile.c_str());

        // 收集墙体矩形
        auto wallLayer = currentMap->getLayer("wal"); // 确保 "Paths" 是墙体所在的图层名称
        if (wallLayer)
        {
            _wallRects.clear();
            for (int x = 0; x < wallLayer->getLayerSize().width; ++x)
            {
                for (int y = 0; y < wallLayer->getLayerSize().height; ++y)
                {
                    auto tileGID = wallLayer->getTileGIDAt(Vec2(x, y));
                    if (tileGID != 0) // 假设 GID 为 0 表示无效瓦片
                    {
                        Vec2 tilePos = wallLayer->getPositionAt(Vec2(x, y));
                        Size tileSize = currentMap->getTileSize();
                        Rect tileRect = Rect(tilePos.x, tilePos.y, tileSize.width, tileSize.height);
                        _wallRects.push_back(tileRect);
                    }
                }
            }
            CCLOG("收集到 %zu 个墙体矩形。", _wallRects.size());
        }
        else
        {
            CCLOG("未找到墙体图层。");
        }
    }
    else
    {
        CCLOG("地图加载失败: %s", mapFile.c_str());
    }
}

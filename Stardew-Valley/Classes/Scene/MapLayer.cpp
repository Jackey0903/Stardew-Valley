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

    // 初始化用于绘制碰撞矩形的 DrawNode
    collisionDrawNode = DrawNode::create();
    this->addChild(collisionDrawNode, 9); // 确保绘制在地图之上

    return true;
}

void MapLayer::loadMap(const std::string& mapFile, float scaleFactor)
{
    CCLOG("尝试加载地图: %s", mapFile.c_str());

    // 移除当前地图
    if (currentMap)
    {
        this->removeChild(currentMap, true);
        currentMap = nullptr;
        CCLOG("移除旧地图");
    }
    else
    {
        CCLOG("当前没有地图需要移除");
    }

    // 清除之前的碰撞矩形
    _wallRects.clear();
    collisionDrawNode->clear();

    // 加载新地图
    currentMap = TMXTiledMap::create(mapFile);

    if (currentMap)
    {
        this->addChild(currentMap, 0);

        currentMapName = mapFile;
        CCLOG("地图加载成功: %s", mapFile.c_str());

        // 设置地图的锚点，缩放和位置
        currentMap->setAnchorPoint(Vec2::ZERO);
        currentMap->setScale(scaleFactor); 
        currentMap->setPosition(Vec2::ZERO);

        // 收集 "Collision" 对象层的墙体矩形
        TMXObjectGroup* collisionGroup = currentMap->getObjectGroup("Collision");
        if (collisionGroup)
        {
            _wallRects.clear();
            auto objects = collisionGroup->getObjects();
            for (const auto& obj : objects)
            {
                ValueMap objMap = obj.asValueMap();

                // 获取 x, y, width, height
                float x = objMap["x"].asFloat() * scaleFactor;
                float y = objMap["y"].asFloat() * scaleFactor;
                float width = objMap["width"].asFloat() * scaleFactor;
                float height = objMap["height"].asFloat() * scaleFactor;

                Rect wallRect(x, y, width, height); 
                _wallRects.emplace_back(wallRect); 

                // 可视化碰撞矩形
                Vec2 vertices[4] = {
                    Vec2(wallRect.origin.x, wallRect.origin.y),
                    Vec2(wallRect.origin.x + wallRect.size.width, wallRect.origin.y),
                    Vec2(wallRect.origin.x + wallRect.size.width, wallRect.origin.y + wallRect.size.height),
                    Vec2(wallRect.origin.x, wallRect.origin.y + wallRect.size.height)
                };
                collisionDrawNode->drawPolygon(vertices, 4, Color4F(0, 0, 0, 0), 1, Color4F(1, 0, 0, 1));


                CCLOG(u8"收集墙体矩形: (%.2f, %.2f, %.2f, %.2f)", wallRect.origin.x, wallRect.origin.y, wallRect.size.width, wallRect.size.height);
            }
            CCLOG(u8"总共收集到 %zu 个墙体矩形。", _wallRects.size());
        }
        else
        {
            CCLOG(u8"未找到 Collision 对象层。");
        }
    }
    else
    {
        CCLOG(u8"地图加载失败: %s", mapFile.c_str());
    }
}
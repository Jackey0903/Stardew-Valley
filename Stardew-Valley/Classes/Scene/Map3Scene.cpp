// Map3Scene.cpp
#include "Map3Scene.h"
#include "MapLayer.h"
#include "Object/Animal.h"
#include "ui/CocosGUI.h"
#include "Object/Npc.h"
USING_NS_CC;
 
Scene* Map3Scene::createScene()
{
    return Map3Scene::create();
}

bool Map3Scene::init()
{
    if (!BaseMapScene::init())
        return false;

    // 加载Map3，假设BaseMapScene的loadMap会将_tiledMap的anchorPoint设为(0,0)、position为(0,0)，并缩放地图
    // 比如缩放5倍
    loadMap("Map/Map3/map3.tmx", 5.0f);
    initPlayer();

    cocos2d::Rect animalArea(50, 0, 100, 100);

    auto cat = Animal::create(animalArea, "Cat");
    _tiledMap->addChild(cat, 20);
    cat->setScale(0.5f);

    return true;
}

void Map3Scene::onEnter()
{
    BaseMapScene::onEnter();

    // 在Map3上查找npc对象层并创建NPC
    if (_tiledMap)
    {
        // 获取名为"Npc"的对象层
        auto npcLayer = _tiledMap->getObjectGroup("Npc_Leah");
        if (npcLayer)
        {
            auto npcObjects = npcLayer->getObjects();
            for (auto& obj : npcObjects)
            {
                auto npcObject = obj.asValueMap();
                if (npcObject["name"].asString() == "Npc_Leah")
                {
                    // 获取NPC在地图坐标系中的位置（Tiled左下为(0,0)）
                    float npcX = npcObject["x"].asFloat();
                    float npcY = npcObject["y"].asFloat();
                    Vec2 npcPosition(npcX, npcY);
                    CCLOG("Creating Npc_Leah at position: (%.2f, %.2f)", npcPosition.x, npcPosition.y);
                    _npcLeahDialogues = {
                        {u8"Hey,Jackey.", ""},
                        {u8"What’s up with you?", ""},
                        {u8"You look tired.", ""},
                        {u8"You must pay attention to rest.", ""}
                    };
                    LeahOption = { {u8"Got it ,thank you!"},{u8"Okay,dear."},{u8"I’m not too tired."} };
                    // 创建 NpcLeah 实例并添加到地图
                    auto npcLeah = Npc::createWithPosition(npcPosition, "Leah", _npcLeahDialogues, LeahOption);
                    if (npcLeah)
                    {
                        // 根据需要调整缩放或其他属性
                        npcLeah->setScale(0.1f); // 增大NPC的缩放，使点击区域更大
                        _tiledMap->addChild(npcLeah, 25);
                    }
                }
            }
        }
    }
}

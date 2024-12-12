// GameScene.cpp
#include "Scene/GameScene.h"
#include "MapLayer.h"
#include "Player/Player.h"

USING_NS_CC;

extern std::string g_selectedMap;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
        return false;

    currentMap = "Map/Map2/map2.tmx";

    auto mapLayer = MapLayer::create();
    mapLayer->setName("MapLayer");
    this->addChild(mapLayer, 0);
    mapLayer->loadMap(currentMap);

    auto tiledMap = mapLayer->getTMXTiledMap();

    if (!tiledMap)
    {
        CCLOG("loaded ");
    }

    _player = Player::create();
    this->addChild(_player, 10); // 确保玩家在地图之上

    _player->setTiledMap(tiledMap);
    _player->setInitPositionMap(tiledMap->getPosition());

    return true;
}

void GameScene::onEnter()
{
    Scene::onEnter();

    if (!g_selectedMap.empty() && g_selectedMap != currentMap)
    {
        CCLOG("GameScene: 切换到新地图: %s", g_selectedMap.c_str());
        currentMap = g_selectedMap;

        auto mapLayer = dynamic_cast<MapLayer*>(this->getChildByName("MapLayer"));
        if (mapLayer)
        {
            mapLayer->loadMap(currentMap);
            auto tiledMap = mapLayer->getTMXTiledMap();



            // 设置地图的锚点为中心
            tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

            // 获取屏幕的可见区域大小
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto origin = Director::getInstance()->getVisibleOrigin();

            // 计算地图的缩放比例，使其铺满窗口
            float scaleX = visibleSize.width / tiledMap->getContentSize().width * 1.5;
            float scaleY = visibleSize.height / tiledMap->getContentSize().height * 1.5;
            float scale = MAX(scaleX, scaleY); // 选择较大的缩放比例，确保地图铺满窗口
            tiledMap->setScale(scale);

            // 将地图置于屏幕中心
            tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
            if (_player && tiledMap)
            {

                _player->setPosition(Vec2(60.000000, 50.000000)); // 设置玩家的初始位置

                _player->setTiledMap(tiledMap);
                _player->setInitPositionMap(tiledMap->getPosition());


            }
        }
        g_selectedMap.clear();
    }
}

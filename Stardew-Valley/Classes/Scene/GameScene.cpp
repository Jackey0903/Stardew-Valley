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
    this->addChild(_player, 10); // ȷ������ڵ�ͼ֮��

    _player->setTiledMap(tiledMap);
    _player->setInitPositionMap(tiledMap->getPosition());

    return true;
}

void GameScene::onEnter()
{
    Scene::onEnter();

    if (!g_selectedMap.empty() && g_selectedMap != currentMap)
    {
        CCLOG("GameScene: �л����µ�ͼ: %s", g_selectedMap.c_str());
        currentMap = g_selectedMap;

        auto mapLayer = dynamic_cast<MapLayer*>(this->getChildByName("MapLayer"));
        if (mapLayer)
        {
            mapLayer->loadMap(currentMap);
            auto tiledMap = mapLayer->getTMXTiledMap();



            // ���õ�ͼ��ê��Ϊ����
            tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));

            // ��ȡ��Ļ�Ŀɼ������С
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto origin = Director::getInstance()->getVisibleOrigin();

            // �����ͼ�����ű�����ʹ����������
            float scaleX = visibleSize.width / tiledMap->getContentSize().width * 1.5;
            float scaleY = visibleSize.height / tiledMap->getContentSize().height * 1.5;
            float scale = MAX(scaleX, scaleY); // ѡ��ϴ�����ű�����ȷ����ͼ��������
            tiledMap->setScale(scale);

            // ����ͼ������Ļ����
            tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
            if (_player && tiledMap)
            {

                _player->setPosition(Vec2(60.000000, 50.000000)); // ������ҵĳ�ʼλ��

                _player->setTiledMap(tiledMap);
                _player->setInitPositionMap(tiledMap->getPosition());


            }
        }
        g_selectedMap.clear();
    }
}

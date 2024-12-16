// BaseMapScene.cpp
#include "BaseMapScene.h"
#include "MapLayer.h"
#include "Player/Player.h"

USING_NS_CC;

bool BaseMapScene::init()
{
    if (!Scene::init())
        return false;

    _mapLayer = MapLayer::create();
    _mapLayer->setName("MapLayer");
    this->addChild(_mapLayer, 0);

    _tiledMap = nullptr;
    _player = nullptr;

    return true;
}

void BaseMapScene::onEnter()
{
    Scene::onEnter();
    // 子类可在onEnter中根据currentMap调用loadMap()和initPlayer()
}

void BaseMapScene::loadMap(const std::string& mapFile, float scaleFactor)
{
    currentMap = mapFile;
    _mapLayer->loadMap(currentMap);
    _tiledMap = _mapLayer->getTMXTiledMap();
    if (_tiledMap)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        _tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));
        _tiledMap->setScale(scaleFactor);
        _tiledMap->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    }
}

void BaseMapScene::initPlayer()
{
    if (!_tiledMap) return;
    _player = Player::create();
    this->addChild(_player, 10);
    _player->setTiledMap(_tiledMap);
    _player->setInitPositionMap(_tiledMap->getPosition());
    _player->setPosition(Vec2(60, 50)); // 默认玩家初始位置，可子类调节
}

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
    CCLOG("MapLayer 已添加到 BaseMapScene");
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
    CCLOG("BaseMapScene 正在加载地图: %s", mapFile.c_str());
    currentMap = mapFile;
    _mapLayer->loadMap(currentMap, scaleFactor);
    _tiledMap = _mapLayer->getTMXTiledMap();
    if (_tiledMap)
    {
        _tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f)); // 设置地图锚点为中心
        _tiledMap->setPosition(Director::getInstance()->getVisibleSize() / 2); // 可选：将位置设置为可视区域中心
        _tiledMap->setScale(scaleFactor);
        //auto visibleSize = Director::getInstance()->getVisibleSize();
        //auto origin = Director::getInstance()->getVisibleOrigin();
        //_tiledMap->setAnchorPoint(Vec2::ZERO);
        //_tiledMap->setScale(scaleFactor);
        //_tiledMap->setPosition(Vec2::ZERO);
        //this->addChild(_tiledMap, 0);

        CCLOG("地图位置设定为 (%.2f, %.2f)", _tiledMap->getPosition().x, _tiledMap->getPosition().y);
    }
    else
    {
        CCLOG("加载地图失败: %s", mapFile.c_str());
    }
}

//void BaseMapScene::initPlayer()
//{
//    if (!_tiledMap) return;
//    _player = Player::create();
//    this->addChild(_player, 1);
//    _player->setTiledMap(_tiledMap);
//    _player->setInitPositionMap(_tiledMap->getPosition());
//
//    // 设置玩家的初始位置（根据需要调整）
//    // 这里将玩家设置在地图的中心位置
//    Size mapSize = _tiledMap->getContentSize();
//    Vec2 initialPosition = Vec2(mapSize.width / 2, mapSize.height / 2);
//    _player->setPosition(initialPosition);
//    CCLOG("Player 初始化在位置: (%.2f, %.2f)", _player->getPosition().x, _player->getPosition().y);
//    //if (!_tiledMap) return;
//    //_player = Player::create();
//    //this->addChild(_player, 1);
//    //_player->setTiledMap(_tiledMap);
//    //_player->setInitPositionMap(_tiledMap->getPosition());
//    //_player->setPosition(Vec2(60, 50)); // 默认玩家初始位置，可子类调节
//}
void BaseMapScene::initPlayer()
{
    if (!_tiledMap) return;
    _player = Player::create(); 
    if (_player)
    {
        this->addChild(_player, 1);
        _player->setTiledMap(_tiledMap);
        _player->setInitPositionMap(_tiledMap->getPosition());
        _player->setPosition(Vec2(0, 0)); // 默认玩家初始位置，可子类调节
        CCLOG("Player 初始化在位置: (%.2f, %.2f)", _player->getPosition().x, _player->getPosition().y);
    }
    else
    {
        CCLOG("Player 创建失败");
    }
}
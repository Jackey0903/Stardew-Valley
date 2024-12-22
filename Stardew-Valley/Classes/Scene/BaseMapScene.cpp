/****************************************************************
 * 项目名        : Stardew-Valley
 * 文件名        : BaseMapScene.cpp
 * 文件功能      : 场景基类的功能实现
 * 作者          : 胡浩杰
 * 更新日期      : 2024/12/22
 * 许可证        : MIT License
 ****************************************************************/

#include "BaseMapScene.h"
#include "MapLayer.h"
#include "Player/Player.h"

USING_NS_CC;

bool BaseMapScene::init()
{
    if (!Scene::init())        // 初始化场景，确保初始化成功
        return false;

    _mapLayer = MapLayer::create();  
    _mapLayer->setName("MapLayer"); 
    this->addChild(_mapLayer, 0);    // 将地图层添加到场景
    CCLOG("MapLayer 已添加到 BaseMapScene");

    _tiledMap = nullptr;    // 初始化地图指针为nullptr
    _player = nullptr;      // 初始化玩家指针为nullptr

    return true;          
}

void BaseMapScene::onEnter()
{
    Scene::onEnter();       // 调用基类的onEnter，确保进入场景时的基本处理
    // 子类可以在onEnter中根据currentMap调用loadMap()和initPlayer()
}

void BaseMapScene::loadMap(const std::string& mapFile, float scaleFactor)
{
    CCLOG("BaseMapScene 正在加载地图: %s", mapFile.c_str());   
    currentMap = mapFile;  // 设置当前地图文件路径
    _mapLayer->loadMap(currentMap, scaleFactor);

    _tiledMap = _mapLayer->getTMXTiledMap();    // 获取地图对象指针
    if (_tiledMap)   
    {
        _tiledMap->setAnchorPoint(Vec2(0.5f, 0.5f));  // 设置地图锚点为中心
        _tiledMap->setPosition(Director::getInstance()->getVisibleSize() / 2);  // 将地图置于可视区域中心
        _tiledMap->setScale(scaleFactor);    // 设置地图的缩放比例

        CCLOG("地图位置设定为 (%.2f, %.2f)", _tiledMap->getPosition().x, _tiledMap->getPosition().y);   // 输出地图位置
    }
    else
    {
        CCLOG("加载地图失败: %s", mapFile.c_str());  
    }
}

// 初始化玩家角色的方法
void BaseMapScene::initPlayer()
{
    if (!_tiledMap) return;  

    _player = Player::create();    // 创建玩家实例
    if (_player)   
    {
        this->addChild(_player, 1); // 将玩家添加到场景中，层级为1（在地图层之上）
        _player->setTiledMap(_tiledMap);
        _player->setInitPositionMap(_tiledMap->getPosition());

        _player->setPosition(Vec2(0, 0));  
        CCLOG("Player 初始化在位置: (%.2f, %.2f)", _player->getPosition().x, _player->getPosition().y); // 输出玩家位置
    }
    else
    {
        CCLOG("Player 创建失败");   
    }
}

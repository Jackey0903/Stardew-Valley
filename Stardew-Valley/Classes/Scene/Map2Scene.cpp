// Map2Scene.cpp
#include "Map2Scene.h"
#include "MapLayer.h"
#include "Player/Player.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* Map2Scene::createScene()
{
    return Map2Scene::create();
}

bool Map2Scene::init()
{
    if (!BaseMapScene::init())
        return false;

    // 加载Map2并初始化玩家
    loadMap("Map/Map2/map2.tmx", 2.4f);
    initPlayer();

    _isPlanting = false;

    // 初始化事件监听器（键盘、鼠标）
    initEventListeners();

    return true;
}

void Map2Scene::onEnter()
{
    BaseMapScene::onEnter();
    // 无额外逻辑
}

void Map2Scene::initEventListeners()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Map2Scene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Map2Scene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Map2Scene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Map2Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_P)
    {
        _isPlanting = true;
        CCLOG("Planting mode ON");
    }
}

void Map2Scene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_P)
    {
        _isPlanting = false;
        CCLOG("Planting mode OFF");
    }
}

void Map2Scene::onMouseDown(Event* event)
{
    if (!_isPlanting) return;

    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    if (!mouseEvent) return;

    auto mapLayer = this->getChildByName<MapLayer*>("MapLayer");
    if (!mapLayer || !_tiledMap) return;

    auto mouseButton = mouseEvent->getMouseButton();
    Vec2 locationInView(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    auto locationInMap = _tiledMap->convertToNodeSpace(locationInView);

    if (mouseButton == EventMouse::MouseButton::BUTTON_LEFT)
    {
        plantCropAt(locationInMap);
    }
    else if (mouseButton == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        harvestCropAt(locationInMap);
    }
}

void Map2Scene::plantCropAt(const Vec2& locationInMap)
{
    std::vector<std::string> cropImages = {
        "Carrot_1.png","Carrot_2.png","Carrot_3.png",
        "Pumpkin_1.png","Pumpkin_2.png","Pumpkin_3.png",
        "Lajiao_1.png","Lajiao_2.png","Lajiao_3.png"
    };

    static int cropIndex = 0;
    auto crop = Sprite::create(cropImages[cropIndex]);
    crop->setScale(0.1f);
    crop->setPosition(locationInMap);
    _tiledMap->addChild(crop, 15);
    _crops.push_back(crop);

    CCLOG("Planted a crop (%s) at (%.2f,%.2f)", cropImages[cropIndex].c_str(), locationInMap.x, locationInMap.y);
    cropIndex = (cropIndex + 1) % cropImages.size();
}

void Map2Scene::harvestCropAt(const Vec2& locationInMap)
{
    for (auto it = _crops.begin(); it != _crops.end(); ++it)
    {
        auto crop = *it;
        Rect boundingBox = crop->getBoundingBox();
        if (boundingBox.containsPoint(locationInMap))
        {
            crop->removeFromParent();
            _crops.erase(it);
            CCLOG("Harvested a crop at (%.2f,%.2f)", locationInMap.x, locationInMap.y);
            break;
        }
    }
}
